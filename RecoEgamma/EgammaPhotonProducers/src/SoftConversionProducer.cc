#include <iostream>
#include <vector>
#include <memory>

// Framework
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
//
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"
//
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TrackTransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/PatternTools/interface/TwoTrackMinimumDistance.h"

#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "RecoEgamma/EgammaPhotonAlgos/interface/ConversionTrackEcalImpactPoint.h"
#include "RecoEgamma/EgammaPhotonAlgos/interface/ConversionTrackPairFinder.h"
#include "RecoEgamma/EgammaPhotonAlgos/interface/ConversionVertexFinder.h"

#include "DataFormats/EgammaTrackReco/interface/TrackCaloClusterAssociation.h"
#include "RecoEgamma/EgammaPhotonProducers/interface/SoftConversionProducer.h"

#include "Math/GenVector/VectorUtil.h"


SoftConversionProducer::SoftConversionProducer(const edm::ParameterSet& config) : conf_(config) {

  LogDebug("SoftConversionProducer") << " SoftConversionProducer CTOR " << "\n";
  
  conversionOITrackProducer_              = conf_.getParameter<std::string>("conversionOITrackProducer");
  conversionIOTrackProducer_              = conf_.getParameter<std::string>("conversionIOTrackProducer");
  outInTrackClusterAssociationCollection_ = conf_.getParameter<std::string>("outInTrackClusterAssociationCollection");
  inOutTrackClusterAssociationCollection_ = conf_.getParameter<std::string>("inOutTrackClusterAssociationCollection");
  clusterType_                            = conf_.getParameter<std::string>("clusterType");
  clusterBarrelCollection_                = conf_.getParameter<edm::InputTag>("clusterBarrelCollection");
  clusterEndcapCollection_                = conf_.getParameter<edm::InputTag>("clusterEndcapCollection");
  softConversionCollection_               = conf_.getParameter<std::string>("softConversionCollection");
  trackMaxChi2_                           = conf_.getParameter<double>("trackMaxChi2");
  trackMinHits_                           = conf_.getParameter<double>("trackMinHits");
  clustersMaxDeltaEta_                    = conf_.getParameter<double>("clustersMaxDeltaEta");
  clustersMaxDeltaPhi_                    = conf_.getParameter<double>("clustersMaxDeltaPhi");
  
  theTrackPairFinder_ = 0;
  theVertexFinder_ = 0;
  theEcalImpactPositionFinder_ = 0;
  
  // Register the product
  produces< reco::ConversionCollection >(softConversionCollection_);

}


SoftConversionProducer::~SoftConversionProducer() {}


void  SoftConversionProducer::beginRun (edm::Run& r, edm::EventSetup const & theEventSetup) {
  
  //get magnetic field
  theEventSetup.get<IdealMagneticFieldRecord>().get(theMF_);  
  
  // instantiate the Track Pair Finder algorithm
  theTrackPairFinder_ = new ConversionTrackPairFinder ();

  // instantiate the Vertex Finder algorithm
  theVertexFinder_ = new ConversionVertexFinder (conf_);
  
  // instantiate the algorithm for finding the position of the track extrapolation at the Ecal front face
  theEcalImpactPositionFinder_ = new   ConversionTrackEcalImpactPoint ( &(*theMF_) );

}


void  SoftConversionProducer::endRun (edm::Run& r, edm::EventSetup const & theEventSetup) {

  if(theTrackPairFinder_) delete theTrackPairFinder_;
  if(theVertexFinder_) delete theVertexFinder_;
  if(theEcalImpactPositionFinder_) delete theEcalImpactPositionFinder_; 

}



