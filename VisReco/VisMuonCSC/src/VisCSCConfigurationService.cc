//<<<<<< INCLUDES                                                        >>>>>>

#include "VisReco/VisMuonCSC/interface/VisCSCConfigurationService.h"

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                    >>>>>>
edm::ParameterSet* VisCSCConfigurationService::m_pSet = 0;

//<<<<<< MEMBER FUNCTION DEFINITIONS                                     >>>>>> 

VisCSCConfigurationService::VisCSCConfigurationService (const edm::ParameterSet& pSet)
{
    if (!m_pSet) m_pSet = new edm::ParameterSet(pSet);
}

VisCSCConfigurationService::~VisCSCConfigurationService () {}

const edm::ParameterSet* VisCSCConfigurationService::pSet ()
{
    return m_pSet;
}
