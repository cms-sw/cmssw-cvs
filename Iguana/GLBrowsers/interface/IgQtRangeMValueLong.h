#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_LONG_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_LONG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtRangeMValueLong : public IgQtRangeMValue<long>
{
public:
    IgQtRangeMValueLong (long value = 0,
                        long min = SHRT_MIN,
		        long max = SHRT_MAX);
    
    IgQtRangeMValueLong (unsigned int size,
                        const long* values,
                        long min = SHRT_MIN,
		        long max = SHRT_MAX);
    
    IgQtRangeMValueLong (const rangeMValues& values,
                        long min = SHRT_MIN,
		        long max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_LONG_H
