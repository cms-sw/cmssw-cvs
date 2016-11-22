#ifndef VIS_TRACKER_RECO_VIS_TK_REC_TRACKS_TWIG_H
# define VIS_TRACKER_RECO_VIS_TK_REC_TRACKS_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "VisFramework/VisFrameworkBase/interface/VisSelectedTrackNumber.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

namespace reco 
{
    class Track;
}
class SbVec3f;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS


class VisTkRecTracksTwig : public VisQueuedTwig
{
public:
  
    VisTkRecTracksTwig (IgState *state, IgTwig *parent, 
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
    void	  twigChanged (void);
    
private:
    std::string  	m_text;
    const std::string  	m_friendlyName;
    const std::string  	m_moduleLabel;
    const std::string  	m_instanceName;
    const std::string 	m_processName;
    std::vector<std::string> m_trackCollectionNames;
    bool                m_haloFlag;
    double 		m_trackThickness;

    std::vector<reco::Track>  		m_tracks;
    std::vector<std::vector<SbVec3f> >  m_trackPoints;

    std::vector<std::vector<SbVec3f> >  m_trajectoryPositions;
    std::vector<std::vector<SbVec3f> >  m_trajectoryDirections;

    VisSelectedTrackNumber 			m_selectedTrack; 
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_TRACKER_RECO_VIS_TK_REC_TRACKS_TWIG_H
