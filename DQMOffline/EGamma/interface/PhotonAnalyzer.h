#ifndef PhotonAnalyzer_H
#define PhotonAnalyzer_H

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
//
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
// DataFormats
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"



/// EgammaCoreTools
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalEtaPhiRegion.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

// Geometry
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"
//
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TVector3.h"
#include "TProfile.h"
//

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

//
//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//
#include <map>
#include <vector>

/** \class PhotonAnalyzer
 **  
 **
 **  $Id: PhotonAnalyzer
 **  $Date: 2010/01/12 13:50:25 $ 
 **  authors: 
 **   Nancy Marinelli, U. of Notre Dame, US  
 **   Jamie Antonelli, U. of Notre Dame, US
 **     
 ***/


// forward declarations
class TFile;
class TH1F;
class TH2F;
class TProfile;
class TTree;
class SimVertex;
class SimTrack;


class PhotonAnalyzer : public edm::EDAnalyzer
{

 public:
   
  //
  explicit PhotonAnalyzer( const edm::ParameterSet& ) ;
  virtual ~PhotonAnalyzer();
                                   
      
  virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
  virtual void beginJob() ;
  virtual void endJob() ;
 
 private:
  //

  float  phiNormalization( float& a);

  void book3DHistoVector();
  void book2DHistoVector();
  void book1DHistoVector();

  void fill3DHistoVector(std::vector<std::vector<std::vector<MonitorElement*> > >& histoVector,double x, int cut, int type, int part);
  void fill2DHistoVector(std::vector<std::vector<MonitorElement*> >& histoVector,double x, double y, int cut, int type);
  void fill2DHistoVector(std::vector<std::vector<MonitorElement*> >& histoVector,double x, int cut, int type);
  void fill1DHistoVector();


  //////////

  std::string fName_;
  int verbosity_;

  unsigned int prescaleFactor_;

  std::string photonProducer_;       
  std::string photonCollection_;

  std::string barrelRecHitProducer_;
  std::string barrelRecHitCollection_;

  std::string endcapRecHitProducer_;
  std::string endcapRecHitCollection_;

  edm::InputTag triggerEvent_;

  double minPhoEtCut_;
  double invMassEtCut_;


  double cutStep_;
  int numberOfSteps_;

  bool useBinning_;
  bool useTriggerFiltering_;
  bool standAlone_;
  std::string outputFileName_;
  

  int isolationStrength_; 


  edm::ParameterSet parameters_;
           
  ////////

  DQMStore *dbe_;
  std::stringstream currentFolder_;

  int nEvt_;
  int nEntry_;

  std::vector<std::string> triggerLabels;
   
  //////////

  MonitorElement*  h_filters_;

  MonitorElement* h_phoEta_Loose_;
  MonitorElement* h_phoEta_Tight_;
  MonitorElement* h_phoEta_HLT_;
  MonitorElement* h_phoEt_Loose_;
  MonitorElement* h_phoEt_Tight_;
  MonitorElement* h_phoEt_HLT_;

  MonitorElement* h_convEta_Loose_;
  MonitorElement* h_convEta_Tight_;
  MonitorElement* h_convEt_Loose_;
  MonitorElement* h_convEt_Tight_;


  MonitorElement* h_phoEta_Vertex_;

  MonitorElement* h_invMassTwoWithTracks_;
  MonitorElement* h_invMassOneWithTracks_;
  MonitorElement* h_invMassZeroWithTracks_;
  MonitorElement* h_invMassAllPhotons_;

 ////////2D vectors of histograms

  std::vector<MonitorElement*> h_nTrackIsolSolidVsEta_isol_;
  std::vector<MonitorElement*> h_trackPtSumSolidVsEta_isol_;
  std::vector<MonitorElement*> h_nTrackIsolHollowVsEta_isol_;
  std::vector<MonitorElement*> h_trackPtSumHollowVsEta_isol_;
  std::vector<MonitorElement*> h_ecalSumVsEta_isol_;
  std::vector<MonitorElement*> h_hcalSumVsEta_isol_;

