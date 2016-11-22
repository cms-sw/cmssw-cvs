#ifndef VIS_CUTRACKER_VIS_TK_GEOMETRY_TWIG_H
# define VIS_CUTRACKER_VIS_TK_GEOMETRY_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Iguana/Framework/interface/IgTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
# include "DataFormats/GeometryVector/interface/GlobalPoint.h"
# include "VisReco/VisCustomTracker/interface/VisCuTracker.h"
# include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
# include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"
# include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
# include "VisFramework/VisFrameworkBase/interface/VisSelectedTrackNumber.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkNevToIntegrate.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkSignalToIntegrate.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkAccessDbConnection.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkAutoIntegrate.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkCrossedModulesOnly.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkTrackProducer.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkXSize.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkYSize.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkSimhitProducer.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkStripRechitProducer.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkPixelRechitProducer.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkPixelDigiProducer.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkPixelDigiLabel.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkStripDigiProducer.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkStripDigiLabel.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkPixelClusterProducer.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkPixelClusterLabel.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkStripClusterProducer.h"
# include "VisFramework/VisFrameworkBase/interface/VisCuTkStripClusterLabel.h"
# include "VisFramework/VisFrameworkBase/interface/VisSiStripCluster2dRep.h"
# include "VisFramework/VisFrameworkBase/interface/VisSiStripClusterPointSize.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
namespace edm 
{
    class Event;
    class EventSetup;
}
namespace reco
{
    class Track;
}
class VisCuTkBuilder;
class VisCuCmsTracker;
class VisCuTkMapWindow;
class VisCuTkSlWindow;
class VisCuTrackerDigi;
class VisCuTrackerCluster;
class VisCuTrackerRecHit;
class VisCuTrackerSimHit;
class VisCuTkModule;
class SoSeparator;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCuTkGeometryTwig : public VisQueuedTwig, public VisCuTracker
{
public:
    VisCuTkGeometryTwig (IgState *state, IgTwig *parent, const std::string &name = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual ~VisCuTkGeometryTwig (void);

    virtual void		onNewEvent (const edm::Event &event,
					    const edm::EventSetup &eventSetup);
    using VisQueuedTwig::update;
    virtual void        	update (IgTextRep *rep);
    virtual void       		update (Ig3DRep *rep);
    virtual void        	update (IgRPhiRep *rep);
    virtual void        	update (IgRZRep *rep);
    virtual void                configChanged (void); 
    virtual void                dispatchChanges (void); 

private:
    const edm::ParameterSet *pset;
    edm::ESHandle<TrackerGeometry>  m_pDD;
    edm::ESHandle<SiStripDetCabling> m_SiStripDetCabling; 
    TrackerGeometry * m_TrackerGeometry;
    unsigned long long m_cacheID_geo;
    unsigned long long m_cacheID_cab;
    std::string			m_name;
    bool			m_done;
    SoSeparator		       *m_3dDet;
    SoSeparator		       *m_rzDet;
    SoSeparator		       *m_rphiDet;
    SoSeparator		       *digi3d;
    SoSeparator		       *cluster3d;
    SoSeparator		       *rechit3d;
    SoSeparator		       *simhit3d;
    bool			m_3Ddone;
    bool			alreadySelected;
    bool			m_rzdone;
    bool			m_rphidone;
    VisCuCmsTracker * tr;
    VisCuTkBuilder * tkMap;
    VisCuTkMapWindow *winTkMap;
    VisCuTkSlWindow *winTkSl;
    int timesDispatchChangesCalled;
    VisCuTkNevToIntegrate nevToIntegrate;
    VisCuTkAutoIntegrate  autoIntegrate;
    VisCuTkSignalToIntegrate signalToIntegrate;
    VisCuTkAccessDbConnection accessingDb;
    VisCuTkCrossedModulesOnly crossedModulesOnly;
    VisCuTkTrackProducer trackProducerName;
    VisCuTkXSize  xSize;
    VisCuTkYSize  ySize;
    VisCuTkSimhitProducer  simhitProducer;
    VisCuTkStripRechitProducer  striprechitProducer;
    VisCuTkPixelRechitProducer  pixelrechitProducer;
    VisCuTkPixelDigiProducer  pixeldigiProducer;
    VisCuTkPixelDigiLabel  pixeldigiLabel;
    VisCuTkStripDigiProducer  stripdigiProducer;
    VisCuTkStripDigiLabel  stripdigiLabel;
    VisCuTkPixelClusterProducer  pixelclusterProducer;
    VisCuTkPixelClusterLabel  pixelclusterLabel;
    VisCuTkStripClusterProducer  stripclusterProducer;
    VisCuTkStripClusterLabel  stripclusterLabel;
    VisSiStripClusterPointSize  stripclusterPointSize;
    VisSiStripCluster2dRep  stripcluster2dRep;
    VisSelectedTrackNumber m_track;
    std::vector<VisCuTkModule *>selectedModules;
    VisCuTrackerDigi *digidraw;
    VisCuTrackerCluster *clusterdraw;
    VisCuTrackerRecHit *rechitdraw;
    VisCuTrackerSimHit *simhitdraw;
    int previous_event;
    std::vector<reco::Track> 	m_tracks;
    std::vector<std::vector<VisCuTkModule * > >  m_trackModules; 
    void selectCrossedModules();
    void storeTracks(const edm::Event &event);
    std::string trackLabelName;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_CUTRACKER_VIS_TK_GEOMETRY_TWIG_H
