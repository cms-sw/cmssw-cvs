#ifndef VIS_CONFIG_SERVICE_VIS_CONFIGURATION_SERVICE_H
# define VIS_CONFIG_SERVICE_VIS_CONFIGURATION_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisConfigurationService
{ 
public:
    VisConfigurationService (const edm::ParameterSet& pSet);
    ~VisConfigurationService ();
    
    static const edm::ParameterSet* 	pSet ();
    
private:
    static edm::ParameterSet*		m_pSet;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_CONFIG_SERVICE_VIS_CONFIGURATION_SERVICE_H
