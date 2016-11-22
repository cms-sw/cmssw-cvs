#ifndef VIS_FRAMEWORK_BASE_VIS_CU_TK_STRIP_DIGI_LABEL_H
# define VIS_FRAMEWORK_BASE_VIS_CU_TK_STRIP_DIGI_LABEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisActiveConfigurable.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCuTkStripDigiLabel : public VisActiveConfigurable<std::string>
{
public:
    VisCuTkStripDigiLabel (IgState *state, lat::Callback notify);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_CU_TK_STRIP_DIGI_LABEL_H
