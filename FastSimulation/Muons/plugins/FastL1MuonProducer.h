#ifndef FastSimulation_Muons_FastL1MuonProducer_h
#define FastSimulation_Muons_FastL1MuonProducer_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"

// Geometry
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"



#include<vector>

class SimpleL1MuGMTCand;
class L1MuGMTCand;
class FML1EfficiencyHandler;
class FML1PtSmearer;
class L1MuGMTReadoutCollection;
class L1MuTriggerScales;
class L1MuTriggerPtScale;

class RandomEngine;

namespace l1extra { 
  class L1MuonParticle;
}


namespace edm { 
  class ParameterSet;
  class Event;
  class EventSetup;
}

// Data Formats
//#include "DataFormats/MuonReco/interface/Muon.h"

//
// class declaration
//

class FastL1MuonProducer : public edm::EDProducer {

 public:
  
  explicit FastL1MuonProducer(const edm::ParameterSet&);
  ~FastL1MuonProducer();
  
 private:
  
  const RandomEngine * random;
  
  typedef std::vector<SimpleL1MuGMTCand*> FML1Muons;
  typedef std::vector<L1MuGMTCand> L1MuonCollection;
  typedef std::vector<l1extra::L1MuonParticle> L1ExtraCollection;
  
  virtual void beginJob() ;
  virtual void beginRun(edm::Run & run, const edm::EventSetup & es);
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  void readParameters(const edm::ParameterSet&);
  void reconstruct();
  void loadL1Muons(L1MuonCollection& c, 
		   L1ExtraCollection& d, 
		   L1MuGMTReadoutCollection& e) const;
  // ---------- member data ---------------------------
  
  FML1Muons  mySimpleL1MuonCands;
  L1ExtraCollection mySimpleL1MuonExtraCands;
  FML1EfficiencyHandler * myL1EfficiencyHandler;
  FML1PtSmearer * myL1PtSmearer;

  edm::ESHandle<DTGeometry> dtGeometry;
  edm::ESHandle<CSCGeometry> cscGeometry;
  edm::ESHandle<RPCGeometry> rpcGeometry;
  
  // ----------- parameters ---------------------------- 
  edm::InputTag theSimModule;
  edm::InputTag theDTHits;
  edm::InputTag theCSCHits;
  edm::InputTag theRPCHits;
  //  double minEta_ ,  maxEta_;
  
  // Regional Eta scales
  const L1MuTriggerScales* theMuScales;  

  // Pt scale
  const L1MuTriggerPtScale* theMuPtScale;  

  // ----------- counters ------------------------------
  int   nMuonTot , nL1MuonTot;
  
  static double muonMassGeV_ ;

};

#endif
