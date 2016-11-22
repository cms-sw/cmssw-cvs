#ifndef IGUANA_STUDIO_IG_QT_APP_HELP_H
# define IGUANA_STUDIO_IG_QT_APP_HELP_H

# include "Iguana/Studio/interface/config.h"
# include <QObject>

class IgState;

class IGUANA_STUDIO_API IgQtAppHelp : public QObject
{
    Q_OBJECT
public:
    IgQtAppHelp (IgState *state, QWidget *parent = 0);
    ~IgQtAppHelp (void);

    //! Tries to open the url in running netscape
    // or starts netscape if there is no netscape running.
    // If netscape is not installed, pops up a message box.
    static void		netscape (const QString & url);
    
private slots:
    //! Tell us about yourself
    void		about (void);
    //! Tell us about Qt
    void		aboutQt (void);
    //! Requests 3D buttons description to be shown in a web browser
    void 		buttons (void);
    //! Requests IGUANA web page to be shown in a web browser
    void		iguanaWebSite (void);
    //! Pop-up a dialog to report a bug
    void		reportBugDialog (void);
    
private:
    void		init (void);
   
    IgState		*m_state;
    QWidget		*m_parent;
};

#endif // IGUANA_STUDIO_IG_QT_APP_HELP_H







