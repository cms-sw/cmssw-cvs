#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_INT_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_INT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtRangeMValueInt : public IgQtRangeMValue<int>
{
public:
    IgQtRangeMValueInt (int value = 0,
                        int min = SHRT_MIN,
		        int max = SHRT_MAX);
    
    IgQtRangeMValueInt (unsigned int size,
                        const int* values,
                        int min = SHRT_MIN,
		        int max = SHRT_MAX);
    
    IgQtRangeMValueInt (const rangeMValues& values,
                        int min = SHRT_MIN,
		        int max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_INT_H
