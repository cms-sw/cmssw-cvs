//
// Package:    FastL1MuonProducer
// Class:      FastL1MuonProducer
// 
/**\class FastL1MuonProducer FastSimulation/Muons/plugins/FastL1MuonProducer.cc

 Description:
    Fast simulation producer for L1 muons.
    L1MuGMTCand's obtained from a parameterization wich starts from the generated
              muons in the event
*/
//
// Original Author:  Andrea Perrotta
// Modifications: Patrick Janot.
//         Created:  Mon Oct 30 14:37:24 CET 2006
//
//

// CMSSW headers 
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Regional eta scales...
#include "CondFormats/L1TObjects/interface/L1MuTriggerScales.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerScalesRcd.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerPtScale.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerPtScaleRcd.h"

// Fast Simulation headers
#include "FastSimulation/Utilities/interface/RandomEngine.h"
#include "FastSimulation/Muons/plugins/FastL1MuonProducer.h"

// SimTrack
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

// L1
#include "FastSimDataFormats/L1GlobalMuonTrigger/interface/SimpleL1MuGMTCand.h"
#include "FastSimulation/Muons/interface/FML1EfficiencyHandler.h"
#include "FastSimulation/Muons/interface/FML1PtSmearer.h"

// STL headers 
#include <iostream>

// Data Formats
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

// Geometry
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include <map>

// constants, enums and typedefs

//
// static data member definitions
//

double FastL1MuonProducer::muonMassGeV_ = 0.105658369 ; // PDG06

//
// constructors and destructor
//
FastL1MuonProducer::FastL1MuonProducer(const edm::ParameterSet& iConfig):
  myL1EfficiencyHandler(0),
  myL1PtSmearer(0)
{

  readParameters(iConfig.getParameter<edm::ParameterSet>("MUONS"));

  //register your products
  produces<L1MuonCollection> ();
  produces<L1ExtraCollection> ();
  produces<L1MuGMTReadoutCollection>();

  // Initialize the random number generator service
  edm::Service<edm::RandomNumberGenerator> rng;
  if ( ! rng.isAvailable() ) {
    throw cms::Exception("Configuration") <<
      "ParamMuonProducer requires the RandomGeneratorService \n"
      "which is not present in the configuration file. \n"
      "You must add the service in the configuration file\n"
      "or remove the module that requires it.";
  }

  random = new RandomEngine(&(*rng));

}


FastL1MuonProducer::~FastL1MuonProducer()
{
 
  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)
  
  if ( random ) { 
    delete random;
  }
}


//
// member functions
//

// ------------ method called to produce the data  ------------

void 
FastL1MuonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  using namespace edm;

  Handle<std::vector<SimTrack> > simMuons;
  iEvent.getByLabel(theSimModule,simMuons);
  unsigned nmuons = simMuons->size();
  //  Handle<std::vector<SimVertex> > simVertices;
  //  iEvent.getByLabel(theSimModuleLabel_,simVertices);

  Handle<PSimHitContainer> muonDTHits;
  iEvent.getByLabel(theDTHits,muonDTHits);
  Handle<PSimHitContainer> muonCSCHits;
  iEvent.getByLabel(theCSCHits,muonCSCHits);
  Handle<PSimHitContainer> muonRPCHits;
  iEvent.getByLabel(theRPCHits,muonRPCHits);


