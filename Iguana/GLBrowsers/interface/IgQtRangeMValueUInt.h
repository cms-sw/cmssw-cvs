#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_UINT_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_UINT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtRangeMValueUInt : public IgQtRangeMValue<unsigned int>
{
public:
    IgQtRangeMValueUInt (unsigned int value = 0,
                        unsigned int min = 0,
		        unsigned int max = SHRT_MAX);
    
    IgQtRangeMValueUInt (unsigned int size,
                        const unsigned int* values,
                        unsigned int min = 0,
		        unsigned int max = SHRT_MAX);
    
    IgQtRangeMValueUInt (const rangeMValues& values,
                        unsigned int min = 0,
		        unsigned int max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_UINT_H
