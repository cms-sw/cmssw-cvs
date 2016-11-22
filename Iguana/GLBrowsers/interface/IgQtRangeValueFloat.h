#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_VALUE_FLOAT_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_VALUE_FLOAT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeValue.h"
# include "Iguana/GLBrowsers/interface/IgQtRangeValuePrecision.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtRangeValueFloat : public IgQtRangeValue<float>,
						  public IgQtRangeValuePrecision
{
public:
    IgQtRangeValueFloat (float value = 0,
		         float min = SHRT_MIN,
		         float max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    virtual void	setValue (float value);
    virtual bool	isValueChanged (void) const;

    float		roundOff (float number, float precision = .001);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_VALUE_FLOAT_H
