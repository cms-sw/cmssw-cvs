//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>

edm::ParameterSet* 	VisConfigurationService::m_pSet = 0;

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisConfigurationService::VisConfigurationService (const edm::ParameterSet& pSet)
{
    if(!m_pSet)  m_pSet = new edm::ParameterSet(pSet);
}

VisConfigurationService::~VisConfigurationService ()
{ }

const edm::ParameterSet*
VisConfigurationService::pSet ()
{ return m_pSet; }
