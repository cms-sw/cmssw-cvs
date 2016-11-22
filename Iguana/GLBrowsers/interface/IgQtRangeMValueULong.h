#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_ULONG_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_ULONG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtRangeMValueULong : public IgQtRangeMValue<unsigned long>
{
public:
    IgQtRangeMValueULong (unsigned long value = 0,
                        unsigned long min = 0,
		        unsigned long max = SHRT_MAX);
    
    IgQtRangeMValueULong (unsigned int size,
                        const unsigned long* values,
                        unsigned long min = 0,
		        unsigned long max = SHRT_MAX);
    
    IgQtRangeMValueULong (const rangeMValues& values,
                        unsigned long min = 0,
		        unsigned long max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_ULONG_H