//
// Loop over generated muons and reconstruct L1, L3 and Global muons
//
  
  int nMu = 0;
  mySimpleL1MuonCands.clear();
  mySimpleL1MuonExtraCands.clear();

  std::multimap<float,SimpleL1MuGMTCand*>  mySimpleL1MuonCandsTemp;

  for( unsigned fsimi=0; fsimi < nmuons; ++fsimi) {
    // The sim track can be a muon or a decaying hadron
    const SimTrack& mySimTrack = (*simMuons)[fsimi];
    // Keep only the muons at L1 (either primary or secondary)
    int pid = mySimTrack.type();        
    if ( fabs(pid) != 13 ) continue;

    // Check whether there are hits in DT/CSC/RPC,
    // and keep for the L1 mu the position of first such hit:
    bool hasPSimHits = false;
    GlobalPoint glbPosition;
    PSimHitContainer::const_iterator simDTHit=muonDTHits->begin();
    PSimHitContainer::const_iterator endDTHit=muonDTHits->end();
    for ( ; simDTHit!=endDTHit; ++simDTHit) {
      if ( simDTHit->trackId() == mySimTrack.trackId() ) {
        glbPosition = dtGeometry->idToDet(simDTHit->detUnitId())->surface().toGlobal(simDTHit->localPosition());
        hasPSimHits = true;
        break;
      }
    }

    if (! hasPSimHits) {
      PSimHitContainer::const_iterator simCSCHit=muonCSCHits->begin();
      PSimHitContainer::const_iterator endCSCHit=muonCSCHits->end();
      for ( ; simCSCHit!=endCSCHit; ++simCSCHit) {
	if ( simCSCHit->trackId() == mySimTrack.trackId() ) {
	  glbPosition = cscGeometry->idToDet(simCSCHit->detUnitId())->surface().toGlobal(simCSCHit->localPosition());
	  hasPSimHits = true;
	  break;
	}
      }
    }

    if (! hasPSimHits) {
      PSimHitContainer::const_iterator simRPCHit=muonRPCHits->begin();
      PSimHitContainer::const_iterator endRPCHit=muonRPCHits->end();
      for ( ; simRPCHit!=endRPCHit; ++simRPCHit) {
	if ( simRPCHit->trackId() == mySimTrack.trackId() ) {
	  glbPosition = rpcGeometry->idToDet(simRPCHit->detUnitId())->surface().toGlobal(simRPCHit->localPosition());
	  hasPSimHits = true;
	  break;
	}
      }
    }

    // *** Reconstruct parameterized muons starting from undecayed simulated muons
    if (hasPSimHits) {

      nMu++;

      //
      // Now L1 parametrization
      //
      double pT = mySimTrack.momentum().pt();
      double eta = glbPosition.eta();
      // Avoid L1MuScales complains if |eta|>2.4:
      if (eta > 2.4) eta = 2.4-1e-6; else if (eta < -2.4) eta = -2.4+1e-6;
      double phi = glbPosition.phi();
      if ( phi < 0. ) phi = 2* M_PI + phi;

      unsigned etaIndex = theMuScales->getGMTEtaScale()->getPacked(eta);
      unsigned phiIndex = theMuScales->getPhiScale()->getPacked(phi);
      unsigned pTIndex = theMuPtScale->getPtScale()->getPacked(pT);
      float etaValue = theMuScales->getGMTEtaScale()->getLowEdge(etaIndex);
      float phiValue = theMuScales->getPhiScale()->getLowEdge(phiIndex);
      float pTValue = theMuPtScale->getPtScale()->getLowEdge(pTIndex) + 1e-6;
      float etaValue2 = theMuScales->getGMTEtaScale()->getLowEdge(etaIndex+1);
      float phiValue2 = theMuScales->getPhiScale()->getLowEdge(phiIndex+1);
      float pTValue2 = theMuPtScale->getPtScale()->getLowEdge(pTIndex+1) + 1e-6;
     
      // Choose the closest index. (Not sure it is what is to be done)
      if ( fabs(etaValue2 - eta) < fabs(etaValue-eta) ) { 
	etaValue = etaValue2;
	++etaIndex;
      }
      if ( fabs(phiValue2-phi) < fabs(phiValue-phi) ) { 
	phiValue = phiValue2;
	++phiIndex;
      }
      if ( fabs(pTValue2-pT) < fabs(pTValue-pT) ) { 
	pTValue = pTValue2;
	++pTIndex;
      }

      SimpleL1MuGMTCand * thisL1MuonCand = 
	new SimpleL1MuGMTCand(&mySimTrack,
			      etaIndex, phiIndex, pTIndex,
			      etaValue,phiValue,pTValue);
      bool hasL1 = myL1EfficiencyHandler->kill(thisL1MuonCand);
      if (hasL1) {
	bool status2 = myL1PtSmearer->smear(thisL1MuonCand);
	if (!status2) { std::cout << "Pt smearing of L1 muon went wrong!!" << std::endl; }
	if (status2) {
	  mySimpleL1MuonCandsTemp.insert(
	       std::pair<float,SimpleL1MuGMTCand*>(thisL1MuonCand->ptValue(),thisL1MuonCand));
	}
	else {
	  delete thisL1MuonCand;
	}
      }

    }

  }
  
  // kill low ranked L1 muons, and fill L1extra muons -->
  std::multimap<float,SimpleL1MuGMTCand*>::const_reverse_iterator L1mu = mySimpleL1MuonCandsTemp.rbegin();
  std::multimap<float,SimpleL1MuGMTCand*>::const_reverse_iterator lastL1mu = mySimpleL1MuonCandsTemp.rend();
  unsigned rank=0;
  unsigned rankb=0;
  unsigned rankf=0;
  for ( ; L1mu!=lastL1mu; ++L1mu ) {
    SimpleL1MuGMTCand* theMuon = L1mu->second;
    theMuon->setRPCBit(0);
    ++rank;
    bool addMu = false;
    if  (theMuon->isFwd() ) { 
      if ( rankf < 4 ) addMu = true;
      theMuon->setRPCIndex(rankf);
      theMuon->setDTCSCIndex(rankf);
      rankf++;
    } else {  
      if ( rankb < 4 ) addMu = true;
      theMuon->setRPCIndex(rankb);
      theMuon->setDTCSCIndex(rankb);
      rankb++;
    } 

    if ( addMu ) {
      theMuon->setRank(rank);
      mySimpleL1MuonCands.push_back(theMuon);
      double pt = theMuon->ptValue() + 1.e-6 ;
      double eta = theMuon->etaValue();
      double phi = theMuon->phiValue();
      math::PtEtaPhiMLorentzVector PtEtaPhiMP4(pt,eta,phi,muonMassGeV_);
      math::XYZTLorentzVector myL1P4(PtEtaPhiMP4);
      // math::PtEtaPhiMLorentzVector myL1P4(pt,eta,phi,muonMassGeV_);
      mySimpleL1MuonExtraCands.push_back( l1extra::L1MuonParticle( theMuon->charge(), myL1P4, *theMuon ) );
    }
    else {
      theMuon->setRank(0);
    }

  }

