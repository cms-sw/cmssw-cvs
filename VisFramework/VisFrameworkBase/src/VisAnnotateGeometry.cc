//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisAnnotateGeometry.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisAnnotateGeometry::VisAnnotateGeometry (IgState *state, lat::Callback notify)
    : VisActiveConfigurable<bool> (state, notify, false, "VisAnnotateGeometry")
{}
