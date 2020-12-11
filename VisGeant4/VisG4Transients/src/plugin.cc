//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Transients/interface/VisG4TrajectoriesTwig.h"
#include "VisGeant4/VisG4Transients/interface/VisG4TransientsTwig.h"
#include "VisGeant4/VisG4Transients/interface/xtypeinfo.h"
#include "VisGeant4/VisG4Transients/interface/VisSoG4Trajectories.h"
#include "VisGeant4/VisG4Transients/interface/VisSoG4Trajectories.h"
#include "Iguana/GLBrowsers/interface/Ig3DInitialiser.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
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

XTYPEINFO_DEF_1 (VisG4TrajectoriesTwig,IgSimpleTwig,   false);
XTYPEINFO_DEF_1 (VisG4TransientsTwig,  IgSimpleTwig,   false);

static Ig3DInitialiser initG4T  (lat::CreateCallback (&VisSoG4Trajectories::initClass));
