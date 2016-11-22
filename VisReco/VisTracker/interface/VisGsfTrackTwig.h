#ifndef VIS_TRACKER_VIS_GSF_TRACK_TWIG_H
# define VIS_TRACKER_VIS_GSF_TRACK_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisReco/VisTracker/interface/VisGsfTrackDirection.h"
# include "VisReco/VisTracker/interface/VisGsfTrackShape.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

namespace reco 
{
    class GsfTrack;
}
class SbVec3f;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisGsfTrackTwig : public VisQueuedTwig
{
public:
    VisGsfTrackTwig (IgState *state, IgTwig *parent, 
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
    VisGsfTrackDirection m_showDirection;
    VisGsfTrackShape	m_shape;
    
    std::vector<reco::GsfTrack> m_tracks;
    std::vector<std::vector<SbVec3f> >  m_trajectoryPositions;
    std::vector<std::vector<SbVec3f> >  m_trajectoryDirections;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_TRACKER_VIS_GSF_TRACK_TWIG_H
