#ifndef IGUANA_GL_BROWSERS_IG_3DMSHORT_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DMSHORT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMControlShort.h"
# include "Iguana/GLBrowsers/interface/Ig3DRangeMControl.h"
# include <Inventor/fields/SoMFShort.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IGUANA_GL_BROWSERS_API Ig3DMShortControl : public Ig3DRangeMControl<SoMFShort, short, IgQtRangeMControlShort>
{
    Q_OBJECT
public:
    Ig3DMShortControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeMControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned int size, const short* newvals);
    void		setValue (const std::vector<short>& newvals);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DMSHORT_CONTROL_H
