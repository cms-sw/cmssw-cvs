#include "Iguana/GLBrowsers/interface/IgRZWindow.h"
#include "Iguana/GLModels/interface/IgRZModel.h"
#include "Iguana/GLModels/interface/Ig3DBaseRep.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/GLBrowsers/interface/IgRZBrowser.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtObjectMenu.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/GLBrowsers/interface/IgControlCentre.h"
#include "Iguana/GLBrowsers/interface/IgRZViewPropertiesCategory.h"
#include "Iguana/GLBrowsers/interface/Ig3DLightsCategory.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgModelManager.h"
#include "Iguana/Studio/interface/IgDocumentDataManager.h"
#include <QApplication>
#include <QDebug>
#include <QMenuBar>

IgRZWindow::IgRZWindow (IgPage *page)
  : Ig3DBaseWindow (page),
    m_rzViewPropertiesCategory (0),
    m_lightsCategory (0)
{   
  static int lastWindowNumber = 0;    
  IgRZModel *rzModel = 0;
  IgModelManager *manager = IgModelManager::get (page->state ());
  if (!manager)
    manager = new IgModelManager (page->state ());
  manager->lookup (&rzModel);
  if (!rzModel)
  {
    IgDocumentDataManager* dm = IgDocumentDataManager::get(page->state ());
    rzModel = new  IgRZModel (dm->lookup(0));
    manager->add (dm->lookup(0), rzModel);
  }
    
  QString t = "RZ Window #" + QString::number (lastWindowNumber++);
  IgView::setTitle (t.ascii ());   
    
  watchBrowser (new IgRZBrowser (state (), 
				 page, 
				 rzModel));
  initCategories ();
  initMenu ();
  initToolBar ();    

  show ();
}

IgRZWindow::~IgRZWindow (void)
{
}

void
IgRZWindow::initCategories (void)
{
  Ig3DBaseWindow::initCategories ();
    
  //Create a Lights Category and add it in control centre
  m_lightsCategory = Ig3DLightsCategory::get (page ()->state ());
  if (!m_lightsCategory)
  {
    m_lightsCategory = new Ig3DLightsCategory (page ()->state ());
  }    

  m_lightsCategory->registerBrowser (state (), browser ());

  // Create a RZ Window Properties Category and add it in control
  // centre
  m_rzViewPropertiesCategory = IgRZViewPropertiesCategory::get (page ()->state ());
    
  if (!m_rzViewPropertiesCategory)
  {
    m_rzViewPropertiesCategory = new IgRZViewPropertiesCategory (
      page ()->state (),
      dynamic_cast <IgRZBrowser *> (browser ()));
  }

  m_rzViewPropertiesCategory->registerBrowser (state (),
					       dynamic_cast <IgRZBrowser *> (browser ()));    
}

void
IgRZWindow::initMenu (void)
{
  Ig3DBaseWindow::initMenu ();    

  ASSERT (IgQtAppMenuService::get (state ()));
    
  // Get the menu as it has already been initialised in the
  // baseClass.

  QMenuBar *menubar = IgQtAppMenuService::get (state ())->menuBar ();    
  QMenu *menuView = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuView"));
  ASSERT (menuView);

  // Submenus
  if (menuView->findChild <QAction *> (QString::fromUtf8 ("actionRZProp")) == 0)
  {
    QAction *actionRZProp = new QAction (menuView);
    actionRZProp->setObjectName (QString::fromUtf8 ("actionRZProp"));
    actionRZProp->setEnabled (true);

    menuView->addAction (actionRZProp);
    QObject::connect (actionRZProp, SIGNAL(triggered()), this, SLOT(showRZViewProperties ()));
    actionRZProp->setText (QApplication::translate ("IgMainWindow", "&RZ View Properties...", 0, QApplication::UnicodeUTF8));
  }
}

void
IgRZWindow::focusIn (void)
{
  Ig3DBaseWindow::focusIn ();
    
  ASSERT (IgQtObjectMenuService::get (state ()));
  qDebug () << "IgRZWindow::focusIn should add the IgRZWindow::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  // 	->add (lat::CreateCallback (this, &IgRZWindow::repMenu));
}

void
IgRZWindow::focusOut (void)
{
  Ig3DBaseWindow::focusOut ();

  ASSERT (IgQtObjectMenuService::get (state ()));
  qDebug () << "IgRZWindow::focusOut should remove the IgRZWindow::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  // 	->remove (lat::CreateCallback (this, &IgRZWindow::repMenu));
}

void
IgRZWindow::showRZViewProperties (void)
{
  centre ()->selectCategory (m_rzViewPropertiesCategory);
  centre ()->show ();
}

void
IgRZWindow::showLights (void)
{
  centre ()->selectCategory (m_lightsCategory);
  centre ()->show ();
}

//////////////////////////////////////////////////////////////////////
void
IgRZWindow::repMenu (IgQtObjectMenuMessage message)
{
  static const IgQtObjectMenu::ItemDef defs [] = {
    { -1, Ig3DBaseBrowser::MENU_3D_OPS, 0, 0, -1 }
    //	{ -1, IgRZWindow::MENU_RZ_MANIP, "Manipulate", SLOT(repNode ()), -1 },
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
