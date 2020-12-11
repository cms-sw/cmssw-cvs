//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Fields/interface/VisG4MagFieldTwig.h"
#include "VisGeant4/VisG4Fields/interface/VisG4AnnotationsTwig.h"
#include "VisGeant4/VisG4Fields/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/xtypeinfo.h"
#include "Iguana/GLBrowsers/interface/Ig3DInitialiser.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/xtypeinfo.h"

#include "VisGeant4/VisG4Fields/interface/config.h"
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

XTYPEINFO_DEF_1 (VisG4MagFieldTwig,    IgSimpleTwig,   false);
XTYPEINFO_DEF_1 (VisG4AnnotationsTwig, IgSimpleTwig,   false);
