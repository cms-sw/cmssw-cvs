//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/Ig3DWindow.h"
#include "Iguana/GLModels/interface/Ig3DModel.h"
#include "Iguana/GLModels/interface/Ig3DBaseModel.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/GLBrowsers/interface/Ig3DBrowser.h"
#include "Iguana/GLBrowsers/interface/Ig3DLightsCategory.h"
#include "Iguana/GLBrowsers/interface/Ig3DSlicersCategory.h"
#include "Iguana/GLBrowsers/interface/Ig3DClipsCategory.h"
#include "Iguana/GLBrowsers/interface/Ig3DAnimsCategory.h"
#include "Iguana/GLBrowsers/interface/Ig3DNodeCategory.h"
#include "Iguana/GLBrowsers/interface/Ig3DViewpointsCategory.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "Iguana/Studio/interface/IgQtObjectMenu.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Studio/interface/IgQtAppToolBarService.h"
#include "Iguana/GLBrowsers/interface/IgControlCentre.h"
#include "Iguana/GLModels/interface/Ig3DBaseRep.h"
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <classlib/utils/Callback.h>
#include <QApplication>
#include <QDebug>
#include <QtGui>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DWindow::Ig3DWindow (IgPage *page)
  : Ig3DBaseWindow (page),
    m_slicersCategory (0),
    m_clipsCategory (0),
    m_lightsCategory (0),
    m_materialsCategory (0),
    m_animsCategory (0),
    m_viewpointsCategory (0),
    m_nodeCategory (0)
{
  static int lastWindowNumber = 0;
  QString t = "3D Window #" + QString::number (lastWindowNumber++);
  setTitle (t.ascii ());

  Ig3DBrowser *b = new Ig3DBrowser (state (), 
				    page, 
				    dynamic_cast<Ig3DModel *> (model ()));
  watchBrowser (b);

  // Show myself and show all the document data (FIXME: select?).    
  initCategories ();
  // Make sure that this is called AFTER we set up a browser for this window.
  initMenu ();    
  initToolBar ();

  show ();
}

Ig3DWindow::~Ig3DWindow (void)
{
}

void
Ig3DWindow::initCategories (void)
{
  Ig3DBaseWindow::initCategories ();
    
  //Create a Lights Category and add it in control centre
  m_lightsCategory = Ig3DLightsCategory::get (page ()->state ());
  if (!m_lightsCategory)
  {
    m_lightsCategory = new Ig3DLightsCategory (page ()->state ());
  }    
    
  //Create a Slicer Category and add it in control centre
  m_slicersCategory = Ig3DSlicersCategory::get (page ()->state ());
  if (!m_slicersCategory)
  {
    m_slicersCategory = new Ig3DSlicersCategory (page ()->state (),
						 browser ());
  }

  //Create the Clipplane category and add it in control centre
  m_clipsCategory = Ig3DClipsCategory::get (page ()->state ());
  if (!m_clipsCategory)
  {
    m_clipsCategory = new Ig3DClipsCategory (page ()->state (),
					     browser ());
  }  
	
  //Create the Animator category and add it in control centre

  m_animsCategory = Ig3DAnimsCategory::get (page ()->state ());
  if (!m_animsCategory)
  {
    m_animsCategory = new Ig3DAnimsCategory (page ()->state (),
					     browser ());
  }
    
  // Create the View Points category and add it in control centre
  m_viewpointsCategory = Ig3DViewpointsCategory::get (page ()->state ());
  if (!m_viewpointsCategory)
  {
    m_viewpointsCategory = new Ig3DViewpointsCategory(page ()->state (),
						      dynamic_cast<Ig3DBrowser *> (browser ()));
  }

  // Create the category to control shapes
  m_nodeCategory = Ig3DNodeCategory::get (page ()->state ());
  if (!m_nodeCategory)
  {
    m_nodeCategory = new Ig3DNodeCategory(page ()->state (), browser ());
  }
	
  m_lightsCategory->registerBrowser (state (), browser ());
  m_slicersCategory->registerBrowser (state (), browser ());
  m_clipsCategory->registerBrowser (state (), browser ());
  m_animsCategory->registerBrowser (state (), browser ());
  m_viewpointsCategory->registerBrowser (state (), browser ());
  m_nodeCategory->registerBrowser (state (), browser ());
}

