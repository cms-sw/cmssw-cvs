#include "VisReco/VisTracker/interface/VisTkRecoContent.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgModuleDef.h"

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisTkRecoContent, VisTkRecoContent::catalogLabel ());
