#ifndef IGUANA_STUDIO_IG_QT_APP_WINDOW_SERVICE_H
# define IGUANA_STUDIO_IG_QT_APP_WINDOW_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtAppWindowService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppWindowService);
public:
    IgQtAppWindowService (IgState *state, QWidget *mainWindow);
    ~IgQtAppWindowService (void);

    virtual QWidget *		mainWindow (void);

private:
    IgState			*m_state;
    QWidget			*m_mainWindow;

    // undefined semantics
    IgQtAppWindowService (const IgQtAppWindowService &);
    IgQtAppWindowService &operator= (const IgQtAppWindowService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_APP_WINDOW_SERVICE_H
