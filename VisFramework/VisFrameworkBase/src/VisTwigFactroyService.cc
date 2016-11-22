//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include <stdexcept>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (VisTwigFactroyService, "Services/Framework/Twig Factory");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisTwigFactroyService::VisTwigFactroyService (IgState *state)
{    
    ASSERT (state);
    state->put (s_key, this);
}

VisTwigFactroyService::~VisTwigFactroyService (void) 
{    
    ASSERT (m_state);
    m_state->detach (s_key);
}

bool
VisTwigFactroyService::registerTwig (const std::string &name, CreateTwigCallback createFn) 
{
    return m_callback.insert (CallbackMap::value_type (name, createFn)).second;
}

bool
VisTwigFactroyService::unregisterTwig (const std::string &name) 
{
    return m_callback.erase (name) == 1;
}

VisQueuedTwig *
VisTwigFactroyService::createTwig (const std::string &name, 
				   IgState *state, IgTwig *parent,
				   const std::string &friendlyName,
				   const std::string &moduleLabel,
				   const std::string &instanceName,
				   const std::string &processName) 
{
    CallbackMap::const_iterator i = m_callback.find (name);
    
    if (i == m_callback.end ())
    {
	throw std::runtime_error ("Unknown Twig name.");
    }
    std::string twigName = friendlyName
			   + std::string (":") + moduleLabel
			   + std::string (":") + instanceName 
			   + std::string (":") + processName;
    
    return (i->second)(state, parent, twigName, 
		       friendlyName, moduleLabel, instanceName, processName);
}
