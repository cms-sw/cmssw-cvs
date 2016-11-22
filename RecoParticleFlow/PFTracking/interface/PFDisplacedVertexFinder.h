#ifndef RecoParticleFlow_PFTracking_PFDisplacedVertexFinder_h
#define RecoParticleFlow_PFTracking_PFDisplacedVertexFinder_h 

#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertexCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertexCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertexSeed.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertexSeedFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertex.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertexFwd.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "RecoParticleFlow/PFTracking/interface/PFCheckHitPattern.h"

/// \brief Displaced Vertex Finder Algorithm
/*!
\author Maxime Gouzevitch
\date October 2009
*/

class TrackingGeometry;
class TrackerGeometry;
class MagneticField;

class PFDisplacedVertexFinder {

 public:

  PFDisplacedVertexFinder();

  ~PFDisplacedVertexFinder();

  /// -------- Useful Types -------- ///

  typedef std::set< reco::TrackBaseRef >::iterator IEset;
  typedef reco::PFDisplacedVertexCandidateCollection::iterator IDVC;
  typedef reco::PFDisplacedVertexSeedCollection::iterator IDVS;
  typedef reco::PFDisplacedVertexCollection::iterator IDV;

  typedef std::pair <unsigned int, unsigned int> PFTrackHitInfo;
  typedef std::pair <PFTrackHitInfo, PFTrackHitInfo> PFTrackHitFullInfo;

  /// -------- Set different algo parameters -------- ///

  /// Sets algo parameters for the vertex finder
  void setParameters(double transvSize, double longSize, 
		     double primaryVertexCut, double tobCut, 
		     double tecCut, double minAdaptWeight) {
    transvSize_ = transvSize;
    longSize_   = longSize;
    primaryVertexCut_ = primaryVertexCut;
    tobCut_ = tobCut;
    tecCut_ = tecCut;
    minAdaptWeight_ = minAdaptWeight;
  }

  /// Sets debug printout flag
  void setDebug( bool debug ) {debug_ = debug;}

  /// Sets parameters for track extrapolation and hits study
  void setEdmParameters( const MagneticField* magField,
			    edm::ESHandle<GlobalTrackingGeometry> globTkGeomHandle,
			    edm::ESHandle<TrackerGeometry> tkerGeomHandle){ 
    magField_ = magField; 
    globTkGeomHandle_ = globTkGeomHandle;
    tkerGeomHandle_ = tkerGeomHandle; 
  }


  /// Set input collections of tracks
  void  setInput(const edm::Handle< reco::PFDisplacedVertexCandidateCollection >&); 
  
  
  /// \return auto_ptr to collection of DisplacedVertices
  std::auto_ptr< reco::PFDisplacedVertexCollection > transferDisplacedVertices() {return displacedVertices_;}

  const std::auto_ptr< reco::PFDisplacedVertexCollection >& displacedVertices() const {return displacedVertices_;}



  /// -------- Main function which find vertices -------- ///

  void findDisplacedVertices();


 private:
  
  /// -------- Different steps of the finder algorithm -------- ///

  /// Analyse a vertex candidate and select potential vertex point(s)
  void findSeedsFromCandidate(reco::PFDisplacedVertexCandidate&, reco::PFDisplacedVertexSeedCollection&);

  /// Sometimes two vertex candidates can be quite close and coming from the same vertex
  void mergeSeeds(reco::PFDisplacedVertexSeedCollection&, std::vector<bool>& bLocked);

  /// Fit one by one the vertex points with associated tracks to get displaced vertices
  bool fitVertexFromSeed(reco::PFDisplacedVertexSeed&, std::string, reco::PFDisplacedVertex&);

  /// Remove potentially fakes displaced vertices
  void selectVertices(const reco::PFDisplacedVertexCollection&,  std::vector <bool>&);


  /// -------- Tools -------- ///

  bool isCloseTo(const reco::PFDisplacedVertexSeed&, const reco::PFDisplacedVertexSeed&) const;

  double getTransvDiff(const GlobalPoint&, const GlobalPoint&) const;
  double getLongDiff(const GlobalPoint&, const GlobalPoint&) const;
  double getLongProj(const GlobalPoint&, const GlobalVector&) const;

  reco::PFDisplacedVertex::VertexTrackType getVertexTrackType(PFTrackHitFullInfo&) const;

  unsigned commonTracks(const reco::PFDisplacedVertex&, const reco::PFDisplacedVertex&) const;

  friend std::ostream& operator<<(std::ostream&, const PFDisplacedVertexFinder&);


  /// -------- Members -------- ///

  std::auto_ptr< reco::PFDisplacedVertexCandidateCollection >  displacedVertexCandidates_;
  std::auto_ptr< reco::PFDisplacedVertexCollection >    displacedVertices_;

  /// -------- Parameters -------- ///

  /// Algo parameters for the vertex finder

  double transvSize_;
  double longSize_;
  double primaryVertexCut_;
  double tobCut_;
  double tecCut_;
  double minAdaptWeight_;

  /// If true, debug printouts activated
  bool   debug_;
  
  /// Tracker geometry for discerning hit positions
  edm::ESHandle<GlobalTrackingGeometry> globTkGeomHandle_;

  /// doc? 
  edm::ESHandle<TrackerGeometry> tkerGeomHandle_;

  /// to be able to extrapolate tracks f
  const MagneticField* magField_;

  
  PFCheckHitPattern hitPattern_;

};

#endif


