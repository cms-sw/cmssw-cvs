#include "VisSimulation/VisSimBase/interface/VisSimContent.h"
#include "VisSimulation/VisSimBase/interface/VisSimQtActions.h"
#include "VisSimulation/VisSimBase/interface/VisG4GeomContent.h"
#include "VisSimulation/VisSimBase/interface/VisG4MagFieldContent.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgModuleDef.h"

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisSimContent, VisSimContent::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisG4GeomContent, VisG4GeomContent::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisG4MagFieldContent, VisG4MagFieldContent::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisSimQtActions, VisSimQtActions::catalogLabel ());
