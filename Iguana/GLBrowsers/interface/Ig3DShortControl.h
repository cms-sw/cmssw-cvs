#ifndef IGUANA_GL_BROWSERS_IG_3DSHORT_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DSHORT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeControlShort.h"
# include "Iguana/GLBrowsers/interface/Ig3DRangeControl.h"
# include <Inventor/fields/SoSFShort.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IGUANA_GL_BROWSERS_API Ig3DShortControl : public Ig3DRangeControl<SoSFShort, short, IgQtRangeControlShort>
{
    Q_OBJECT
public:
    Ig3DShortControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (short newval);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DSHORT_CONTROL_H
