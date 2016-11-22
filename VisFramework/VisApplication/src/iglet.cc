//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisApplication/src/VisApplicationMain.h"
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
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisApplicationMain, VisApplicationMain::catalogLabel ());
