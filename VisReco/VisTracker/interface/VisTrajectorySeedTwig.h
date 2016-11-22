#ifndef VIS_TRACKER_RECO_VIS_TK_TRAJECTORYSEED_TWIG_H
# define VIS_TRACKER_RECO_VIS_TK_TRAJECTORYSEED_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisTrajectorySeedScale.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
# include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
# include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
# include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SbVec3f;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS


class VisTrajectorySeedTwig : public VisQueuedTwig
{
public:
  
    VisTrajectorySeedTwig (IgState *state, IgTwig *parent, 
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
    VisTrajectorySeedScale 	m_seedRescaleFactor;
    std::vector<std::vector<SbVec3f> >  m_hitPoints;
    std::vector<SbVec3f>  m_seedPositions;
    std::vector<SbVec3f>  m_seedDirections;

    edm::ESHandle<GlobalTrackingGeometry> 	m_trackerGeometry;
    edm::ESHandle<MagneticField> 		m_mgField;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_TRACKER_RECO_VIS_TK_REC_TRACKS_TWIG_H
