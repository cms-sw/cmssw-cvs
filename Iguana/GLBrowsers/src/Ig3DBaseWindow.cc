#include "Iguana/GLBrowsers/interface/Ig3DBaseWindow.h"
#include "Iguana/Studio/interface/IgModelManager.h"
#include "Iguana/Studio/interface/IgDocumentDataManager.h"
#include "Iguana/GLModels/interface/Ig3DModel.h"
#include "Iguana/GLBrowsers/interface/Ig3DSystem.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/GLBrowsers/interface/Ig3DBaseBrowser.h"
#include "Iguana/GLBrowsers/interface/IgCommonViewPropertiesCategory.h"
#include "Iguana/GLBrowsers/interface/IgTwigConfigElement.h"
#include "Iguana/Studio/interface/IgQtSite.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtAppWindowService.h"
#include "Iguana/Studio/interface/IgQtAppContextService.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/GLBrowsers/interface/IgStudioCentre.h"
#include "Iguana/GLBrowsers/interface/IgControlCentre.h"
#include <QtGui>

static void autoinit (IgState *state, QMdiArea *site)
{ Ig3DSystem::init (state, site); }

static Ig3DModel *getModelFromManager (IgPage *page)
{
  autoinit (page->state (), page->workspace ());
    
  IgModelManager *manager = IgModelManager::get (page->state ());
  if (!manager)
    manager = new IgModelManager (page->state ());
    
  Ig3DModel *model = 0;
  manager->lookup (page->state (), &model);
  return model;
}

Ig3DBaseWindow::Ig3DBaseWindow (IgPage *page)
  : IgView(page),
    actionInvertCamera(0),
    actionShowAxis(0),
    actionCameraToggle(0),
    m_site(page),
    m_browser(0)
{ 
  m_model = getModelFromManager (page);
  m_state = new IgState (IgDocumentDataManager::get(page->state ())->
			 lookup (page->state ()));
  m_centre = IgStudioCentre::get (m_state)->controlCentre ();
     
  if (! IgTwigConfigElement::get (m_state))
    new IgTwigConfigElement (page->state ());

  // This is the context for this window 
  IgQtAppContextService *contextService = new IgQtAppContextService (m_state,
								     IgQtAppWindowService::get (m_state)->mainWindow ());
  contextService->focusIn ();
}

void
Ig3DBaseWindow::watchBrowser (Ig3DBaseBrowser *browser)
{
  m_browser = browser;
  // Show myself and show all the document data (FIXME: select?).
  QString t (IgDocumentDataRoot::getCurrentRoot());
  t += QString(" ( ") + title ().c_str () + " )";
		    
  m_browser->getWidget ()->parentWidget ()->setCaption (t);
  setTitle (t.latin1 ());
  setWindowTitle (t);
    
  browse (IgDocumentData::get (m_state)->root ());
  // register the MDI window used by this view.
  page ()->registerView (dynamic_cast <IgView *> (this), 
			 m_browser->getWidget ()->parentWidget ());
  connect (page (), SIGNAL (autoPrint (const QString &)), this, SLOT (print (const QString &)));
  connect (m_browser, SIGNAL(windowStateChanged (Qt::WindowStates, Qt::WindowStates)),
	   this, SLOT (changeWindowState (Qt::WindowStates, Qt::WindowStates)));
}

void
Ig3DBaseWindow::changeWindowState (Qt::WindowStates oldState, Qt::WindowStates newState) 
{
  IgQtAppContextService *contextService = IgQtAppContextService::get (m_state);
  QMdiArea *ws = dynamic_cast <IgPage *>(m_site)->workspace ();
  QList<QMdiSubWindow *> wlist = ws->subWindowList ();
  qDebug () << "Old state: " << oldState;
  qDebug () << "New state: " << newState;

  // Application lost focus
  if (QApplication::activeWindow () == 0)
  {
    qDebug () << "QApplication lost focus. Do nothing.";
    return;
  }
  
  if (wlist.length () > 1)
  {
    contextService->focusIn ();
    qDebug () << "There is more then 1 window";
  
    if (oldState == 0 && (newState == Qt::WindowActive || newState == 10 || newState == 9))
    {
      contextService->focusIn ();
      toolBar->setVisible (true);
    } 
    else if ((oldState == Qt::WindowActive || oldState == 10 || oldState == 9)
	     && (newState == 0 || newState == 1))
    {
      contextService->focusOut ();
      toolBar->setVisible (false);
    }
  }
  else
  {
    contextService->focusIn ();
    qDebug () << "There only 1 window. Do nothing.";
  }
}

