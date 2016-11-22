#ifndef IGUANA_GL_BROWSERS_IG_3DUSHORT_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DUSHORT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeControlUShort.h"
# include "Iguana/GLBrowsers/interface/Ig3DRangeControl.h"
# include <Inventor/fields/SoSFUShort.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IGUANA_GL_BROWSERS_API Ig3DUShortControl : public Ig3DRangeControl<SoSFUShort, unsigned short, IgQtRangeControlUShort>
{
    Q_OBJECT
public:
    Ig3DUShortControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned short newval);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DUSHORT_CONTROL_H