void
Ig3DWindow::initMenu (void)
{
  // NB: We need to make sure that the popup menu is already there.
  // This is done by calling the base class method.
  Ig3DBaseWindow::initMenu ();
    
  ASSERT (IgQtAppMenuService::get (state ()));

  // Get the menu as it has already been initialised in the
  // baseClass.

  QMenuBar *menubar = IgQtAppMenuService::get (state ())->menuBar ();    
  QMenu *menuView = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuView"));
  ASSERT (menuView);
    
  // Submenus. First we check if the first item of the submenu is
  // already there, if it is, we don't create any item, as we as
  // assume that all of them are already there. Viceversa, we create
  // all of them if the first one is not found.
  if (menuView->findChild <QAction *> (QString::fromUtf8 ("actionSlices")) == 0)
  {
    QAction *actionSlices = new QAction (menuView);
    actionSlices->setObjectName (QString::fromUtf8 ("actionSlices"));
    actionSlices->setEnabled (true);

    QAction *actionClips = new QAction (menuView);
    actionClips->setObjectName (QString::fromUtf8 ("actionClips"));
    actionClips->setEnabled (true);

    QAction *actionLights = new QAction (menuView);
    actionLights->setObjectName (QString::fromUtf8 ("actionLights"));
    actionLights->setEnabled (true);

    QAction *actionAnimations = new QAction (menuView);
    actionAnimations->setObjectName (QString::fromUtf8 ("actionAnimations"));
    actionAnimations->setEnabled (true);

    QAction *actionViewpoints = new QAction (menuView);
    actionViewpoints->setObjectName (QString::fromUtf8 ("actionViewpoints"));
    actionViewpoints->setEnabled (true);

    QAction *actionManipulator = new QAction (menuView);
    actionManipulator->setObjectName (QString::fromUtf8 ("actionManipulator"));
    actionManipulator->setEnabled (true);

    menuView->addAction (actionSlices);
    menuView->addAction (actionClips);
    menuView->addAction (actionLights);
    menuView->addAction (actionAnimations);
    menuView->addAction (actionViewpoints);
    menuView->addAction (actionManipulator);

    QObject::connect (actionSlices, SIGNAL(triggered()), this, SLOT(showSlicers ()));
    QObject::connect (actionClips, SIGNAL(triggered()), this, SLOT(showClips ()));
    QObject::connect (actionLights, SIGNAL(triggered()), this, SLOT(showLights ()));
    QObject::connect (actionAnimations, SIGNAL(triggered()), this, SLOT(showAnims ()));
    QObject::connect (actionViewpoints, SIGNAL(triggered()), this, SLOT(showViewpoints ()));
    QObject::connect (actionManipulator, SIGNAL(triggered()), this, SLOT(showNodeManip ()));

    actionSlices->setText (QApplication::translate ("IgMainWindow", "&Slices...", 0, QApplication::UnicodeUTF8));
    actionClips->setText (QApplication::translate ("IgMainWindow", "&Clips...", 0, QApplication::UnicodeUTF8));
    actionLights->setText (QApplication::translate ("IgMainWindow", "&Lights...", 0, QApplication::UnicodeUTF8));
    actionAnimations->setText (QApplication::translate ("IgMainWindow", "&Animations...", 0, QApplication::UnicodeUTF8));
    actionViewpoints->setText (QApplication::translate ("IgMainWindow", "&Viewpoints...", 0, QApplication::UnicodeUTF8));
    actionManipulator->setText (QApplication::translate ("IgMainWindow", "&Node Manipulator...", 0, QApplication::UnicodeUTF8));
  }
}

void
Ig3DWindow::focusIn (void)
{
  Ig3DBaseWindow::focusIn ();
    
  ASSERT (IgQtObjectMenuService::get (state ()));
  qDebug () << windowTitle () << "Ig3DWindow::focusIn should add the Ig3DWindow::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  // 	->add (lat::CreateCallback (this, &Ig3DWindow::repMenu));
}

void
Ig3DWindow::focusOut (void)
{
  Ig3DBaseWindow::focusOut ();

  ASSERT (IgQtObjectMenuService::get (state ()));
  qDebug () << windowTitle () << "Ig3DWindow::focusOut should remove the Ig3DWindow::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  // 	->remove (lat::CreateCallback (this, &Ig3DWindow::repMenu));
}