// end killing of low ranked L1 muons -->


  int nL1 =  mySimpleL1MuonCands.size();
  nMuonTot   += nMu;
  nL1MuonTot += nL1;

  std::auto_ptr<L1MuonCollection> l1Out(new L1MuonCollection);
  std::auto_ptr<L1ExtraCollection> l1ExtraOut(new L1ExtraCollection);
  std::auto_ptr<L1MuGMTReadoutCollection> l1ReadOut(new L1MuGMTReadoutCollection(1));
  
  loadL1Muons(*l1Out,*l1ExtraOut,*l1ReadOut);
  iEvent.put(l1Out);
  iEvent.put(l1ExtraOut);
  iEvent.put(l1ReadOut);

  L1mu = mySimpleL1MuonCandsTemp.rbegin();
  for ( ; L1mu!=lastL1mu; ++L1mu ) {
    delete L1mu->second;
  }
}


void FastL1MuonProducer::loadL1Muons(L1MuonCollection & c , 
				     L1ExtraCollection & d,
				     L1MuGMTReadoutCollection & e) const
{

  FML1Muons::const_iterator l1mu;
  L1ExtraCollection::const_iterator l1ex;
  L1MuGMTReadoutRecord rc = L1MuGMTReadoutRecord(0);

  // Add L1 muons:
  // int nmuons = mySimpleL1MuonCands.size();
  for (l1mu=mySimpleL1MuonCands.begin();l1mu!=mySimpleL1MuonCands.end();++l1mu) {
    c.push_back(*(*l1mu));
  }
  
// Add extra particles.
  int nr = 0; 
  int nrb = 0; 
  int nrf = 0;
  for (l1ex=mySimpleL1MuonExtraCands.begin();l1ex!=mySimpleL1MuonExtraCands.end();++l1ex) {

    d.push_back(*l1ex);
    
    L1MuGMTExtendedCand aMuon(l1ex->gmtMuonCand());

    rc.setGMTCand(nr,aMuon);
    // Find the regional eta index
    double etaPilePoil = mySimpleL1MuonCands[nr]->getMomentum().Eta();

    nr++;
    unsigned typeRPC=0;
    unsigned typeDTCSC=0;
    unsigned RPCIndex=0;
    unsigned DTCSCIndex=0;
    unsigned RPCRegionalEtaIndex=0;
    unsigned DTCSCRegionalEtaIndex=0;
    float etaRPCValue=-10.;
    float etaDTCSCValue=-10.;
    // Forward muons
    if ( aMuon.isFwd() ) { 

      rc.setGMTFwdCand(nrf,aMuon);

      // CSC
      typeDTCSC = 2;
      DTCSCIndex = 8+nrf;
      DTCSCRegionalEtaIndex = theMuScales->getRegionalEtaScale(2)->getPacked(etaPilePoil); 
      etaDTCSCValue = theMuScales->getRegionalEtaScale(2)->getLowEdge(DTCSCRegionalEtaIndex);
      float etaDTCSCValue2 = theMuScales->getRegionalEtaScale(2)->getLowEdge(DTCSCRegionalEtaIndex+1);
      if ( fabs(etaDTCSCValue2-etaPilePoil) < fabs(etaDTCSCValue-etaPilePoil) ) { 
	etaDTCSCValue = etaDTCSCValue2;
	++DTCSCRegionalEtaIndex;
      }
      // RPC (limited to the RPC acceptance)
      if ( fabs(etaPilePoil) < 2.1 ) { 
	RPCIndex = 12+nrf;
	typeRPC = 3;
	RPCRegionalEtaIndex = theMuScales->getRegionalEtaScale(3)->getPacked(etaPilePoil); 
	etaRPCValue = theMuScales->getRegionalEtaScale(3)->getLowEdge(RPCRegionalEtaIndex);
	float etaRPCValue2 = theMuScales->getRegionalEtaScale(3)->getLowEdge(RPCRegionalEtaIndex+1);
	if ( fabs(etaRPCValue2-etaPilePoil) < fabs(etaRPCValue-etaPilePoil) ) { 
	  etaRPCValue = etaRPCValue2;
	  ++RPCRegionalEtaIndex;
	}
      }
      // Next muon
      nrf++;

    // Barrel muons
    } else { 

      rc.setGMTBrlCand(nrb,aMuon);

      // DT
      typeDTCSC = 0;
      DTCSCIndex = 0+nrb;
      DTCSCRegionalEtaIndex = theMuScales->getRegionalEtaScale(0)->getPacked(etaPilePoil); 
      etaDTCSCValue = theMuScales->getRegionalEtaScale(0)->getLowEdge(DTCSCRegionalEtaIndex);
      float etaDTCSCValue2 = theMuScales->getRegionalEtaScale(0)->getLowEdge(DTCSCRegionalEtaIndex+1);
      if ( fabs(etaDTCSCValue2-etaPilePoil) < fabs(etaDTCSCValue-etaPilePoil) ) { 
	etaDTCSCValue = etaDTCSCValue2;
	++DTCSCRegionalEtaIndex;
      }

      // RPC
      typeRPC = 1;
      RPCIndex = 4+nrb;
      RPCRegionalEtaIndex = theMuScales->getRegionalEtaScale(1)->getPacked(etaPilePoil); 
      etaRPCValue = theMuScales->getRegionalEtaScale(1)->getLowEdge(RPCRegionalEtaIndex);
      float etaRPCValue2 = theMuScales->getRegionalEtaScale(1)->getLowEdge(RPCRegionalEtaIndex+1);
      if ( fabs(etaRPCValue2-etaPilePoil) < fabs(etaRPCValue-etaPilePoil) ) { 
	etaRPCValue = etaRPCValue2;
	++RPCRegionalEtaIndex;
      }

      // Next muon
      nrb++;

    }

    // Add a muon regional candidate - first DT/CSC
    L1MuRegionalCand regionalMuonDTCSC = 
      L1MuRegionalCand(typeDTCSC,
		       aMuon.phiIndex(),
		       DTCSCRegionalEtaIndex,
		       aMuon.ptIndex(),
		       (1-aMuon.charge())/2,
		       aMuon.charge_valid(),
		       1,   // FineHalo
		       aMuon.quality(),
		       aMuon.bx());
    regionalMuonDTCSC.setPhiValue(aMuon.phiValue());
    regionalMuonDTCSC.setEtaValue(etaDTCSCValue);
    regionalMuonDTCSC.setPtValue(aMuon.ptValue());    
    
    rc.setInputCand(DTCSCIndex,regionalMuonDTCSC);
 
    // Then RPC (if in RPC acceptance)
    if ( fabs(etaPilePoil) < 2.1 ) { 
      L1MuRegionalCand regionalMuonRPC = 
	L1MuRegionalCand(typeRPC,
			 aMuon.phiIndex(),
			 RPCRegionalEtaIndex,
			 aMuon.ptIndex(),
			 (1-aMuon.charge())/2,
			 aMuon.charge_valid(),
			 0,   // FineHalo
			 aMuon.quality(),
			 aMuon.bx());
      regionalMuonRPC.setPhiValue(aMuon.phiValue());
      regionalMuonRPC.setEtaValue(etaRPCValue);
      regionalMuonRPC.setPtValue(aMuon.ptValue());

      rc.setInputCand(RPCIndex,regionalMuonRPC);

    }

  }

  // Update the event
  e.addRecord(rc);

}

