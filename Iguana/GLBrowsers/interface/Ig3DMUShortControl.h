#ifndef IGUANA_GL_BROWSERS_IG_3DMUSHORT_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DMUSHORT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMControlUShort.h"
# include "Iguana/GLBrowsers/interface/Ig3DRangeMControl.h"
# include <Inventor/fields/SoMFUShort.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IGUANA_GL_BROWSERS_API Ig3DMUShortControl : public Ig3DRangeMControl<SoMFUShort, unsigned short, IgQtRangeMControlUShort>
{
    Q_OBJECT
public:
    Ig3DMUShortControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeMControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned int size, const unsigned short* newvals);
    void		setValue (const std::vector<unsigned short>& newvals);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DMUSHORT_CONTROL_H