void
Ig3DWindow::initToolBar (void)
{
  Ig3DBaseWindow::initToolBar ();
  actionInvertCamera->setEnabled(true);
  actionYX->setEnabled(true);
  actionXZ->setEnabled(true);
  actionYZ->setEnabled(true);
  actionShowAxis->setEnabled(true);
  actionCameraToggle->setEnabled(true);
}

void
Ig3DWindow::showClips (void)
{
  centre ()->selectCategory (m_clipsCategory);
  centre ()->show ();
}

void
Ig3DWindow::showSlicers (void)
{
  centre ()->selectCategory (m_slicersCategory);
  centre ()->show ();
}

void
Ig3DWindow::showLights (void)
{
  centre ()->selectCategory (m_lightsCategory);
  centre ()->show ();
}

void
Ig3DWindow::showAnims (void)
{
  centre ()->selectCategory (m_animsCategory);
  centre ()->show ();
}

void
Ig3DWindow::showViewpoints (void)
{
  centre ()->selectCategory (m_viewpointsCategory);
  centre ()->show ();
}

void
Ig3DWindow::showNodeManip (void)
{
  centre ()->selectCategory (m_nodeCategory);
  centre ()->show ();
}

//////////////////////////////////////////////////////////////////////
void
Ig3DWindow::repMenu (IgQtObjectMenuMessage message)
{
  static const IgQtObjectMenu::ItemDef defs [] = {
    { -1, Ig3DBaseBrowser::MENU_3D_OPS, 0, 0, -1 },
    { -1, Ig3DBaseBrowser::MENU_SLICE,  "Slice", SLOT(repSlice()), -1 },
    { -1, Ig3DBaseBrowser::MENU_CLIP,   "Clip", SLOT(repClip()), -1 },
  };
  static const int	ndefs = sizeof (defs)/sizeof (defs [0]);

  IgRepresentable	*object = message.object ();

  IgRep *m_menuRep = dynamic_cast<IgRep *>
		     (IgRepSet::lookup (object, model (), false));

  // An irrelevant object or already added item, remove our items.
  message.menu ()->removeFromDefs (defs, ndefs);
  if (m_menuRep)
    // Add our items if they aren't there yet.
    message.menu ()->insertFromDefs (this, defs, ndefs);
}

//////////////////////////////////////////////////////////////////////
void
Ig3DWindow::repSlice (void)
{
  ASSERT (browser ()->menuRep ());
  ASSERT (m_slicersCategory);
  m_slicersCategory->makeOne (browser ()->menuRep ());
}

void
Ig3DWindow::repClip (void)
{
  ASSERT (browser ()->menuRep ());
  ASSERT (m_clipsCategory);
  m_clipsCategory->makeOne (browser ()->menuRep ());
}

//////////////////////////////////////////////////////////////////////
void
Ig3DWindow::printViews (void)
{
  static SoNode * node = 0;
  if (! node)
  {
    node = browser ()->openNode (Ig3DBaseModel::encode
				 ("Default View Points").getString (),
				 "Select viewpoints to load",
				 browser ()->getWidget ()->parentWidget ()); 
  }
    
  if (node)
  {
    ASSERT (node->isOfType (SoGroup::getClassTypeId ()));
    SoGroup *views = static_cast<SoGroup *> (node);
    IgSoViewpoint *currentView = new IgSoViewpoint ();
    Ig3DViewpointsCategory::setViewpoint (browser (), currentView);
    for (int i = 1, n = views->getNumChildren (); i < n; ++i)
    {
      ASSERT (views->getChild (i)->isOfType (IgSoViewpoint::getClassTypeId ()));
      IgSoViewpoint *obj = static_cast<IgSoViewpoint *>(views->getChild (i));
      Ig3DViewpointsCategory::setCamera (browser (), obj);
      QString filename = obj->getName ().getString ();
	    
      browser ()->printVector (filename + ".pdf", "pdf", 2);
      browser ()->printBitmap (filename + ".jpg",
			       SoOffscreenRenderer::getScreenPixelsPerInch (),
			       300, "jpg");
    }
    Ig3DViewpointsCategory::setCamera (browser (), currentView);
  }
}
