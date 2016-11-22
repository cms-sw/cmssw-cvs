#include "VisReco/VisMuonCSC/interface/VisMuonCSCContent.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgModuleDef.h"

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisMuonCSCContent, VisMuonCSCContent::catalogLabel ());
