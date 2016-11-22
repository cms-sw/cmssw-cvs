//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/Models/interface/IgSimpleTextRep.h"
#include "Iguana/Models/interface/IgTextModel.h"
#include "Iguana/Models/interface/IgSimpleTextModel.h"
#include "Iguana/Models/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Framework/interface/xtypeinfo.h"
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

XTYPEINFO_DEF_1 (IgTextRep, IgRep, false);
XTYPEINFO_DEF_1 (IgSimpleTextRep, IgTextRep, false);
XTYPEINFO_DEF_1 (IgTextModel, IgModel, false);
XTYPEINFO_DEF_1 (IgSimpleTextModel, IgTextModel, false);

lat::logflag LFcommonModels = { 0, "commonModels", true, -1 };
