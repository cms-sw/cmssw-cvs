#ifndef VIS_SIM_HITS_VIS_PSIM_HIT_TWIG_H
# define VIS_SIM_HITS_VIS_PSIM_HIT_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisSimHitConnect.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class PSimHit;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisPSimHitTwig : public VisQueuedTwig
{
public:
    VisPSimHitTwig (IgState *state, IgTwig *parent, const std::string &name = "",
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
    virtual void        	twigChanged (void);

private:
    std::string			m_text;
    std::string			m_friendlyName;
    std::string			m_moduleLabel;
    std::string			m_instanceName;
    std::string 		m_processName;
    VisSimHitConnect		m_connect;
    std::vector<PSimHit>      	m_hits;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_SIM_HITS_VIS_PSIM_HIT_TWIG_H
