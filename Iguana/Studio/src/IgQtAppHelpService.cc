#include "Iguana/Studio/interface/IgQtAppHelpService.h"
#include "Iguana/Studio/interface/IgQtAppHelp.h"
#include "Iguana/Studio/interface/iguana-small.xpm"
#include <QMenuBar>
#include <QMainWindow>
#include <QPixmap>
#include <QApplication>

IG_DEFINE_STATE_ELEMENT (IgQtAppHelpService, "Services/Qt/App Help");

IgQtAppHelpService::IgQtAppHelpService (IgState *state,
			                QWidget *mainWindow)
    : m_state (state),
      m_mainWindow (dynamic_cast<QMainWindow *> (mainWindow)),
      m_about (new IgQtAppHelp (state, mainWindow))
{
    ASSERT (state);
    ASSERT (mainWindow);
    state->put (s_key, this);

    QMenuBar *menubar = m_mainWindow->menuBar ();
    QMenu *menuHelp = new QMenu (menubar);
    menuHelp->setObjectName (QString::fromUtf8 ("menuHelp"));
    menuHelp->setTearOffEnabled (true);
    menubar->addMenu (menuHelp);
    
    QIcon icon;
    icon.addPixmap (QPixmap (iguana_small_xpm), QIcon::Normal, QIcon::Off);

    QAction *actionAbout = new QAction (mainWindow);
    actionAbout->setObjectName (QString::fromUtf8 ("actionAbout"));
    actionAbout->setEnabled (true);
    actionAbout->setIcon (icon);

    QAction *actionWebSite = new QAction (mainWindow);
    actionWebSite->setObjectName (QString::fromUtf8 ("actionWebSite"));
    actionWebSite->setEnabled (true);
    
    QAction *actionReportBug = new QAction (mainWindow);
    actionReportBug->setObjectName (QString::fromUtf8 ("actionReportBug"));
    actionReportBug->setEnabled (true);

    QAction *actionViewerButtons = new QAction (mainWindow);
    actionViewerButtons->setObjectName (QString::fromUtf8 ("actionViewerButtons"));
    actionViewerButtons->setEnabled (true);

    QAction *actionAboutQt = new QAction (mainWindow);
    actionAboutQt->setObjectName (QString::fromUtf8 ("actionAboutQt"));
    actionAboutQt->setEnabled (true);

    menuHelp->addAction (actionWebSite);
    menuHelp->addSeparator ();
    menuHelp->addAction (actionReportBug);
    menuHelp->addSeparator ();
    menuHelp->addAction (actionViewerButtons);    
    menuHelp->addSeparator ();
    menuHelp->addAction (actionAboutQt);
    menuHelp->addAction (actionAbout);

    QObject::connect (actionWebSite, SIGNAL(triggered()), m_about, SLOT(iguanaWebSite ()));
    QObject::connect (actionReportBug, SIGNAL(triggered()), m_about, SLOT(reportBugDialog ()));
    QObject::connect (actionViewerButtons, SIGNAL(triggered()), m_about, SLOT(buttons ()));
    QObject::connect (actionAboutQt, SIGNAL(triggered()), m_about, SLOT(aboutQt ()));
    QObject::connect (actionAbout, SIGNAL(triggered()), m_about, SLOT(about()));
    
    actionWebSite->setText (QApplication::translate ("IgMainWindow", "IGUANA &Web site", 0, QApplication::UnicodeUTF8));
    actionWebSite->setShortcut (QApplication::translate ("IgMainWindow", "F1", 0, QApplication::UnicodeUTF8));
    actionReportBug->setText (QApplication::translate ("IgMainWindow", "&Report Bug...", 0, QApplication::UnicodeUTF8));
    actionViewerButtons->setText (QApplication::translate ("IgMainWindow", "Viewer &Buttons", 0, QApplication::UnicodeUTF8));
    actionAboutQt->setText (QApplication::translate ("IgMainWindow", "About Qt", 0, QApplication::UnicodeUTF8));
    actionAbout->setText (QApplication::translate ("IgMainWindow", "About IGUANA", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle (QApplication::translate ("IgMainWindow", "&Help", 0, QApplication::UnicodeUTF8));
}

IgQtAppHelpService::~IgQtAppHelpService (void)
{
    ASSERT (m_state);
    ASSERT (m_mainWindow);
    m_state->detach (s_key);
}

