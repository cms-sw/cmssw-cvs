//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Studio/interface/IgQtAppDebugService.h"
#include "Iguana/Studio/interface/IgQtAppDebug.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include <classlib/utils/DebugAids.h>
#include <QMenuBar>
#include <QMainWindow>
#include <QApplication>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtAppDebugService, "Services/Qt/App Debug");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAppDebugService::IgQtAppDebugService (IgState *state,
					  QWidget *mainWindow)
    : m_state (state),
      m_mainWindow (dynamic_cast<QMainWindow *> (mainWindow)),
      m_info (new IgQtAppDebug (state, mainWindow))
{
    ASSERT (state);
    ASSERT (mainWindow);
    state->put (s_key, this);

    QMenuBar *menubar = m_mainWindow->menuBar ();
    QMenu *menuDebug = new QMenu (menubar);
    menuDebug->setObjectName (QString::fromUtf8 ("menuDebug"));
    menuDebug->setTearOffEnabled (true);

    QMenu *menuHelp = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuHelp"));
    if (menuHelp != 0)
	menubar->insertMenu (menuHelp->menuAction (), menuDebug);
    else
    {	
	menubar->addMenu (menuDebug);
        menuDebug->setTitle (QApplication::translate ("IgMainWindow", "&Debug", 0, QApplication::UnicodeUTF8));
    }
    
    QAction *actionResources = new QAction (menuDebug);
    actionResources->setObjectName (QString::fromUtf8 ("actionResources"));
    actionResources->setEnabled (true);

    QAction *actionLibraries = new QAction (menuDebug);
    actionLibraries->setObjectName (QString::fromUtf8 ("actionLibraries"));
    actionLibraries->setEnabled (true);

    QAction *actionPlugins = new QAction (menuDebug);
    actionPlugins->setObjectName (QString::fromUtf8 ("actionPlugins"));
    actionPlugins->setEnabled (true);

    QAction *actionStateTree = new QAction (menuDebug);
    actionStateTree->setObjectName (QString::fromUtf8 ("actionStateTree"));
    actionStateTree->setEnabled (true);

    QAction *actionDebugMe = new QAction (menuDebug);
    actionDebugMe->setObjectName (QString::fromUtf8 ("actionDebugMe"));
    actionDebugMe->setEnabled (true);

    QAction *actionShowProf = new QAction (menuDebug);
    actionShowProf->setObjectName (QString::fromUtf8 ("actionShowProf"));
    actionShowProf->setEnabled (true);

    menuDebug->addAction (actionResources);
    menuDebug->addAction (actionLibraries);
    menuDebug->addAction (actionPlugins);
    menuDebug->addAction (actionStateTree);
    menuDebug->addAction (actionDebugMe);
    menuDebug->addSeparator();
    menuDebug->addAction (actionShowProf);    

    QObject::connect (actionResources, SIGNAL(triggered()), m_info, SLOT(resources ()));
    QObject::connect (actionLibraries, SIGNAL(triggered()), m_info, SLOT(libraries ()));
    QObject::connect (actionPlugins, SIGNAL(triggered()), m_info, SLOT(plugins ()));
    QObject::connect (actionStateTree, SIGNAL(triggered()), m_info, SLOT(dumpState ()));
    QObject::connect (actionDebugMe, SIGNAL(triggered()), m_info, SLOT(debugme ()));
    QObject::connect (actionShowProf, SIGNAL(triggered()), m_info, SLOT(showProfiler ()));

    actionResources->setText (QApplication::translate ("IgMainWindow", "Show &Resources", 0, QApplication::UnicodeUTF8));
    actionLibraries->setText (QApplication::translate ("IgMainWindow", "Show &Libraries", 0, QApplication::UnicodeUTF8));
    actionPlugins->setText (QApplication::translate ("IgMainWindow", "Show &Plugins", 0, QApplication::UnicodeUTF8));
    actionStateTree->setText (QApplication::translate ("IgMainWindow", "&Show State Tree", 0, QApplication::UnicodeUTF8));
    actionDebugMe->setText (QApplication::translate ("IgMainWindow", "&Debug Me Now", 0, QApplication::UnicodeUTF8));
    actionShowProf->setText (QApplication::translate ("IgMainWindow", "Show &Profiler", 0, QApplication::UnicodeUTF8));
}

IgQtAppDebugService::~IgQtAppDebugService (void)
{
    ASSERT (m_state);
    ASSERT (m_mainWindow);
    m_state->detach (s_key);
}
