#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/eventsetupdata_registration_macro.h"

#include "Validation/RecoEgamma/interface/EgammaObjects.h"
#include "Validation/RecoEgamma/interface/PhotonValidator.h"
#include "Validation/RecoEgamma/interface/PhotonPostprocessing.h"
#include "Validation/RecoEgamma/interface/ElectronMcSignalValidator.h"
#include "Validation/RecoEgamma/interface/ElectronMcFakeValidator.h"

DEFINE_SEAL_MODULE();

DEFINE_ANOTHER_FWK_MODULE(EgammaObjects);
DEFINE_ANOTHER_FWK_MODULE(PhotonValidator);
DEFINE_ANOTHER_FWK_MODULE(PhotonPostprocessing);
DEFINE_ANOTHER_FWK_MODULE(ElectronMcSignalValidator);
DEFINE_ANOTHER_FWK_MODULE(ElectronMcFakeValidator);

