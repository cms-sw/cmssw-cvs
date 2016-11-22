#include "VisReco/VisTrigger/interface/VisTriggerContent.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgModuleDef.h"

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisTriggerContent, VisTriggerContent::catalogLabel ());
