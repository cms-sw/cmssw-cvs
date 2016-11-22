#ifndef IGUANA_GL_BROWSERS_IG_3DCAMERA_ROT_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DCAMERA_ROT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/Ig3DRotationControl.h"
# include "Iguana/GLBrowsers/interface/Ig3DCameraUtils.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DCameraRotControl
    : public Ig3DRotationControl, public Ig3DCameraUtils
{
public:
    Ig3DCameraRotControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
    virtual void	applyValue (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DCAMERA_ROT_CONTROL_H
