//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisCoreContent.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventMenu.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgModuleDef.h"

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisCoreContent, VisCoreContent::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisEventMenu, VisEventMenu::catalogLabel ());
