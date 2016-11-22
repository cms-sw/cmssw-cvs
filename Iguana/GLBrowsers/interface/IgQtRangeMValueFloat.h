#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_FLOAT_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_MVALUE_FLOAT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMValue.h"
# include "Iguana/GLBrowsers/interface/IgQtRangeValuePrecision.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtRangeMValueFloat : public IgQtRangeMValue<float>,
						    public IgQtRangeValuePrecision
{
public:
    IgQtRangeMValueFloat (float value = 0.,
                          float min = SHRT_MIN,
		          float max = SHRT_MAX);
    
    IgQtRangeMValueFloat (unsigned int size,
                          const float* values,
                          float min = SHRT_MIN,
		          float max = SHRT_MAX);
    
    IgQtRangeMValueFloat (const rangeMValues& values,
                          float min = SHRT_MIN,
		          float max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    using inherited::setValue;
    virtual void	setValue (unsigned int index, float value);
    
    using inherited::isValueChanged;
    virtual bool	isValueChanged (unsigned int index) const;

    float		roundOff (float number, float precision = .001);

protected:
    using inherited::forceValueChanged;
    void   		forceValueChanged (float delta);
    void   		forceValueChanged (unsigned int index, float delta);
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_VALUE_FLOAT_H
