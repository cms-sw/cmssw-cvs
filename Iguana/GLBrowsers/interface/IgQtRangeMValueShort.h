#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_SHORT_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_SHORT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtRangeMValueShort : public IgQtRangeMValue<short>
{
public:
    IgQtRangeMValueShort (short value = 0,
                        short min = SHRT_MIN,
		        short max = SHRT_MAX);
    
    IgQtRangeMValueShort (unsigned int size,
                        const short* values,
                        short min = SHRT_MIN,
		        short max = SHRT_MAX);
    
    IgQtRangeMValueShort (const rangeMValues& values,
                        short min = SHRT_MIN,
		        short max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_SHORT_H
