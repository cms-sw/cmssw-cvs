#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/Framework/interface/IgArgsElement.h"
#include "Iguana/Studio/interface/IgQMainWindowSite.h"
#include "Iguana/Studio/interface/IgQtSite.h"
#include "Iguana/Studio/interface/IgQtAppExtensions.h"
#include "Iguana/Framework/interface/IgSelectionService.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/Studio/interface/IgQtAppActiveService.h"
#include "Iguana/Studio/interface/IgDocumentDataManager.h"
#include "Iguana/Studio/interface/IgModelManager.h"
#include <Inventor/Qt/SoQt.h>
#include <QtGui>

int main (int argc, char **argv)
{
    SoQt::init(argc, argv, argv[0]);
    IgState *state = new IgState;
    
    // Initialise Qt
    IgArgsElement	*args = new IgArgsElement (state, argc, argv);
    QApplication	qapp (args->args (), args->argv ());
    QApplication::setOverrideCursor (Qt::waitCursor);
    
    // Show splash screen
    QSplashScreen *splash = new QSplashScreen(QPixmap(QString::fromUtf8(":/icons/IGUANA_SPLASH.xpm")));
    splash->show();

    IgQMainWindowSite *mainSite = new IgQMainWindowSite (state, 0);
    QWidget *mainWindow = IgQtSite::selfFrom (mainSite);
    qApp->setMainWidget (mainWindow);
    qApp->setStyle (new QWindowsStyle);

    IgQtAppExtensions::install (state, mainWindow);

    new IgSelectionService (state);
    new IgQtObjectMenuService (state);
    new IgQtAppActiveService (state);
    new IgDocumentDataManager (state);
    new IgModelManager (state);
    
    std::string caption("IGUANA");
    mainWindow->setCaption (caption.c_str());
    
    unsigned int x = 1000, y=760;
    mainWindow->resize (x, y);
    mainWindow->show ();

    // Delete splash screen and restore cursor   
    splash->clear ();
    QApplication::restoreOverrideCursor ();
    delete splash;

    SoQt::mainLoop();
    return 0;
}
