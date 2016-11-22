#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_USHORT_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_USHORT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtRangeMValueUShort : public IgQtRangeMValue<unsigned short>
{
public:
    IgQtRangeMValueUShort (unsigned short value = 0,
                        unsigned short min = 0,
		        unsigned short max = SHRT_MAX);
    
    IgQtRangeMValueUShort (unsigned int size,
                        const unsigned short* values,
                        unsigned short min = 0,
		        unsigned short max = SHRT_MAX);
    
    IgQtRangeMValueUShort (const rangeMValues& values,
                        unsigned short min = 0,
		        unsigned short max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_USHORT_H
