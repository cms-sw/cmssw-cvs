#ifndef VIS_TRACKER_VIS_TRACKING_REC_HIT_TWIG_H
# define VIS_TRACKER_VIS_TRACKING_REC_HIT_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SbVec3f;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisTrackingRecHitTwig : public VisQueuedTwig
{
public:
    VisTrackingRecHitTwig (IgState *state, IgTwig *parent, 
			   const std::string &name = "",
			   const std::string &friendlyName = "",
			   const std::string &moduleLabel = "",
			   const std::string &instanceName = "",
			   const std::string &processName = "");

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void  onNewEvent (const edm::Event &event,
			      const edm::EventSetup &eventSetup);

    using VisQueuedTwig::update;
    virtual void  update (Ig3DRep *rep);
    virtual void  update (IgLegoRep *rep);
    virtual void  update (IgTextRep *rep);
    virtual void  update (IgRPhiRep *rep);
    virtual void  update (IgRZRep *rep);
    virtual void  twigChanged (void);

private:
    std::string  	m_text;
    const std::string  	m_friendlyName;
    const std::string  	m_moduleLabel;
    const std::string  	m_instanceName;
    const std::string 	m_processName;
    
    std::vector<SbVec3f> m_recHits;
    edm::ESHandle<GlobalTrackingGeometry> m_trackingGeometry;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_TRACKER_VIS_TRACKING_REC_HIT_TWIG_H
