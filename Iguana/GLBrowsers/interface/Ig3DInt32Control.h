#ifndef IGUANA_GL_BROWSERS_IG_3DINT32_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DINT32_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeControlInt.h"
# include "Iguana/GLBrowsers/interface/Ig3DRangeControl.h"
# include <Inventor/fields/SoSFInt32.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DInt32Control : public Ig3DRangeControl<SoSFInt32, int, IgQtRangeControlInt>
{
    Q_OBJECT
public:
    Ig3DInt32Control (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (int newval);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DINT32_CONTROL_H