  std::vector<std::vector<MonitorElement*> > h_nTrackIsolSolidVsEta_;
  std::vector<std::vector<MonitorElement*> > h_trackPtSumSolidVsEta_;
  std::vector<std::vector<MonitorElement*> > h_nTrackIsolHollowVsEta_;
  std::vector<std::vector<MonitorElement*> > h_trackPtSumHollowVsEta_;
  std::vector<std::vector<MonitorElement*> > h_ecalSumVsEta_;
  std::vector<std::vector<MonitorElement*> > h_hcalSumVsEta_;

  std::vector<MonitorElement*> h_nTrackIsolSolidVsEt_isol_;
  std::vector<MonitorElement*> h_trackPtSumSolidVsEt_isol_;
  std::vector<MonitorElement*> h_nTrackIsolHollowVsEt_isol_;
  std::vector<MonitorElement*> h_trackPtSumHollowVsEt_isol_;
  std::vector<MonitorElement*> h_ecalSumVsEt_isol_;
  std::vector<MonitorElement*> h_hcalSumVsEt_isol_;

  std::vector<std::vector<MonitorElement*> > h_nTrackIsolSolidVsEt_;
  std::vector<std::vector<MonitorElement*> > h_trackPtSumSolidVsEt_;
  std::vector<std::vector<MonitorElement*> > h_nTrackIsolHollowVsEt_;
  std::vector<std::vector<MonitorElement*> > h_trackPtSumHollowVsEt_;
  std::vector<std::vector<MonitorElement*> > h_ecalSumVsEt_;
  std::vector<std::vector<MonitorElement*> > h_hcalSumVsEt_;

  std::vector<MonitorElement*> h_nTrackIsolSolid_isol_;
  std::vector<MonitorElement*> h_trackPtSumSolid_isol_;
  std::vector<MonitorElement*> h_nTrackIsolHollow_isol_;
  std::vector<MonitorElement*> h_trackPtSumHollow_isol_;
  std::vector<MonitorElement*> h_ecalSum_isol_;
  std::vector<MonitorElement*> h_hcalSum_isol_;

  std::vector<std::vector<MonitorElement*> > h_nTrackIsolSolid_;
  std::vector<std::vector<MonitorElement*> > h_trackPtSumSolid_;
  std::vector<std::vector<MonitorElement*> > h_nTrackIsolHollow_;
  std::vector<std::vector<MonitorElement*> > h_trackPtSumHollow_;
  std::vector<std::vector<MonitorElement*> > h_ecalSum_;
  std::vector<std::vector<MonitorElement*> > h_hcalSum_;



  std::vector<MonitorElement*> p_nTrackIsolSolidVsEta_isol_;
  std::vector<MonitorElement*> p_trackPtSumSolidVsEta_isol_;
  std::vector<MonitorElement*> p_nTrackIsolHollowVsEta_isol_;
  std::vector<MonitorElement*> p_trackPtSumHollowVsEta_isol_;
  std::vector<MonitorElement*> p_ecalSumVsEta_isol_;
  std::vector<MonitorElement*> p_hcalSumVsEta_isol_;

  std::vector<std::vector<MonitorElement*> > p_nTrackIsolSolidVsEta_;
  std::vector<std::vector<MonitorElement*> > p_trackPtSumSolidVsEta_;
  std::vector<std::vector<MonitorElement*> > p_nTrackIsolHollowVsEta_;
  std::vector<std::vector<MonitorElement*> > p_trackPtSumHollowVsEta_;
  std::vector<std::vector<MonitorElement*> > p_ecalSumVsEta_;
  std::vector<std::vector<MonitorElement*> > p_hcalSumVsEta_;

  std::vector<MonitorElement*> p_nTrackIsolSolidVsEt_isol_;
  std::vector<MonitorElement*> p_trackPtSumSolidVsEt_isol_;
  std::vector<MonitorElement*> p_nTrackIsolHollowVsEt_isol_;
  std::vector<MonitorElement*> p_trackPtSumHollowVsEt_isol_;


  std::vector<std::vector<MonitorElement*> > p_nTrackIsolSolidVsEt_;
  std::vector<std::vector<MonitorElement*> > p_trackPtSumSolidVsEt_;
  std::vector<std::vector<MonitorElement*> > p_nTrackIsolHollowVsEt_;
  std::vector<std::vector<MonitorElement*> > p_trackPtSumHollowVsEt_;


  std::vector<MonitorElement*> p_ecalSumVsEt_part_;
  std::vector<MonitorElement*> p_hcalSumVsEt_part_;

