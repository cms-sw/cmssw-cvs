#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_VALUE_PRECISION_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_VALUE_PRECISION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtRangeValuePrecision
{
public:
    IgQtRangeValuePrecision (float precision = 0.001);
    
    virtual float	precision (void) const;
    virtual void	setPrecision (float precision);
    
    static float 	minimumPrecision (void);

private:
    float		m_precision;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_VALUE_PRECISION_H
