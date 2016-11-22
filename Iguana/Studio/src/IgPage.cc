//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Studio/interface/IgDocument.h"
#include "Iguana/Studio/interface/IgView.h"
#include "Iguana/Studio/interface/IgViewDB.h"
#include "Iguana/Studio/interface/IgViewCreator.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtAppToolBarService.h"
#include "Iguana/Studio/interface/IgQtAppWindowService.h"
#include "Iguana/Studio/interface/IgQtSite.h"
#include <QMenuBar>
#include <QApplication>
#include <QMainWindow>
#include <QMdiArea>
#include <QShortcut>
#include <QToolBar>
#include <classlib/utils/DebugAids.h>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPage::IgPage (IgDocument *doc)
  : IgQWidgetSite (doc->state (), doc->window ()),
      m_document (doc),
      m_numRegisteredWindows (0)
{
    QMainWindow *mainWindow = dynamic_cast<QMainWindow *>(IgQtAppWindowService::get (m_document->state ())->mainWindow ());
    m_workspace = new QMdiArea (mainWindow->centralWidget ());

    // FIXME: register in document
    // FIXME: read, write support
    ASSERT (doc);    
    doc->addPage (this);    
    
    QMenuBar *menubar = mainWindow->menuBar ();
    QShortcut *shortcut = new QShortcut (Qt::Key_Escape, m_workspace); 
    // adds Esc accelerator
    // connected to IgPage's
    // showFullScreen() slot
    connect (shortcut, SIGNAL(activated()), this, SLOT (showFullScreen ()));

    // Here we populate the file menu with the "New" entry to allow user to
    // create new views of the different models
    QMenu *fileMenu = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuFile"));
    ASSERT (fileMenu);

    QMenu *menuNew = new QMenu (fileMenu);
    menuNew->setObjectName (QString::fromUtf8 ("menuNew"));
    menuNew->setTearOffEnabled (true);

    QList<QAction *> alist = fileMenu->actions ();
    QAction *sep = fileMenu->insertSeparator (alist.at (0));
    fileMenu->insertMenu (sep, menuNew);
    menuNew->setTitle (QApplication::translate ("IgMainWindow", "&New", 0, QApplication::UnicodeUTF8));
    
    IgViewDB *viewDB = IgViewDB::get ();
    ASSERT (viewDB);
    
    // FIXME: sometimes, especially after rebuilding, viewDB is
    // empty...We need to find some better way to get notified when a
    // new IgView plugin is loaded.
    for (IgViewDB::Iterator i = viewDB->begin ();
	 i != viewDB->end ();
	 i++)
    {	
	IgViewCreator *viewCreator = new IgViewCreator (i, this);

	QAction *actionNewView = new QAction (menuNew);
	actionNewView->setObjectName (QString::fromUtf8 (QString::fromStdString ((*i)->name ())));
	actionNewView->setEnabled (true);

	QObject::connect (actionNewView, SIGNAL(triggered()), viewCreator, SLOT(createView ()));
	menuNew->addAction (actionNewView);
	actionNewView->setText (QApplication::translate ("IgMainWindow", QString::fromStdString ((*i)->name ()), 0, QApplication::UnicodeUTF8));
    }
    
    // Here we create the "Window" menu in which the user can decide
    // to hide/show some utility views (for example the python shell
    // should go here...)
    //
    // FIXME: how do we distinguish between different kind of Views?
    // By having two, separate, plugin categories?
    QMenu *menuWindow = new QMenu (menubar);
    menuWindow->setObjectName (QString::fromUtf8 ("menuWindow"));
    menuWindow->setTearOffEnabled (true);

    QMenu *menuDebug = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuDebug"));
    ASSERT (menuDebug);    
    menubar->insertMenu (menuDebug->menuAction (), menuWindow);
    menuWindow->setTitle (QApplication::translate ("IgMainWindow", "&Window", 0, QApplication::UnicodeUTF8));

    QAction *actionTile = new QAction (menuWindow);
    actionTile->setObjectName (QString::fromUtf8 ("actionTile"));
    actionTile->setEnabled (true);

    QAction *actionCascade = new QAction (menuWindow);
    actionCascade->setObjectName (QString::fromUtf8 ("actionCascade"));
    actionCascade->setEnabled (true);

    QAction *actionMaximized = new QAction (menuWindow);
    actionMaximized->setObjectName (QString::fromUtf8 ("actionMaximized"));
    actionMaximized->setEnabled (true);

    QAction *actionFullScreen = new QAction (menuWindow);
    actionFullScreen->setObjectName (QString::fromUtf8 ("actionFullScreen"));
    actionFullScreen->setEnabled (true);

    menuWindow->addAction (actionTile);
    menuWindow->addAction (actionCascade);
    menuWindow->addSeparator();
    menuWindow->addAction (actionMaximized);
    menuWindow->addAction (actionFullScreen); 
    menuWindow->addSeparator();

    QObject::connect (actionTile, SIGNAL(triggered()), m_workspace, SLOT(tileSubWindows ()));
    QObject::connect (actionCascade, SIGNAL(triggered()), m_workspace, SLOT(cascadeSubWindows ()));
    QObject::connect (actionMaximized, SIGNAL(triggered()), this, SLOT(showMaximized ()));
    QObject::connect (actionFullScreen, SIGNAL(triggered()), this, SLOT(showFullScreen ()));

    actionTile->setText (QApplication::translate ("IgMainWindow", "&Tile", 0, QApplication::UnicodeUTF8));
    actionCascade->setText (QApplication::translate ("IgMainWindow", "&Cascade", 0, QApplication::UnicodeUTF8));
    actionMaximized->setText (QApplication::translate ("IgMainWindow", "Show &Maximized", 0, QApplication::UnicodeUTF8));
    actionFullScreen->setText (QApplication::translate ("IgMainWindow", "&Full screen", 0, QApplication::UnicodeUTF8));
    actionFullScreen->setShortcut (QApplication::translate ("IgMainWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));

//     IgQtFocusManager *focusManager = new IgQtFocusManager (state (), &m_views);
    
//     QObject::connect (m_workspace,
// 		      SIGNAL (subWindowActivated (QMdiSubWindow *)),
// 		      focusManager,
// 		      SLOT (windowActivated (QMdiSubWindow *)));
    
    //IgQtSite::selfFrom (doc->window ())->show ();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgState *
IgPage::state (void) const
{ return m_document->state (); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
QMdiArea *
IgPage::workspace (void)
{
    return m_workspace;
}

void
IgPage::registerView (IgView *view, QWidget *selectableWidget)
{
    ASSERT (m_views[selectableWidget] == 0);
    m_views[selectableWidget] = view;
    
    // FIXME: maybe we should also introduce different kind of views,
    // so that a view can have a title, without having to be
    // registered in the window menu.
    if (view->title () != "")
    {
	QMainWindow *mainWindow = dynamic_cast<QMainWindow *>(IgQtAppWindowService::get (m_document->state ())->mainWindow ());
	QMenuBar *menubar = mainWindow->menuBar ();
	QMenu *menuWindow = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuWindow"));
	QAction *actionNewView = new QAction (menuWindow);
	actionNewView->setObjectName (QString::fromUtf8 (QString::fromStdString (view->title ())));
	actionNewView->setEnabled (true);

	QObject::connect (actionNewView, SIGNAL(triggered()), selectableWidget, SLOT(setFocus ()));
	QObject::connect (actionNewView, SIGNAL(triggered()), selectableWidget, SLOT(show ()));
	menuWindow->addAction (actionNewView);
	actionNewView->setText (QApplication::translate ("IgMainWindow", QString::fromStdString (view->title ()), 0, QApplication::UnicodeUTF8));
	// FIXME: If minimized, should be unfocused first.
    }
}

void
IgPage::showMaximized (void)
{    
    QMainWindow *mainWindow = dynamic_cast<QMainWindow *>(IgQtAppWindowService::get (m_document->state ())->mainWindow ());
    QMenuBar *menubar = mainWindow->menuBar ();
    QMenu *menuWindow = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuWindow"));
    ASSERT (menuWindow);
    
    QAction *actionMaximized = menuWindow->findChild <QAction *> (QString::fromUtf8 ("actionMaximized"));
    QAction *actionFullScreen = menuWindow->findChild <QAction *> (QString::fromUtf8 ("actionFullScreen"));
    ASSERT (actionMaximized);
    ASSERT (actionFullScreen);

    if (mainWindow->isMaximized ())
    {
	actionMaximized->setText (QApplication::translate ("IgMainWindow", "Show &Maximized", 0, QApplication::UnicodeUTF8));
	actionFullScreen->setEnabled (true);
	mainWindow->showNormal ();
    } 
    else 
    { 
	actionMaximized->setText (QApplication::translate ("IgMainWindow", "Show &Normal", 0, QApplication::UnicodeUTF8));
	actionFullScreen->setEnabled (false);
	mainWindow->showMaximized ();
    }
}

void		
IgPage::showFullScreen (void)
{    
    QMainWindow *mainWindow = dynamic_cast<QMainWindow *>(IgQtAppWindowService::get (m_document->state ())->mainWindow ());
    QMenuBar *menubar = mainWindow->menuBar ();
    QMenu *menuWindow = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuWindow"));
    ASSERT (menuWindow);
    
    QAction *actionMaximized = menuWindow->findChild <QAction *> (QString::fromUtf8 ("actionMaximized"));
    ASSERT (actionMaximized);

    QMenuBar *menu = IgQtAppMenuService::get (m_document->state ())->menuBar ();
    ASSERT (menu);
    QToolBar *toolBar = IgQtAppToolBarService::get (m_document->state ())->toolBar (3000, "General graphics ops");
    ASSERT (toolBar);

    if (mainWindow->isFullScreen ())
    {
	actionMaximized->setEnabled (true);
	menu->show ();
	toolBar->show ();
	mainWindow->showNormal ();
    } 
    else 
    { 
	menu->hide ();
	toolBar->hide ();
	actionMaximized->setEnabled (false);
	mainWindow->showFullScreen ();
    }
}

void
IgPage::printViews (const QString &text) 
{
    emit autoPrint (text);    
}
