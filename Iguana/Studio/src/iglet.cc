#include "Iguana/Studio/interface/IgFileProxyDB.h"
#include "Iguana/Studio/interface/IgFileLocalProxy.h"
#include "Iguana/Studio/interface/IgStudioDriver.h"
#include "Iguana/Studio/interface/IgQtSite.h"
#include "Iguana/Studio/interface/IgQMainWindowSite.h"
#include "Iguana/Studio/interface/IgQSplitterSite.h"
#include "Iguana/Studio/interface/IgQHSplitterSite.h"
#include "Iguana/Studio/interface/IgQVSplitterSite.h"
#include "Iguana/Studio/interface/IgQtTextBrowser.h"
#include "Iguana/Studio/interface/IgQtTwigBrowser.h"
#include "Iguana/Studio/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgSiteDB.h"
#include "Iguana/Framework/interface/IgModuleDef.h"
#include "Iguana/Framework/interface/IgBrowserDB.h"
#include "Iguana/Framework/interface/IgDriverDB.h"
#include "Iguana/Framework/interface/IgCapabilityDB.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Framework/interface/xtypeinfo.h"

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgDriverDB, IgStudioDriver, "IGUANA");
DEFINE_IGUANA_PLUGIN (IgBrowserDB, IgQtTextBrowser, "Text");
DEFINE_IGUANA_PLUGIN (IgBrowserDB, IgQtTwigBrowser, "Twig");
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (IgQtTwigBrowser).name (),
			typeid (IgTwig).name ()));

DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQMainWindowSite, "QMainWindow");
DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQSplitterSite, "QSplitter");
DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQHSplitterSite, "QHSplitter");
DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQVSplitterSite, "QVSplitter");
DEFINE_IGUANA_PLUGIN (IgFileProxyDB, IgFileLocalProxy, IgFileLocalProxy::catalogLabel ());
