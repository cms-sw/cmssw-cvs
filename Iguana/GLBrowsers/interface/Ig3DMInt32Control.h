#ifndef IGUANA_GL_BROWSERS_IG_3DMINT32_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DMINT32_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMControlInt.h"
# include "Iguana/GLBrowsers/interface/Ig3DRangeMControl.h"
# include <Inventor/fields/SoMFInt32.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IGUANA_GL_BROWSERS_API Ig3DMInt32Control : public Ig3DRangeMControl<SoMFInt32, int, IgQtRangeMControlInt>
{
    Q_OBJECT
public:
    Ig3DMInt32Control (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeMControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned int size, const int* newvals);
    void		setValue (const std::vector<int>& newvals);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DMINT32_CONTROL_H
