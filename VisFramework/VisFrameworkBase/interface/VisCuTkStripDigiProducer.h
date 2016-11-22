#ifndef VIS_FRAMEWORK_BASE_VIS_CU_TK_STRIP_DIGI_PRODUCER_H
# define VIS_FRAMEWORK_BASE_VIS_CU_TK_STRIP_DIGI_PRODUCER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisActiveConfigurable.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCuTkStripDigiProducer : public VisActiveConfigurable<std::string>
{
public:
    VisCuTkStripDigiProducer (IgState *state, lat::Callback notify);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_CU_TK_STRIP_DIGI_PRODUCER_H
