#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "VisReco/VisMuonCSC/interface/VisCSCConfigurationService.h"

typedef edm::serviceregistry::ParameterSetMaker<VisCSCConfigurationService> maker;

DEFINE_SEAL_MODULE ();
DEFINE_ANOTHER_FWK_SERVICE_MAKER (VisCSCConfigurationService, maker);
