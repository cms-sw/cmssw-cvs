#ifndef VIS_CUSTOMTRACKER_VIS_CU_TRACKER_SIMHIT_H
# define VIS_CUSTOMTRACKER_VIS_CU_TRACKER_SIMHIT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisReco/VisCustomTracker/interface/VisCuCmsTracker.h"
# include "VisReco/VisCustomTracker/interface/VisCuDetectorRep.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "FWCore/Framework/interface/Event.h"
# include "SimDataFormats/TrackingHit/interface/PSimHit.h"
# include <vector>

class SoSeparator;
class QPainter;
class TrackerGeometry;
class VisCuTkModule;
class VisCuTkMap2D;

class VisCuTrackerSimHit
{
public:
  VisCuTrackerSimHit(void);
  
  void 		setEvent(const edm::Event &event, const edm::EventSetup &eventSetup);
  SoSeparator  *update(VisCuDetectorRep::DetectorRep type);
  void 		setTracker(VisCuCmsTracker *tr) {m_tracker=tr;}
  void 		drawsimhit(VisCuTkModule *mod, int nlay, QPainter *p);
  void 		setMap(VisCuTkMap2D *m) {m_map=m;}
  std::string  	updatetext(VisCuTkModule *mod);
  
  std::string 	simhitProducerName;

private:
  VisCuCmsTracker 	*m_tracker;
  VisCuTkMap2D 		*m_map;
  std::vector<PSimHit> 	m_trackerHits;
  edm::ESHandle<TrackerGeometry> m_pDD;
  bool 			m_firsteventloaded;
  bool			m_horizontal_view;
};

#endif // VIS_CUSTOMTRACKER_VIS_CU_TRACKER_SIMHIT_H
