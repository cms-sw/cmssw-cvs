#ifndef IGUANA_STUDIO_IG_QT_APP_STATUS_BAR_SERVICE_H
# define IGUANA_STUDIO_IG_QT_APP_STATUS_BAR_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;
class QMainWindow;
class QString;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtAppStatusBarService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppStatusBarService);
public:
    IgQtAppStatusBarService (IgState *state, QWidget *mainWindow);
    ~IgQtAppStatusBarService (void);

    void		setMessage (QString message);
    void		setMessage (const std::string &message);
    void		setMessage (const char *message);

private:
    IgState		*m_state;
    QMainWindow		*m_mainWindow;

    // undefined semantics
    IgQtAppStatusBarService (const IgQtAppStatusBarService &);
    IgQtAppStatusBarService &operator= (const IgQtAppStatusBarService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_APP_STATUS_BAR_SERVICE_H
