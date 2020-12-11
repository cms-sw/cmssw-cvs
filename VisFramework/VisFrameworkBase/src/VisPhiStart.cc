//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisPhiStart.h"
#include <math.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisPhiStart::VisPhiStart (IgState *state, lat::Callback notify)
    : VisActiveConfigurable<double> (state, notify, M_PI / 2.0, "VisPhiStart")
{
}
