#ifndef IGUANA_GL_BROWSERS_IG_3DCAMERA_POS_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DCAMERA_POS_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/Ig3DVec3fControl.h"
# include "Iguana/GLBrowsers/interface/Ig3DCameraUtils.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DCameraPosControl
    : public Ig3DVec3fControl, public Ig3DCameraUtils
{
public:
    Ig3DCameraPosControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DCAMERA_POS_CONTROL_H
