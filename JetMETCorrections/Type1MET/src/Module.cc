
#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "JetMETCorrections/Type1MET/interface/Type1MET.h"
#include "JetMETCorrections/Type1MET/interface/MuonMET.h"
#include "JetMETCorrections/Type1MET/interface/TauMET.h"
#include "JetMETCorrections/Type1MET/interface/MuonMETValueMapProducer.h"
#include "JetMETCorrections/Type1MET/interface/MuonTCMETValueMapProducer.h"

using cms::Type1MET;
using cms::MuonMET;
using cms::TauMET;
using cms::MuonMETValueMapProducer;
using cms::MuonTCMETValueMapProducer;



DEFINE_FWK_MODULE(Type1MET);
DEFINE_FWK_MODULE(MuonMET);
DEFINE_FWK_MODULE(TauMET);
DEFINE_FWK_MODULE(MuonMETValueMapProducer);
DEFINE_FWK_MODULE(MuonTCMETValueMapProducer);