  std::vector<std::vector<MonitorElement*> > p_ecalSumVsEt_isol_;
  std::vector<std::vector<MonitorElement*> > p_hcalSumVsEt_isol_;

  std::vector<std::vector<std::vector<MonitorElement*> > > p_ecalSumVsEt_;
  std::vector<std::vector<std::vector<MonitorElement*> > > p_hcalSumVsEt_;


  std::vector<MonitorElement*> p_r9VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > p_r9VsEt_;

  std::vector<MonitorElement*> p_e1x5VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > p_e1x5VsEt_;

  std::vector<MonitorElement*> p_r9VsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_r9VsEta_;

  std::vector<MonitorElement*> p_e1x5VsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_e1x5VsEta_;

  std::vector<MonitorElement*> p_e2x5VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > p_e2x5VsEt_;

  std::vector<MonitorElement*> p_e2x5VsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_e2x5VsEta_;

  std::vector<MonitorElement*> p_r1x5VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > p_r1x5VsEt_;

  std::vector<MonitorElement*> p_r1x5VsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_r1x5VsEta_;

  std::vector<MonitorElement*> p_r2x5VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > p_r2x5VsEt_;

  std::vector<MonitorElement*> p_r2x5VsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_r2x5VsEta_;

  std::vector<MonitorElement*> p_sigmaIetaIetaVsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_sigmaIetaIetaVsEta_;

  std::vector<MonitorElement*> p_dCotTracksVsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_dCotTracksVsEta_;


  std::vector<MonitorElement*> p_hOverEVsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_hOverEVsEta_;
  std::vector<MonitorElement*> p_hOverEVsEt_isol_;
  std::vector<std::vector<MonitorElement*> > p_hOverEVsEt_;


  std::vector<MonitorElement*> h_phoEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoEta_;
  std::vector<MonitorElement*> h_phoPhi_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoPhi_;

  std::vector<MonitorElement*> h_phoEta_BadChannels_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoEta_BadChannels_;
  std::vector<MonitorElement*> h_phoEt_BadChannels_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoEt_BadChannels_;
  std::vector<MonitorElement*> h_phoPhi_BadChannels_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoPhi_BadChannels_;


  std::vector<MonitorElement*> h_scEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_scEta_;
  std::vector<MonitorElement*> h_scPhi_isol_;
  std::vector<std::vector<MonitorElement*> > h_scPhi_;

  std::vector<MonitorElement*> h_phoConvEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoConvEta_;
  std::vector<MonitorElement*> h_phoConvPhi_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoConvPhi_;

  std::vector<MonitorElement*> h_convVtxRvsZ_isol_;
  std::vector<std::vector<MonitorElement*> > h_convVtxRvsZ_;
  std::vector<MonitorElement*> h_convVtxZ_isol_;
  std::vector<std::vector<MonitorElement*> > h_convVtxZ_;
  std::vector<MonitorElement*> h_convVtxYvsX_isol_;
  std::vector<std::vector<MonitorElement*> > h_convVtxYvsX_;
  std::vector<MonitorElement*> h_convVtxR_isol_;
  std::vector<std::vector<MonitorElement*> > h_convVtxR_;

  std::vector<MonitorElement*> h_r9VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > h_r9VsEt_;

  std::vector<MonitorElement*> h_r9VsEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_r9VsEta_;


/*   std::vector<MonitorElement*> h_profiler9VsEt_isol_; */
/*   std::vector<std::vector<MonitorElement*> > h_profiler9VsEt_; */

/*   std::vector<MonitorElement*> h_profiler9VsEta_isol_; */
/*   std::vector<std::vector<MonitorElement*> > h_profiler9VsEta_; */








  std::vector<MonitorElement*> h_e1x5VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > h_e1x5VsEt_;

  std::vector<MonitorElement*> h_e1x5VsEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_e1x5VsEta_;

  std::vector<MonitorElement*> h_e2x5VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > h_e2x5VsEt_;

  std::vector<MonitorElement*> h_e2x5VsEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_e2x5VsEta_;

  std::vector<MonitorElement*> h_r1x5VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > h_r1x5VsEt_;

  std::vector<MonitorElement*> h_r1x5VsEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_r1x5VsEta_;

