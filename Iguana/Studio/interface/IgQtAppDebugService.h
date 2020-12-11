#ifndef IGUANA_STUDIO_IG_QT_APP_DEBUG_SERVICE_H
# define IGUANA_STUDIO_IG_QT_APP_DEBUG_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgQtAppDebug;
class QWidget;
class QMainWindow;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtAppDebugService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppDebugService);
public:
    static const int	MENU_ID_DEBUG		= 9000;
    static const int	MENU_ID_DEBUG_RESOURCES	= 9001;
    static const int	MENU_ID_DEBUG_LIBRARIES	= 9002;
    static const int	MENU_ID_DEBUG_PLUGINS	= 9003;
    static const int	MENU_ID_DEBUG_DUMPSTATE	= 9004;
    static const int	MENU_ID_DEBUG_SESSION	= 9005;
    static const int	MENU_ID_DEBUG_POOLS	= 9006;
    static const int	MENU_ID_DEBUG_CRASH	= 9007;
    static const int	MENU_ID_DEBUG_DEBUGME	= 9008;
    static const int	MENU_ID_DEBUG_PROFILER_SEP = 9100;
    static const int	MENU_ID_DEBUG_PROFILER	= 9101;

    IgQtAppDebugService (IgState *state, QWidget *mainWindow);
    ~IgQtAppDebugService (void);

    // FIXME: Add menu item to show various debugging information.
    // Use ResourceInfo::info and TimeInfo::process_times.  Show
    // currently loaded services.

private:
    IgState			*m_state;
    QMainWindow			*m_mainWindow;
    IgQtAppDebug		*m_info;

    // undefined semantics
    IgQtAppDebugService (const IgQtAppDebugService &);
    IgQtAppDebugService &operator= (const IgQtAppDebugService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_APP_DEBUG_SERVICE_H
