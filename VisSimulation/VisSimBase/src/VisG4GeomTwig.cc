//<<<<<< INCLUDES                                                       >>>>>>

#include "VisSimulation/VisSimBase/interface/VisG4GeomTwig.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisGeant4/VisG4Core/interface/VisG4VisManager.h"
#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4GeomTwig::VisG4GeomTwig (IgState *state, IgTwig *parent)
    : VisG4GeometryTwig (VisG4VisManager::get (state), parent)
{}
