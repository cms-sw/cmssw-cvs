#ifndef IGUANA_GL_BROWSERS_IG_3DVECTOR_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DVECTOR_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/Ig3DVectorBaseControl.h"
# include <Inventor/SbLinear.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DVectorControl : public Ig3DVectorBaseControl
{
    Q_OBJECT

public:
    Ig3DVectorControl (IgControlCategory	*pane,
		       QString			label,
		       Style			style);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    // Control value and change access
    float		xValue (void) const;
    float		yValue (void) const;
    float		zValue (void) const;
    SbVec3f		value (void) const;

    float		xChange (void) const;
    float		yChange (void) const;
    float		zChange (void) const;
    SbVec3f		change (void) const;

    // Access to the x,y,z control
    IgQtRangeControlFloat *	xControl (void) const;
    IgQtRangeControlFloat *	yControl (void) const;
    IgQtRangeControlFloat *	zControl (void) const;
    IgQtRangeControlFloat *	control  (void) const;
    
    using Ig3DVectorBaseControl::setValue;
    using Ig3DVectorBaseControl::value;
    using Ig3DVectorBaseControl::change;
    using Ig3DVectorBaseControl::control;

public slots:
    virtual void	setValue (const SbVec3f &vec);
    virtual void	setValue (float x, float y, float z);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DVECTOR_CONTROL_H
