#ifndef IGUANA_GL_BROWSERS_IG_3DFLOAT_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DFLOAT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeControlFloat.h"
# include "Iguana/GLBrowsers/interface/Ig3DRangeControl.h"
# include <Inventor/fields/SoSFFloat.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IGUANA_GL_BROWSERS_API Ig3DFloatControl : public Ig3DRangeControl<SoSFFloat, float, IgQtRangeControlFloat>
{
    Q_OBJECT
public:
    Ig3DFloatControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (float newval);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DFLOAT_CONTROL_H
