//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisCustomTracker/interface/VisCuTkGeometryTwig.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkRecoContent.h"
#include "VisReco/VisCustomTracker/interface/xtypeinfo.h"
#include "VisFramework/VisFrameworkBase/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgModuleDef.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisCuTkRecoContent, VisCuTkRecoContent::catalogLabel ());
