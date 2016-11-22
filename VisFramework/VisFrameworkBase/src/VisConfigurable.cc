//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisConfigurable.h"
#include "VisFramework/VisFrameworkBase/interface/VisConfiguration.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisConfigurable::~VisConfigurable (void)
{
    VisConfiguration *configuration = VisConfiguration::get (m_state);
    ASSERT (configuration);
    (*configuration).erase (this);
}

void 
VisConfigurable::baseInit (const std::string &iname) 
{ 
    LOG (0, trace, LFfwvis, "VisConfigurable[" << iname 
	 << "]::baseInit()\n");

    VisConfiguration *configuration = VisConfiguration::get (m_state);
    if (configuration == 0) 
    {
	configuration = new VisConfiguration (m_state);
    }
    
    ASSERT (configuration);
    (*configuration).insert (iname, this); 
}
