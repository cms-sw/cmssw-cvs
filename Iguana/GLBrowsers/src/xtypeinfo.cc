//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/Ig3DBaseBrowser.h"
#include "Iguana/GLBrowsers/interface/Ig2DBrowser.h"
#include "Iguana/GLBrowsers/interface/Ig3DBrowser.h"
#include "Iguana/GLBrowsers/interface/IgLegoBrowser.h"
#include "Iguana/GLBrowsers/interface/IgRPhiBrowser.h"
#include "Iguana/GLBrowsers/interface/IgRZBrowser.h"
#include "Iguana/GLBrowsers/interface/IgSpareBrowser.h"
#include "Iguana/GLBrowsers/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/xtypeinfo.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

XTYPEINFO_DEF_1 (Ig3DBaseBrowser, IgBrowser, false); // SoQtExaminerViewer,false
XTYPEINFO_DEF_1 (Ig2DBrowser, Ig3DBaseBrowser, false);
XTYPEINFO_DEF_1 (Ig3DBrowser, Ig3DBaseBrowser, false);
XTYPEINFO_DEF_1 (IgLegoBrowser, Ig3DBaseBrowser, false);
XTYPEINFO_DEF_1 (IgRPhiBrowser, Ig3DBaseBrowser, false);
XTYPEINFO_DEF_1 (IgRZBrowser, Ig3DBaseBrowser, false);
XTYPEINFO_DEF_1 (IgSpareBrowser, Ig3DBaseBrowser, false);
