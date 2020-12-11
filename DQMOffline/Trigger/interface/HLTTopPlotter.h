#ifndef DQMOffline_Trigger_HLTTopPlotter_H
#define DQMOffline_Trigger_HLTTopPlotter_H

/** \class HLTTopPlotter
 *  Get L1/HLT efficiency/rate plots
 *  Documentation available on the CMS TWiki:
 *  https://twiki.cern.ch/twiki/bin/view/CMS/MuonHLTOfflinePerformance
 *
 *  $Date: 2010/07/21 04:23:22 $
 *  $Revision: 1.6 $
 *  
 *  \author  J. Slaunwhite (modified from above
 */

// Base Class Headers
#include "DQMOffline/Trigger/interface/HLTMuonMatchAndPlot.h"
//#include "DQMOffline/Trigger/interface/MuonInformationDump.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
//#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/Math/interface/LorentzVector.h"

#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/MuonReco/interface/Muon.h"
//#include "CommonTools/Utilities/interface/StringCutObjectSelector.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"


#include <vector>
#include "TFile.h"
#include "TNtuple.h"



typedef math::XYZTLorentzVector LorentzVector;

class HLTTopPlotter: public HLTMuonMatchAndPlot {

  // Inheritance means TopPlotter has all the same
  // members and functions as HLTMuonMatchAndPlot
  // In this header we can define extras for top

  
public:

  /// Constructor
  HLTTopPlotter( const edm::ParameterSet& pset, std::string triggerName,
                 std::vector<std::string> moduleNames, MuonSelectionStruct inputSelection,
                 std::string customName,
                 std::vector<std::string> validTriggers,
                 const edm::Run & currentRun,
                 const edm::EventSetup & currentEventSetup);

  // Operations
  void            begin  ( );
  void            analyze( const edm::Event & iEvent );
  void            finish ( );
  //MonitorElement* bookIt ( TString name, TString title, std::vector<double> );
  


private:

  std::vector <MonitorElement*> hDeltaRMaxJetLep;
  std::vector <MonitorElement*> hJetMultip;

  void sortJets (reco::CaloJetCollection & theJets);
  
  double EtaCut_, PtCut_;
  int NJets_;
  edm::InputTag CaloJetInputTag;

  std::vector<double> theJetMParameters;
  

};
#endif
