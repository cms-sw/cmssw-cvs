#ifndef IGUANA_STUDIO_IG_QT_APP_CRASH_ALERT_SERVICE_H
# define IGUANA_STUDIO_IG_QT_APP_CRASH_ALERT_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <classlib/utils/Callback.h>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtAppCrashAlertService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppCrashAlertService);
public:
    IgQtAppCrashAlertService (IgState *state, QWidget *mainWindow);
    ~IgQtAppCrashAlertService (void);

    virtual bool		hasCrashed (void);
    virtual void		addUnwind (lat::Callback action);
    virtual void		removeUnwind (lat::Callback action);

private:
    IgState			*m_state;
    QWidget			*m_mainWindow;
    QWidget			*m_popup;
    std::list<lat::Callback>	m_unwinds;

    // undefined semantics
    IgQtAppCrashAlertService (const IgQtAppCrashAlertService &);
    IgQtAppCrashAlertService &operator= (const IgQtAppCrashAlertService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_APP_CRASH_ALERT_SERVICE_H
