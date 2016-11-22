#ifndef VIS_TRIGGER_VIS_L1MU_DTTRACK_CONTAINER_TWIG_H
# define VIS_TRIGGER_VIS_L1MU_DTTRACK_CONTAINER_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackContainer.h"
# include <qstring.h>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisL1MuDTTrackContainerTwig : public VisQueuedTwig
{
public:
    VisL1MuDTTrackContainerTwig (IgState *state, IgTwig *parent, 
				 const std::string &name,
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
    virtual void       	update (Ig3DRep *rep);
    virtual void       	update (IgTextRep *rep);

private:    
    IgState             *m_state;
    std::string	       	m_text;
    std::string	 	m_friendlyName;
    std::string	 	m_moduleLabel;
    std::string	 	m_instanceName;
    std::string	 	m_processName;
    std::vector<L1MuDTTrackCand>  *m_container;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_TRIGGER_VIS_L1MU_DTTRACK_CONTAINER_TWIG_H