void
Ig3DBaseWindow::initCategories (void)
{
  // Make sure this is called only once.
  // Create a 3D View Properties Category and add it in control
  // centre.
    
  m_viewPropertiesCategory = IgCommonViewPropertiesCategory::get (page ()->state ());
  if (!m_viewPropertiesCategory)
  {
    // Category is added to the global state.
    m_viewPropertiesCategory = 
      new IgCommonViewPropertiesCategory (page ()->state ());
  }
  m_viewPropertiesCategory->registerBrowser (m_state, browser ());
}

void 
Ig3DBaseWindow::initMenu (void)
{
  ASSERT (m_state);    
  ASSERT (IgQtAppMenuService::get (m_state));

  QMenuBar *menubar = IgQtAppMenuService::get (m_state)->menuBar ();
  QMenu *menuView = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuView"));   
  if (! menuView)
  {
    menuView = new QMenu (menubar);
    menuView->setObjectName (QString::fromUtf8 ("menuView"));
    menuView->setTearOffEnabled (true);
	
    QMenu *menuWindow = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuWindow"));
    ASSERT (menuWindow);
    menubar->insertMenu (menuWindow->menuAction (), menuView);
    menuView->setTitle (QApplication::translate ("IgMainWindow", "&View", 0, QApplication::UnicodeUTF8));
  }
  QAction *actionToolbar = menuView->findChild <QAction *> (QString::fromUtf8 ("actionToolbar"));
  if (actionToolbar == 0)
  {
    actionToolbar = new QAction (menuView);
    actionToolbar->setObjectName (QString::fromUtf8 ("actionToolbar"));
    actionToolbar->setEnabled (true);
    actionToolbar->setCheckable (true);
    actionToolbar->setChecked (true);
    actionToolbar->setText (QApplication::translate ("IgMainWindow", "&Tool Bar", 0, QApplication::UnicodeUTF8));
	
    menuView->addAction (actionToolbar);
    menuView->addSeparator ();
  }

  QObject::connect (actionToolbar, SIGNAL(toggled(bool)), this, SLOT(toolbarVisibilityChanged(bool)));

  // Register the message broadcasting only when the window is
  // focused, unregister on focus out.
  ASSERT (IgQtAppContextService::get (state ()));
  IgQtAppContextService *cs = IgQtAppContextService::get (state ());
  cs->addOnFocusIn (lat::CreateCallback (this, &Ig3DBaseWindow::focusIn));
  cs->addOnFocusOut (lat::CreateCallback (this, &Ig3DBaseWindow::focusOut));
}

