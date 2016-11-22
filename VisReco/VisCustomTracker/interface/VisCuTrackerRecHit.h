#ifndef VIS_CUSTOMTRACKER_VIS_CU_TRACKER_RECHIT_H
# define VIS_CUSTOMTRACKER_VIS_CU_TRACKER_RECHIT_H

# include "VisReco/VisCustomTracker/interface/VisCuCmsTracker.h"
# include "VisReco/VisCustomTracker/interface/VisCuDetectorRep.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "FWCore/Framework/interface/Event.h"
# include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
# include "DataFormats/GeometryVector/interface/LocalPoint.h"
# include <vector>

class SoSeparator;
class QPainter;
class TrackerGeometry;
class VisCuTkModule;
class VisCuTkMap2D;

class VisCuTrackerRecHit
{
public:
  VisCuTrackerRecHit(void);
  
  void 		setEvent(const edm::Event &event, const edm::EventSetup &eventSetup);
  SoSeparator  *update(VisCuDetectorRep::DetectorRep type);
  void 		setTracker(VisCuCmsTracker *tr) {m_tracker=tr;}
  void 		drawrechit(VisCuTkModule *mod, int nlay, QPainter *p);
  void 		setMap(VisCuTkMap2D *m) {m_map=m;}
  std::string  	updatetext(VisCuTkModule *mod);
  
  std::string 	striprechitProducerName;
  std::string 	pixelrechitProducerName;

private:
  VisCuCmsTracker      *m_tracker;
  VisCuTkMap2D 	       *m_map;
  bool 			m_firsteventloaded;
  bool			m_horizontal_view;
  std::vector<std::pair<LocalPoint, const GeomDetUnit* > > m_rdigi;
  edm::ESHandle<TrackerGeometry> m_geom;
};

#endif // VIS_CUSTOMTRACKER_VIS_CU_TRACKER_RECHIT_H