// ------------ method called once each job just before starting event loop  ------------
void 
FastL1MuonProducer::beginJob()
{

  // Initialize
  nMuonTot = 0;

  nL1MuonTot = 0;
  mySimpleL1MuonCands.clear();
  mySimpleL1MuonExtraCands.clear();
  myL1EfficiencyHandler = new FML1EfficiencyHandler(random);
  myL1PtSmearer = new FML1PtSmearer(random);


}

void 
FastL1MuonProducer::beginRun(edm::Run & run, 
			     const edm::EventSetup & es) {

// Get the DT Geometry
  es.get<MuonGeometryRecord>().get(dtGeometry);
// Get the CSC Geometry
  es.get<MuonGeometryRecord>().get(cscGeometry);
// Get the RPC Geometry
  es.get<MuonGeometryRecord>().get(rpcGeometry);

  // Read trigger scales
  edm::ESHandle< L1MuTriggerScales > muScales ;
  es.get< L1MuTriggerScalesRcd >().get( muScales ) ;
  theMuScales = &(*muScales);

  edm::ESHandle< L1MuTriggerPtScale > muPtScale ;
  es.get< L1MuTriggerPtScaleRcd >().get( muPtScale ) ;
  theMuPtScale = &(*muPtScale);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
FastL1MuonProducer::endJob() {

  if (myL1EfficiencyHandler) delete myL1EfficiencyHandler;
  if ( myL1PtSmearer) delete myL1PtSmearer;
  std::cout << " ===> FastL1MuonProducer , final report." << std::endl;
  std::cout << " ===> Number of total -> L1 muons in the whole job : "
            <<   nMuonTot << " -> " << nL1MuonTot << std::endl;
}


void FastL1MuonProducer::readParameters(const edm::ParameterSet& fastMuons) {
  // Muons
  theSimModule = fastMuons.getParameter<edm::InputTag>("simModule");
  theDTHits = fastMuons.getParameter<edm::InputTag>("dtSimHits");
  theCSCHits = fastMuons.getParameter<edm::InputTag>("cscSimHits");
  theRPCHits = fastMuons.getParameter<edm::InputTag>("rpcSimHits");

}


