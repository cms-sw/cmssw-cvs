#ifndef VIS_FRAMEWORK_BASE_VIS_SELECTED_TRACK_NUMBER_H
# define VIS_FRAMEWORK_BASE_VIS_SELECTED_TRACK_NUMBER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisActiveConfigurable.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisSelectedTrackNumber : public VisActiveConfigurable<int>
{
public:
    VisSelectedTrackNumber (IgState *state, lat::Callback notify);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_SELECTED_TRACK_NUMBER_H