void SoftConversionProducer::produce(edm::Event& theEvent, const edm::EventSetup& theEventSetup) {
  
  edm::Handle<reco::TrackCollection> outInTrkHandle;
  theEvent.getByLabel(conversionOITrackProducer_,  outInTrkHandle);
   
  edm::Handle<reco::TrackCaloClusterPtrAssociation> outInTrkClusterAssocHandle;
  theEvent.getByLabel( conversionOITrackProducer_, outInTrackClusterAssociationCollection_, outInTrkClusterAssocHandle);

  edm::Handle<reco::TrackCollection> inOutTrkHandle;
  theEvent.getByLabel(conversionIOTrackProducer_, inOutTrkHandle);
  
  edm::Handle<reco::TrackCaloClusterPtrAssociation> inOutTrkClusterAssocHandle;
  theEvent.getByLabel( conversionIOTrackProducer_, inOutTrackClusterAssociationCollection_, inOutTrkClusterAssocHandle);

  edm::Handle<edm::View<reco::CaloCluster> > clusterBarrelHandle;
  theEvent.getByLabel(clusterBarrelCollection_, clusterBarrelHandle);

  edm::Handle<edm::View<reco::CaloCluster> > clusterEndcapHandle;
  if(clusterType_ == "BasicCluster"){
    //theEvent.getByLabel(clusterEndcapProducer_, clusterEndcapCollection_, clusterEndcapHandle);
    theEvent.getByLabel(clusterEndcapCollection_, clusterEndcapHandle);
  }

  // create temporary map to loop over tracks conveniently
  TrackClusterMap trackClusterMap;

  int nTracksOI = (int) outInTrkHandle->size();
  //  std::cout << "  nTracksOI " <<  nTracksOI << std::endl;
  for(int itrk=0; itrk<nTracksOI; itrk++){
    reco::TrackRef tRef(outInTrkHandle,itrk);
    if(!trackQualityCut(tRef)) continue;
    reco::CaloClusterPtr cRef = (*outInTrkClusterAssocHandle)[tRef];
    trackClusterMap.push_back(make_pair(tRef,cRef));
  }

  int nTracksIO = (int) inOutTrkHandle->size();
  // std::cout << "  nTracksIO " <<  nTracksIO << std::endl;
  for(int itrk=0; itrk<nTracksIO; itrk++){
    reco::TrackRef tRef(inOutTrkHandle,itrk);
    if(!trackQualityCut(tRef)) continue;
    reco::CaloClusterPtr cRef = (*inOutTrkClusterAssocHandle)[tRef];
    trackClusterMap.push_back(make_pair(tRef,cRef));
  }

  // Transform Track into TransientTrack (needed by the Vertex fitter)
  edm::ESHandle<TransientTrackBuilder> theTransientTrackBuilder;
  theEventSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTransientTrackBuilder);

  // the output collection to be produced from this producer
  std::auto_ptr< reco::ConversionCollection > outputColl(new reco::ConversionCollection);

  // prepare iterator
  TrackClusterMap::iterator iter1    = trackClusterMap.begin();
  TrackClusterMap::iterator iter2    = trackClusterMap.begin();
  TrackClusterMap::iterator iter_end = trackClusterMap.end();



  // double-loop to make pairs
  for( ; iter1 != iter_end; iter1++) {
    //    std::cout << " back to start of loop 1 " << std::endl;
    const reco::TrackRef trk1 = iter1->first;

    for(iter2 = iter1+1; iter2 != iter_end; iter2++) {
      //      std::cout << " back to start of loop 2 " << std::endl;
      const reco::TrackRef trk2 = iter2->first;
      if(trk1 == trk2) continue;


      const reco::CaloClusterPtr cls1 = iter1->second;
      reco::TransientTrack tsk1 = theTransientTrackBuilder->build(*trk1);

      const reco::CaloClusterPtr cls2 = iter2->second;
      reco::TransientTrack tsk2 = theTransientTrackBuilder->build(*trk2);

      //      std::cout << " after transient " << std::endl;
      // std::cout << " eta1 " << cls1->position().Eta() << " eta2 " << cls2->position().Eta() << std::endl;

      if ( ( tsk1.innermostMeasurementState().globalParameters().position() - tsk2.innermostMeasurementState().globalParameters().position() ).mag() < 1e-7 &&
	   ( tsk1.innermostMeasurementState().globalParameters().momentum() - tsk2.innermostMeasurementState().globalParameters().momentum() ).mag() < 1e-7 ) continue;


      double dEta = std::abs(cls1->position().Eta() - cls2->position().Eta());
      if(dEta > clustersMaxDeltaEta_) continue;
      double dPhi = std::abs(ROOT::Math::VectorUtil::DeltaPhi(cls1->position(),cls2->position()));
      if(dPhi > clustersMaxDeltaPhi_) continue;

      std::vector<reco::TransientTrack> toBeFitted;
      toBeFitted.push_back(tsk1);
      toBeFitted.push_back(tsk2);

      // 
      TwoTrackMinimumDistance md;
      md.calculate  (  tsk1.initialFreeState(),  tsk2.initialFreeState() );
      float minAppDist = md.distance(); 
 


      //      std::cout << " Before vertex " << std::endl;
      reco::Vertex theConversionVertex = (reco::Vertex) theVertexFinder_->run(toBeFitted);
      //std::cout << " After vertex " << std::endl;

      if(theConversionVertex.isValid()){
	//	std::cout << " valid vertex " << std::endl;
	reco::CaloClusterPtrVector scRefs;
	scRefs.push_back(cls1);
	scRefs.push_back(cls2);
	//std::cout << " after push back scref " << std::endl; 
	std::vector<reco::CaloClusterPtr> clusterRefs;
	clusterRefs.push_back(cls1);
	clusterRefs.push_back(cls2);
	//std::cout << " after push back slusterref " << std::endl; 
	std::vector<reco::TrackRef> trkRefs;
	trkRefs.push_back(trk1);
	trkRefs.push_back(trk2);

	std::vector<math::XYZVector> trackPin;
	std::vector<math::XYZVector> trackPout;
        trackPin.push_back( trk1->innerMomentum());
        trackPin.push_back( trk2->innerMomentum());
        trackPout.push_back( trk1->outerMomentum());
        trackPout.push_back( trk2->outerMomentum());

	//	std::cout << " Before impact finder " << std::endl;
	std::vector<math::XYZPoint> trkPositionAtEcal = theEcalImpactPositionFinder_->find(toBeFitted,clusterBarrelHandle);
	//	std::cout << " After impact finder " << std::endl;
	if((clusterType_ == "BasicCluster") && (std::abs(cls2->position().Eta()) > 1.5)){
	  trkPositionAtEcal.clear();
	  trkPositionAtEcal = theEcalImpactPositionFinder_->find(toBeFitted,clusterEndcapHandle);
	}

        double dummy = -9999.;
        std::vector<math::XYZPoint> dummyVec;
	reco::Conversion  newCandidate(scRefs,  trkRefs,  trkPositionAtEcal, theConversionVertex, clusterRefs,  minAppDist,dummyVec,  trackPin, trackPout, dummy );

	// Check this candidate is already in the collection.
	// This is checking that two tracks in a conversion candidate are identicial.

	if(NotAlreadyIn(newCandidate,outputColl)) outputColl->push_back(newCandidate);

	// 	printf("=====> run(%d), event(%d) <=====\n",theEvent.id().run(),theEvent.id().event());
	//	printf("Found a softConverion with vtxR(%f), vtxEta(%f), pt(%f), pt1(%f), pt2(%f)\n",
	//      newCandidate.conversionVertex().position().rho(),newCandidate.conversionVertex().position().eta(),
	//      newCandidate.pairMomentum().perp(),trk1->momentum().rho(),trk2->momentum().rho());

	clusterRefs.clear();
	trkRefs.clear();
	trkPositionAtEcal.clear();
      }// if(theConversionVertex.isValid()

      toBeFitted.clear();

    }// end of iter2
  }// end of iter1

  //  std::cout << " Putting stuff in the event " << std::endl;
  // put the collection into the event
  theEvent.put( outputColl, softConversionCollection_);
  
}


bool SoftConversionProducer::trackQualityCut(const reco::TrackRef& trk){
  return (trk->numberOfValidHits() >= trackMinHits_ && trk->normalizedChi2() < trackMaxChi2_);
}


bool SoftConversionProducer::NotAlreadyIn(const reco::Conversion& thisConv,
					  const std::auto_ptr<reco::ConversionCollection>& outputColl) const {

  if(outputColl->size() == 0) return true;

  reco::ConversionCollection::const_iterator it = outputColl->begin();
  reco::ConversionCollection::const_iterator it_end = outputColl->end();
  for( ; it != it_end; it++){
    const reco::Conversion& conv = *it;
    if((thisConv.tracks()[0] == conv.tracks()[0] && thisConv.tracks()[1] == conv.tracks()[1]) ||
       (thisConv.tracks()[0] == conv.tracks()[1] && thisConv.tracks()[1] == conv.tracks()[0]))
      return false;
  }// for
  
  return true;
}
