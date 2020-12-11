#ifndef IGUANA_GL_BROWSERS_IG_3D_SYSTEM_H
# define IGUANA_GL_BROWSERS_IG_3D_SYSTEM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include <classlib/utils/Callback.h>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DSystem
{
public:
    // implicit constructor
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static void			initAction (lat::Callback action);

    static void			init (IgState *state, QWidget *mainWindow);
    static QWidget *		init (IgState *state, int argc, char **argv,
			    	      const char *appname, const char *classname);

private:
    static void			doinit (IgState *state);

    static bool				s_initialised;
    static std::list<lat::Callback>	s_actions;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3D_SYSTEM_H
