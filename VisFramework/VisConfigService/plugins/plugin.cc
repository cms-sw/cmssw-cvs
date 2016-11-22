//<<<<<< INCLUDES                                                       >>>>>>

#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

typedef edm::serviceregistry::ParameterSetMaker<VisConfigurationService> maker;

DEFINE_SEAL_MODULE ();
DEFINE_ANOTHER_FWK_SERVICE_MAKER (VisConfigurationService, maker);
