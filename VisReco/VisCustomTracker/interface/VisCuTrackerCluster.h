#ifndef VIS_CUSTOMTRACKER_VIS_CU_TRACKER_CLUSTER_H
# define VIS_CUSTOMTRACKER_VIS_CU_TRACKER_CLUSTER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisReco/VisCustomTracker/interface/VisCuCmsTracker.h"
# include "VisReco/VisCustomTracker/interface/VisCuDetectorRep.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "FWCore/Framework/interface/Event.h"
# include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
# include <vector>

class SiStripCluster;
class SiPixelCluster;
class SoSeparator;
class TrackerGeometry;
class QPainter;
class VisCuTkModule;
class VisCuTkMap2D;

class VisCuTrackerCluster 
{
public:
  VisCuTrackerCluster(void);
  
  void 		setEvent(const edm::Event &event, const edm::EventSetup &eventSetup);
  SoSeparator  *update(VisCuDetectorRep::DetectorRep type);
  void 		setTracker(VisCuCmsTracker *tr){m_tracker=tr;}
  void 		drawcluster(VisCuTkModule *mod, int nlay, QPainter *p);
  void 		setMap(VisCuTkMap2D * m){m_map=m;}
  std::string  	updatetext(VisCuTkModule *mod);
  
  std::string 	stripclusterProducerName;
  std::string	stripclusterLabelName;
  std::string 	pixelclusterProducerName;
  std::string 	pixelclusterLabelName;
  std::string 	siStripClusterRepresentation;
  unsigned int  clusterPointSize;

private:
  bool 		m_firsteventloaded;
  bool		m_horizontal_view;
  VisCuCmsTracker *m_tracker;
  VisCuTkMap2D *m_map;
  edm::ESHandle<TrackerGeometry> m_pDD;
  std::vector<SiStripCluster> m_cdigi;
  std::vector<SiPixelCluster> m_cpdigi;
};

#endif // VIS_CUSTOMTRACKER_VIS_CU_TRACKER_CLUSTER_H
