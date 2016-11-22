#ifndef VIS_FRAMEWORK_BASE_VIS_CU_TK_ACCESS_DB_CONNECTION_H
# define VIS_FRAMEWORK_BASE_VIS_VIS_CU_TK_ACCESS_DB_CONNECTION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisActiveConfigurable.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCuTkAccessDbConnection : public VisActiveConfigurable<bool>
{
public:
    VisCuTkAccessDbConnection (IgState *state, lat::Callback notify);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_VIS_CU_TK_ACCESS_DB_CONNECTION_H
