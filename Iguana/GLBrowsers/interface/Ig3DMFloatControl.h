#ifndef IGUANA_GL_BROWSERS_IG_3DMFLOAT_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DMFLOAT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMControlFloat.h"
# include "Iguana/GLBrowsers/interface/Ig3DRangeMControl.h"
# include <Inventor/fields/SoMFFloat.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IGUANA_GL_BROWSERS_API Ig3DMFloatControl : public Ig3DRangeMControl<SoMFFloat, float, IgQtRangeMControlFloat>
{
    Q_OBJECT
public:
    Ig3DMFloatControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeMControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned int size, const float* newvals);
    void		setValue (const std::vector<float>& newvals);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DMFLOAT_CONTROL_H
