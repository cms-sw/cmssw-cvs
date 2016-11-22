#ifndef IGUANA_GL_BROWSERS_IG_3DANGLE_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DANGLE_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/Ig3DFloatControl.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DAngleControl : public Ig3DFloatControl
{
    Q_OBJECT
public:
    Ig3DAngleControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
    virtual void	updateValue (void);
    virtual void	applyValue (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DANGLE_CONTROL_H
