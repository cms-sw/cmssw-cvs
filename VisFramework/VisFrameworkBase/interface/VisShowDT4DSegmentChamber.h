#ifndef VIS_FRAMEWORK_BASE_VIS_SHOW_DT4DSEGMENT_CHAMBER_H
# define VIS_FRAMEWORK_BASE_VIS_SHOW_DT4DSEGMENT_CHAMBER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisActiveConfigurable.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisShowDT4DSegmentChamber : public VisActiveConfigurable<bool>
{
public:
    VisShowDT4DSegmentChamber (IgState *state, lat::Callback notify);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_SHOW_DT4DSEGMENT_CHAMBER_H
