//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisSiStripClusterPointSize.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisSiStripClusterPointSize::VisSiStripClusterPointSize (IgState *state, lat::Callback notify)
    : VisActiveConfigurable<unsigned int> (state, notify, 4, "VisSiStripClusterPointSize")
{
}
