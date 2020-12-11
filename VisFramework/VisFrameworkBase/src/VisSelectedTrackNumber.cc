//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisSelectedTrackNumber.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisSelectedTrackNumber::VisSelectedTrackNumber (IgState *state, lat::Callback notify)
    : VisActiveConfigurable<int> (state, notify, -1 , "VisSelectedTrackNumber")
{}
