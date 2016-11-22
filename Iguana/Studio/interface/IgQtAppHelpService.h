#ifndef IGUANA_STUDIO_IG_QT_APP_HELP_SERVICE_H
# define IGUANA_STUDIO_IG_QT_APP_HELP_SERVICE_H

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"

class IgQtAppHelp;
class QMainWindow;
class QWidget;

class IGUANA_STUDIO_API IgQtAppHelpService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppHelpService);
public:
    IgQtAppHelpService (IgState *state, QWidget *mainWindow);
    ~IgQtAppHelpService (void);

private:
    IgState			*m_state;
    QMainWindow			*m_mainWindow;
    IgQtAppHelp			*m_about;
};

#endif // IGUANA_STUDIO_IG_QT_APP_HELP_SERVICE_H
