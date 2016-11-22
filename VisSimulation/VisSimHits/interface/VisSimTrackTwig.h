#ifndef VIS_SIM_HITS_VIS_SIM_TRACK_TWIG_H
# define VIS_SIM_HITS_VIS_SIM_TRACK_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisPtCut.h"
# include "VisFramework/VisFrameworkBase/interface/VisSimTrackAnnotation.h"
# include "VisFramework/VisFrameworkBase/interface/VisSimTrackTime.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SimTrack;
class SimVertex;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisSimTrackTwig : public VisQueuedTwig
{
public:
    VisSimTrackTwig (IgState *state, IgTwig *parent, const std::string &name = "",
		     const std::string &friendlyName = "",
		     const std::string &moduleLabel = "",
		     const std::string &instanceName = "",
		     const std::string &processName = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void	       	onNewEvent (const edm::Event &event,
					    const edm::EventSetup &eventSetup);

    using VisQueuedTwig::update;
    virtual void        	update (IgTextRep *rep);
    virtual void       		update (Ig3DRep *rep);
    virtual void       		update (IgLegoRep *rep);
    virtual void       		update (IgRPhiRep *rep);
    virtual void       		update (IgRZRep *rep);
    virtual void        	cutChanged (void);

private:
    std::string			m_text;
    std::string			m_friendlyName;
    std::string			m_moduleLabel;
    std::string			m_instanceName;
    std::string 		m_processName;
    std::vector<SimTrack>      	m_tracks;
    std::vector<SimVertex>      m_vertices;
    VisPtCut			m_cut;
    VisSimTrackAnnotation	m_annot;
    VisSimTrackTime		m_time;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_SIM_HITS_VIS_SIM_TRACK_TWIG_H
