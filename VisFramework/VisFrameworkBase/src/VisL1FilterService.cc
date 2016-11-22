//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisL1FilterService.h"
#include <qstring.h>
#include <stdexcept>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (VisL1FilterService, "Services/Framework/L1Filter");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisL1FilterService::VisL1FilterService (IgState *state)
    : m_state (state),
      m_init (false)
{
    ASSERT (state);
    state->put (s_key, this);
}

bool
VisL1FilterService::registerAlgo (std::string name, std::string key) 
{
    return m_algos.insert (VisL1TriggerMap::value_type (name, key)).second;
}

bool
VisL1FilterService::unregisterAlgo (const std::string &name) 
{
    return m_algos.erase (name) == 1;
}

void
VisL1FilterService::algorithm (std::string name, std::string key) 
{
    VisL1TriggerMap::iterator i = m_algos.find (name);
    
    if (i == m_algos.end ())
    {
	throw std::runtime_error (QString ("VisL1FilterService: Unknown L1 Algorithm name: %1").arg (QString::fromStdString (name)).latin1 ());
    }
    i->second = key;
}

VisL1FilterService::~VisL1FilterService (void)
{
    ASSERT (m_state);
    m_state->detach (s_key);
}
