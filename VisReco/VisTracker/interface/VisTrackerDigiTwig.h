#ifndef VIS_TRACKER_VIS_TRACKER_DIGI_TWIG_H
# define VIS_TRACKER_VIS_TRACKER_DIGI_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisLegoScale.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SiStripDigi;
class SoSeparator;
class SoCube;
class SoTransform;
class TrackerGeometry;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisTrackerDigiTwig : public VisQueuedTwig
{
public:
    VisTrackerDigiTwig (IgState *state, IgTwig *parent, 
			const std::string &name = "",
			const std::string &friendlyName = "",
			const std::string &moduleLabel = "",
			const std::string &instanceName = "",
			const std::string &processName = "");

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);
    using VisQueuedTwig::update;
    virtual void	update (Ig3DRep *rep);
    virtual void 	update (IgLegoRep *rep);
    virtual void  	update (IgTextRep *rep);
    virtual void  	update (IgRPhiRep *rep);
    virtual void  	update (IgRZRep *rep);
    virtual void        twigChanged (void);
  
private:
    std::string  	m_text;
    const std::string  	m_friendlyName;
    const std::string  	m_moduleLabel;
    const std::string  	m_instanceName;
    const std::string	m_processName;
    VisLegoScale 	m_scale;
    
    std::vector<std::pair<DetId, SiStripDigi> > m_digis;
    edm::ESHandle<TrackerGeometry> m_pDD;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_TRACKER_VIS_TRACKER_DIGI_TWIG_H
