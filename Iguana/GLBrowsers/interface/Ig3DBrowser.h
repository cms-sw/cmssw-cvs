#ifndef IGUANA_GL_BROWSERS_IG_3D_BROWSER_H
# define IGUANA_GL_BROWSERS_IG_3D_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/GLBrowsers/interface/Ig3DBaseBrowser.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class Ig3DModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DBrowser : public Ig3DBaseBrowser
{
public:
    Ig3DBrowser (IgState *state, IgSite *site, Ig3DModel *model = 0);

    static const char *		catalogLabel (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3D_BROWSER_H
