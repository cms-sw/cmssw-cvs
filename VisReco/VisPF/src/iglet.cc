#include "VisReco/VisPF/interface/VisPFContent.h"

#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgModuleDef.h"

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisPFContent, VisPFContent::catalogLabel ());
