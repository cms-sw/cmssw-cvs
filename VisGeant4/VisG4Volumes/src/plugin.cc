//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4VolumeTwig.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4GeometryTwig.h"
#include "VisGeant4/VisG4Volumes/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/xtypeinfo.h"

#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>
#include <classlib/utils/XTypeInfo.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

XTYPEINFO_DEF_1 (VisG4VolumeTwig,      IgCompoundTwig, false);
XTYPEINFO_DEF_1 (VisG4GeometryTwig,    IgSimpleTwig,   false);