  std::vector<MonitorElement*> h_r2x5VsEt_isol_;
  std::vector<std::vector<MonitorElement*> > h_r2x5VsEt_;

  std::vector<MonitorElement*> h_r2x5VsEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_r2x5VsEta_;



  std::vector<MonitorElement*> h_phoSigmaIetaIeta_isol_;
  std::vector<std::vector<MonitorElement*> > h_phoSigmaIetaIeta_;
 
  std::vector<MonitorElement*> h_sigmaIetaIetaVsEta_isol_;
  std::vector<std::vector<MonitorElement*> > h_sigmaIetaIetaVsEta_;


  std::vector<MonitorElement*> h_tkChi2_isol_;
  std::vector<std::vector<MonitorElement*> > h_tkChi2_;

  std::vector<MonitorElement*> h_vertexChi2_isol_;
  std::vector<std::vector<MonitorElement*> > h_vertexChi2_;

  std::vector<MonitorElement*> p_nHitsVsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_nHitsVsEta_;

  std::vector<MonitorElement*> p_tkChi2VsEta_isol_;
  std::vector<std::vector<MonitorElement*> > p_tkChi2VsEta_;


  ////////3D vectors of histograms

  std::vector<MonitorElement*> h_phoE_part_;
  std::vector<std::vector<MonitorElement*> > h_phoE_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_phoE_;

  std::vector<MonitorElement*> h_phoEt_part_;
  std::vector<std::vector<MonitorElement*> > h_phoEt_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_phoEt_;

  std::vector<MonitorElement*> h_r9_part_;
  std::vector<std::vector<MonitorElement*> > h_r9_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_r9_;

  std::vector<MonitorElement*> h_hOverE_part_;
  std::vector<std::vector<MonitorElement*> > h_hOverE_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_hOverE_;

  std::vector<MonitorElement*> h_h1OverE_part_;
  std::vector<std::vector<MonitorElement*> > h_h1OverE_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_h1OverE_;

  std::vector<MonitorElement*> h_h2OverE_part_;
  std::vector<std::vector<MonitorElement*> > h_h2OverE_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_h2OverE_;

  std::vector<MonitorElement*> h_nPho_part_;
  std::vector<std::vector<MonitorElement*> > h_nPho_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_nPho_;

  std::vector<MonitorElement*> h_phoDistribution_part_;
  std::vector<std::vector<MonitorElement*> > h_phoDistribution_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_phoDistribution_;


  std::vector<MonitorElement*> h_phoConvE_part_;
  std::vector<std::vector<MonitorElement*> > h_phoConvE_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_phoConvE_;

  std::vector<MonitorElement*> h_phoConvEt_part_;
  std::vector<std::vector<MonitorElement*> > h_phoConvEt_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_phoConvEt_;

  std::vector<MonitorElement*> h_phoConvR9_part_;
  std::vector<std::vector<MonitorElement*> > h_phoConvR9_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_phoConvR9_;

  std::vector<MonitorElement*> h_nConv_part_;
  std::vector<std::vector<MonitorElement*> > h_nConv_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_nConv_;

  std::vector<MonitorElement*> h_eOverPTracks_part_;
  std::vector<std::vector<MonitorElement*> > h_eOverPTracks_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_eOverPTracks_;

  std::vector<MonitorElement*> h_pOverETracks_part_;
  std::vector<std::vector<MonitorElement*> > h_pOverETracks_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_pOverETracks_;

  std::vector<MonitorElement*> h_dCotTracks_part_;
  std::vector<std::vector<MonitorElement*> > h_dCotTracks_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_dCotTracks_;

  std::vector<MonitorElement*> h_dPhiTracksAtVtx_part_;
  std::vector<std::vector<MonitorElement*> > h_dPhiTracksAtVtx_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_dPhiTracksAtVtx_;

  std::vector<MonitorElement*> h_dPhiTracksAtEcal_part_;
  std::vector<std::vector<MonitorElement*> > h_dPhiTracksAtEcal_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_dPhiTracksAtEcal_;

  std::vector<MonitorElement*> h_dEtaTracksAtEcal_part_;
  std::vector<std::vector<MonitorElement*> > h_dEtaTracksAtEcal_isol_;
  std::vector<std::vector<std::vector<MonitorElement*> > > h_dEtaTracksAtEcal_;



  //
  //
  


};





#endif




