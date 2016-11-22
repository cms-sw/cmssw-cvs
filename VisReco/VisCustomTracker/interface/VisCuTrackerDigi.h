#ifndef VIS_CUSTOMTRACKER_VIS_CU_TRACKER_DIGI_H
# define VIS_CUSTOMTRACKER_VIS_CU_TRACKER_DIGI_H

# include "VisReco/VisCustomTracker/interface/VisCuCmsTracker.h"
# include "VisReco/VisCustomTracker/interface/VisCuDetectorRep.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "FWCore/Framework/interface/Event.h"
# include <vector>

class PixelDigi;
class SiStripDigi;
class SoSeparator;
class TrackerGeometry;
class QPainter;
class VisCuTkModule;
class VisCuTkMap2D;

class VisCuTrackerDigi
{
public:
  VisCuTrackerDigi(void);
  
  void 		setEvent(const edm::Event &event, const edm::EventSetup &eventSetup);
  SoSeparator  *update(VisCuDetectorRep::DetectorRep type);
  void 		setTracker(VisCuCmsTracker *tr){m_tracker=tr;}
  void 		drawdigi(VisCuTkModule *mod, int nlay, QPainter *p);
  void 		setMap(VisCuTkMap2D *m){m_map=m;}
  std::string  	updatetext(VisCuTkModule *mod);
  
  std::string 	stripdigiProducerName;
  std::string	stripdigiLabelName;
  std::string 	pixeldigiProducerName;
  std::string	pixeldigiLabelName;
  
private:
  bool 		m_firsteventloaded;
  bool		m_horizontal_view;
  VisCuCmsTracker *m_tracker;
  VisCuTkMap2D *m_map;

  std::vector<PixelDigi> 	m_mdigi;
  std::vector<SiStripDigi> 	m_sdigi;
  edm::ESHandle<TrackerGeometry> m_pDD;
};

# endif // VIS_CUSTOMTRACKER_VIS_CU_TRACKER_DIGI_H
