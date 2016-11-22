#ifndef VIS_FRAMEWORK_BASE_VIS_CU_TK_TRACK_PRODUCER_H
# define VIS_FRAMEWORK_BASE_VIS_CU_TK_TRACK_PRODUCER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisActiveConfigurable.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCuTkTrackProducer : public VisActiveConfigurable<std::string>
{
public:
    VisCuTkTrackProducer (IgState *state, lat::Callback notify);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_CU_TK_TRACK_PRODUCER_H
