#ifndef IGUANA_GL_BROWSERS_IG_3DMUINT32_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DMUINT32_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMControlUInt.h"
# include "Iguana/GLBrowsers/interface/Ig3DRangeMControl.h"
# include <Inventor/fields/SoMFUInt32.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IGUANA_GL_BROWSERS_API Ig3DMUInt32Control : public Ig3DRangeMControl<SoMFUInt32, unsigned int, IgQtRangeMControlUInt>
{
    Q_OBJECT
public:
    Ig3DMUInt32Control (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeMControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned int size, const unsigned int* newvals);
    void		setValue (const std::vector<unsigned int>& newvals);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DMUINT32_CONTROL_H