void
Ig3DBaseWindow::initToolBar (void)
{
  // This creates a toolbar with the general widgets.
  toolBar = new QToolBar(this);
  ASSERT (toolBar);
    
  QIcon iconSaveAs;
  iconSaveAs.addPixmap(QPixmap(QString::fromUtf8(":/icons/filesave.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconPrint;
  iconPrint.addPixmap(QPixmap(QString::fromUtf8(":/icons/fileprint.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconConCen;
  iconConCen.addPixmap(QPixmap(QString::fromUtf8(":/icons/control_centre.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconZoomIn;
  iconZoomIn.addPixmap(QPixmap(QString::fromUtf8(":/icons/zoom_plus.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconZoomOut;
  iconZoomOut.addPixmap(QPixmap(QString::fromUtf8(":/icons/zoom_minus.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconPick;
  iconPick.addPixmap(QPixmap(QString::fromUtf8(":/icons/pick.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconViewMode;
  iconViewMode.addPixmap(QPixmap(QString::fromUtf8(":/icons/view.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconSeek;
  iconSeek.addPixmap(QPixmap(QString::fromUtf8(":/icons/seek.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconHome;
  iconHome.addPixmap(QPixmap(QString::fromUtf8(":/icons/home.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconSetHome;
  iconSetHome.addPixmap(QPixmap(QString::fromUtf8(":/icons/set_home.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconViewAll;
  iconViewAll.addPixmap(QPixmap(QString::fromUtf8(":/icons/view_all.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconYX;
  iconYX.addPixmap(QPixmap(QString::fromUtf8(":/icons/yx_small.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconXZ;
  iconXZ.addPixmap(QPixmap(QString::fromUtf8(":/icons/xz_small.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconYZ;
  iconYZ.addPixmap(QPixmap(QString::fromUtf8(":/icons/yz_small.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconAutoPrint;
  iconAutoPrint.addPixmap(QPixmap(QString::fromUtf8(":/icons/autoprint.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconShowAxis;
  iconShowAxis.addPixmap(QPixmap(QString::fromUtf8(":/icons/yx_small_col.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconInvertCamera;
  iconInvertCamera.addPixmap(QPixmap(QString::fromUtf8(":/icons/inverse.xpm")), QIcon::Normal, QIcon::Off);
  QIcon iconCameraToggle;
  iconCameraToggle.addPixmap(QPixmap(QString::fromUtf8(":/icons/perspective.xpm")), QIcon::Normal, QIcon::Off);
  iconCameraToggle.addPixmap(QPixmap(QString::fromUtf8(":/icons/ortho.xpm")), QIcon::Normal, QIcon::On);
  
  QAction *actionSaveAs = new QAction (toolBar);
  actionSaveAs->setObjectName (QString::fromUtf8 ("actionSaveAs"));
  actionSaveAs->setEnabled (true);
  actionSaveAs->setIcon (iconSaveAs);
  actionSaveAs->setToolTip(QString::fromUtf8 ("Save As"));
  actionSaveAs->setWhatsThis (QString::fromUtf8 ("Save scene graph."));

  QAction *actionPrint = new QAction (toolBar);
  actionPrint->setObjectName (QString::fromUtf8 ("actionPrint"));
  actionPrint->setEnabled (true);
  actionPrint->setIcon (iconPrint);
  actionPrint->setToolTip(QString::fromUtf8 ("Print"));
  actionPrint->setWhatsThis (QString::fromUtf8 ("Print scene graph."));

  QAction *actionConCen = new QAction (toolBar);
  actionConCen->setObjectName (QString::fromUtf8 ("actionConCen"));
  actionConCen->setEnabled (true);
  actionConCen->setIcon (iconConCen);
  actionConCen->setToolTip(QString::fromUtf8 ("Show Control Centre"));
  actionConCen->setWhatsThis(QString::fromUtf8 ("Show Control Centre. You can manipulate Clip "
						"planes, slicers, lights, camera, view/print "
						"setting via control centre."));

  QAction *actionZoomIn = new QAction (toolBar);
  actionZoomIn->setObjectName (QString::fromUtf8 ("actionZoomIn"));
  actionZoomIn->setEnabled (true);
  actionZoomIn->setIcon (iconZoomIn);
  actionZoomIn->setToolTip(QString::fromUtf8 ("Zoom In"));
  actionZoomIn->setWhatsThis (QString::fromUtf8 ("Zoom in to the scene graph"));

  QAction *actionZoomOut = new QAction (toolBar);
  actionZoomOut->setObjectName (QString::fromUtf8 ("actionZoomOut"));
  actionZoomOut->setEnabled (true);
  actionZoomOut->setIcon (iconZoomOut);
  actionZoomOut->setToolTip(QString::fromUtf8 ("Zoom Out"));
  actionZoomOut->setWhatsThis (QString::fromUtf8 ("Zoom out of the scene graph"));

  QAction *actionPick = new QAction (toolBar);
  actionPick->setObjectName (QString::fromUtf8 ("actionPick"));
  actionPick->setEnabled (true);
  actionPick->setCheckable (true);
  actionPick->setIcon (iconPick);
  actionPick->setToolTip(QString::fromUtf8 ("Pick"));
  actionPick->setWhatsThis (QString::fromUtf8 ("Selects object manipulation or pick mode \n"
					       "(and deselects camera or viewer mode).\n"
					       "The cursor shape will change to an arrow.\n"
					       "In this mode, the user is manipulating objects\n"
					       "in the scene graph."));

  QAction *actionViewMode = new QAction (toolBar);
  actionViewMode->setObjectName (QString::fromUtf8 ("actionViewMode"));
  actionViewMode->setEnabled (true);
  actionViewMode->setCheckable (true);
  actionViewMode->setChecked (true);
  actionViewMode->setIcon (iconViewMode);
  actionViewMode->setToolTip(QString::fromUtf8 ("View"));
  actionViewMode->setWhatsThis (QString::fromUtf8 ("Selects camera or viewer mode (and deselects object \n"
						   "manipulation or pick mode). The cursor shape will\n"
						   "change to a circular arrows icon. In this mode,\n"
						   "the user is moving the camera in 3D space."));
      
  QActionGroup *groupViewMode = new QActionGroup (toolBar);
  groupViewMode->setObjectName (QString::fromUtf8 ("groupViewMode"));
  groupViewMode->addAction (actionViewMode);
  groupViewMode->addAction (actionPick);
  groupViewMode->setExclusive (true);

  QAction *actionSeek = new QAction (toolBar);
  actionSeek->setObjectName (QString::fromUtf8 ("actionSeek"));
  actionSeek->setEnabled (true);
  actionSeek->setIcon (iconSeek);
  actionSeek->setToolTip(QString::fromUtf8 ("Seek"));
  actionSeek->setWhatsThis (QString::fromUtf8 ("Allows the user to select a new center\n"
					       "of rotation for the camera. When clicked\n"
					       "on (and in viewer mode) the cursor changes\n"
					       "to a crosshair. The next left mouse\n"
					       "buttonpress causes whatever is underneath\n"
					       "the cursor to be selected as the new center\n"
					       "of rotation. Once the button is released,\n"
					       "the camera either jumps or animates to its\n"
					       "new position depending on the current\n"
					       "setting of the seek time in the preferences\n"
					       "dialog box (see View Setting control "
					       "category.)"));
    
  QAction *actionHome = new QAction (toolBar);
  actionHome->setObjectName (QString::fromUtf8 ("actionHome"));
  actionHome->setEnabled (true);
  actionHome->setIcon (iconHome);
  actionHome->setToolTip(QString::fromUtf8 ("Home"));
  actionHome->setWhatsThis (QString::fromUtf8 ("Return camera to its home position\n"
					       "(initial position if not reset)."));

  QAction *actionSetHome = new QAction (toolBar);
  actionSetHome->setObjectName (QString::fromUtf8 ("actionSetHome"));
  actionSetHome->setEnabled (true);
  actionSetHome->setIcon (iconSetHome);
  actionSetHome->setToolTip(QString::fromUtf8 ("Set Home"));
  actionSetHome->setWhatsThis (QString::fromUtf8 ("Resets the home position to the current\n"
						  "camera position."));

  QAction *actionViewAll = new QAction (toolBar);
  actionViewAll->setObjectName (QString::fromUtf8 ("actionViewAll"));
  actionViewAll->setEnabled (true);
  actionViewAll->setIcon (iconViewAll);
  actionViewAll->setToolTip(QString::fromUtf8 ("View All"));
  actionViewAll->setWhatsThis (QString::fromUtf8 ("Brings the entire scene graph into view."));

  actionInvertCamera = new QAction (toolBar);
  actionInvertCamera->setObjectName (QString::fromUtf8 ("actionInvertCamera"));
  actionInvertCamera->setEnabled (false);
  actionInvertCamera->setIcon (iconInvertCamera);
  actionInvertCamera->setToolTip (QString::fromUtf8 ("Invert Camera"));
  actionInvertCamera->setWhatsThis (QString::fromUtf8 ("Invert the camera orientation and "
						       "position through the origin."));

  actionYX = new QAction (toolBar);
  actionYX->setObjectName (QString::fromUtf8 ("actionYX"));
  actionYX->setEnabled (false);
  actionYX->setIcon (iconYX);
  actionYX->setToolTip (QString::fromUtf8 ("Align camera along -ve Z-axis."));
  actionYX->setWhatsThis (QString::fromUtf8 ("Align camera along -ve Z-axis."));

  actionXZ = new QAction (toolBar);
  actionXZ->setObjectName (QString::fromUtf8 ("actionXZ"));
  actionXZ->setEnabled (false);
  actionXZ->setIcon (iconXZ);
  actionXZ->setToolTip (QString::fromUtf8 ("Align camera along -ve Y-axis."));
  actionXZ->setWhatsThis (QString::fromUtf8 ("Align camera along -ve Y-axis."));

  actionYZ = new QAction (toolBar);
  actionYZ->setObjectName (QString::fromUtf8 ("actionYZ"));
  actionYZ->setEnabled (false);
  actionYZ->setIcon (iconYZ);
  actionYZ->setToolTip (QString::fromUtf8 ("Align camera along +ve X-axis."));
  actionYZ->setWhatsThis (QString::fromUtf8 ("Align camera along +ve X-axis."));

  QAction *actionAutoPrint = new QAction (toolBar);
  actionAutoPrint->setObjectName (QString::fromUtf8 ("actionAutoPrint"));
  actionAutoPrint->setEnabled (true);
  actionAutoPrint->setIcon (iconAutoPrint);
  actionAutoPrint->setToolTip (QString::fromUtf8 ("Print PNG"));
  actionAutoPrint->setWhatsThis (QString::fromUtf8 ("Print PNG"));
  
  actionShowAxis = new QAction (toolBar);
  actionShowAxis->setObjectName (QString::fromUtf8 ("actionShowAxis"));
  actionShowAxis->setEnabled (false);
  actionShowAxis->setCheckable (true);
  actionShowAxis->setIcon (iconShowAxis);
  actionShowAxis->setToolTip (QString::fromUtf8 ("Show/Hide Axis"));
  actionShowAxis->setWhatsThis (QString::fromUtf8 ("Show/Hide Axis"));

  actionCameraToggle = new QAction (toolBar);
  actionCameraToggle->setObjectName (QString::fromUtf8 ("actionCameraToggle"));
  actionCameraToggle->setEnabled (false);
  actionCameraToggle->setCheckable (true);
  actionCameraToggle->setIcon (iconCameraToggle);
  actionCameraToggle->setToolTip (QString::fromUtf8 ("Change Projection"));
  actionCameraToggle->setWhatsThis (QString::fromUtf8 ("Selects the type of camera used\n"
						       "by the viewer. It toggles between\n"
						       "the two available camera types -\n"
						       "perspective and orthographic."));

  toolBar->addAction (actionSaveAs);
  toolBar->addAction (actionPrint);
  toolBar->addAction (actionConCen);
  toolBar->addAction (actionZoomIn);
  toolBar->addAction (actionZoomOut);
  toolBar->addAction (actionPick);
  toolBar->addAction (actionViewMode);
  toolBar->addAction (actionSeek);
  toolBar->addAction (actionHome);
  toolBar->addAction (actionSetHome);
  toolBar->addAction (actionViewAll);
  toolBar->addAction (actionYX);
  toolBar->addAction (actionXZ);
  toolBar->addAction (actionYZ);
  toolBar->addAction (actionAutoPrint);
  toolBar->addAction (actionShowAxis);
  toolBar->addAction (actionCameraToggle);
  toolBar->addAction (actionInvertCamera);
  toolBar->show ();

  QObject::connect (actionSaveAs, SIGNAL(triggered()), browser(), SLOT(save ()));
  QObject::connect (actionPrint, SIGNAL(triggered()), browser(), SLOT(print ()));
  QObject::connect (actionConCen, SIGNAL(triggered()), centre(), SLOT(show ()));
  QObject::connect (actionZoomIn, SIGNAL(triggered()), browser(), SLOT(zoomIn ()));
  QObject::connect (actionZoomOut, SIGNAL(triggered()), browser(), SLOT(zoomOut ()));
  QObject::connect (actionPick, SIGNAL(triggered()), this, SLOT(pick ()));
  QObject::connect (actionViewMode, SIGNAL(triggered()), this, SLOT(view ()));
  QObject::connect (actionViewMode, SIGNAL(toggled(bool)), actionSeek, SLOT(setEnabled(bool)));
  QObject::connect (actionSeek, SIGNAL(triggered()), browser(), SLOT(seek ()));
  QObject::connect (actionHome, SIGNAL(triggered()), browser(), SLOT(resetToHomePosition ()));
  QObject::connect (actionSetHome, SIGNAL(triggered()), browser(), SLOT(saveHomePosition ()));
  QObject::connect (actionViewAll, SIGNAL(triggered()), browser(), SLOT(viewAll ()));
  QObject::connect (actionInvertCamera, SIGNAL(triggered()), browser(), SLOT(invertCamera ()));
  QObject::connect (actionYX, SIGNAL(triggered()), browser (), SLOT(viewPlaneZ ()));
  QObject::connect (actionXZ, SIGNAL(triggered()), browser (), SLOT(viewPlaneY ()));
  QObject::connect (actionYZ, SIGNAL(triggered()), browser (), SLOT(viewPlaneX ()));
  QObject::connect (actionAutoPrint, SIGNAL(triggered()), browser(), SLOT(autoPrint ()));
  QObject::connect (actionShowAxis, SIGNAL(toggled (bool)), browser(), SLOT(setFeedbackVisibility (bool)));
  QObject::connect (actionCameraToggle, SIGNAL(triggered ()), browser(), SLOT(toggleCameraType ()));

  dynamic_cast<QMainWindow *>(IgQtAppWindowService::get (m_state)->mainWindow ())->addToolBar(toolBar);
}

void
Ig3DBaseWindow::showViewProperties (void)
{
  QString label (m_viewPropertiesCategory->catalogLabel ());
  m_centre->selectCategory (
    label.right (label.length ()- label.findRev ("/") - 1));
  m_centre->show ();
}

Ig3DBaseWindow::~Ig3DBaseWindow (void)
{
}

void
Ig3DBaseWindow::show (void)
{
  ASSERT (m_browser);
  ASSERT (m_browser->getWidget ());
  ASSERT (m_browser->getWidget ()->parentWidget ());
    
  m_browser->getWidget ()->parentWidget ()->show ();
  QMdiArea *ws = dynamic_cast <IgPage *>(m_site)->workspace ();
      
  ASSERT (ws);
  if (ws->activeSubWindow ())
  {	
    QWidget *pw = ws->activeSubWindow ()->parentWidget();
    if (!pw)
      pw = ws->activeSubWindow ();
      
    if (ws->subWindowList (QMdiArea::CreationOrder).count () == 1)
    {
      pw->resize (600, 600);
      pw->showMaximized ();
    }
    else if (ws)      
    {
      if (! pw->isMaximized ())
	pw->setGeometry (0, 0, ws->width (), ws->height ());
    }
    else
      pw->showMaximized ();
  }
  else
  {
    if (! ws->subWindowList ().empty ())
    {
      ws->setActiveSubWindow (ws->subWindowList ().first ());	    
    }	
  }
}

void
Ig3DBaseWindow::hide (void)
{
  ASSERT (m_browser);
  m_browser->getWidget ()->parentWidget ()->hide ();
}

IgState *
Ig3DBaseWindow::state (void)
{
  return m_state;
}

Ig3DBaseBrowser *
Ig3DBaseWindow::browser (void)
{
  return m_browser;
}

Ig3DBaseModel *
Ig3DBaseWindow::model (void)
{
  return m_model;
}

IgControlCentre * 
Ig3DBaseWindow::centre  (void)
{ return m_centre; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DBaseWindow::browse (IgRepresentable *object)
{
  ASSERT (m_browser);
  // FIXME: This should be replaced with some sort of automatic
  // registration of models into the view/page/document to get all
  // this done more automatically.
  m_browser->browse (object);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DBaseWindow::focusIn (void)
{ 
  ASSERT (IgQtObjectMenuService::get (state ()));
  ASSERT (m_browser);

  qDebug () << windowTitle () << "Ig3DBaseWindow::focusIn should add the Ig3DBaseBrowser::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  //     	->add (lat::CreateCallback (m_browser, &Ig3DBaseBrowser::repMenu));
  m_browser->focusIn ();
  m_centre->updateOnWindowActive ();
}

void
Ig3DBaseWindow::focusOut (void)
{
  ASSERT (IgQtObjectMenuService::get (state ()));
  ASSERT (m_browser);
    
  qDebug () << windowTitle () << "Ig3DBaseWindow::focusOut should remove the Ig3DBaseBrowser::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  //     	->remove (lat::CreateCallback (m_browser, &Ig3DBaseBrowser::repMenu));
  m_browser->focusOut ();
}
    
void
Ig3DBaseWindow::pick (void)
{
  m_browser->pick ();
}

void
Ig3DBaseWindow::view (void)
{
  m_browser->view ();
}

void
Ig3DBaseWindow::toolbarVisibilityChanged (bool visibility)
{
  if(visibility && hasFocus())
    toolBar->setVisible(visibility);
  else
    toolBar->setVisible(visibility);
}

void
Ig3DBaseWindow::print (const QString &text)
{
  QString t = QString::fromStdString (title ());
  t.append (QString ("\n"));    
  t.append (text);
    
  m_browser->autoPrint (t.toStdString ());
}
