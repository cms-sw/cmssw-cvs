//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisEventSetup/interface/xtypeinfo.h"
#include "VisFramework/VisEventSetup/interface/VisCaloGeometryTwig.h"
#include "VisFramework/VisEventSetup/interface/VisCSCGeometryTwig.h"
#include "VisFramework/VisEventSetup/interface/VisDTGeometryTwig.h"
#include "VisFramework/VisEventSetup/interface/VisRPCGeometryTwig.h"
#include "VisFramework/VisEventSetup/interface/VisTrackerGeometryTwig.h"
#include "VisFramework/VisFrameworkBase/interface/xtypeinfo.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

XTYPEINFO_DEF_1 (VisCaloGeometryTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisCSCGeometryTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisDTGeometryTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisRPCGeometryTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisTrackerGeometryTwig, VisQueuedTwig, false);
