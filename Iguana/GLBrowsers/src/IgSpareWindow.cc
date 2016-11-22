#include "Iguana/GLBrowsers/interface/IgSpareWindow.h"
#include "Iguana/GLModels/interface/IgSpareModel.h"
#include "Iguana/GLModels/interface/Ig3DBaseRep.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/GLBrowsers/interface/Ig3DLightsCategory.h"
#include "Iguana/GLBrowsers/interface/IgSpareBrowser.h"
#include "Iguana/Studio/interface/IgQtAppContextService.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtAppToolBarService.h"
#include "Iguana/Studio/interface/IgQtObjectMenu.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/Studio/interface/IgQtSiteStack.h"
#include "Iguana/GLBrowsers/interface/IgControlCentre.h"
#include "Iguana/GLBrowsers/interface/IgSpareViewPropertiesCategory.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgModelManager.h"
#include "Iguana/Studio/interface/IgDocumentDataManager.h"
#include <QApplication>
#include <QDebug>
#include <QtGui>

IgSpareWindow::IgSpareWindow (IgPage *page)
  : Ig3DBaseWindow (page),
    m_spareViewPropertiesCategory (0),
    m_lightsCategory (0)
{   
  static int lastWindowNumber = 0;    
  IgSpareModel *spareModel = 0;
  IgModelManager *manager = IgModelManager::get (page->state ());
  if (!manager)
    manager = new IgModelManager (page->state ());
  manager->lookup (&spareModel);
  if (!spareModel)
  {
    IgDocumentDataManager* dm = IgDocumentDataManager::get(page->state ());
    spareModel = new  IgSpareModel (dm->lookup(0));
    manager->add (dm->lookup(0), spareModel);
  }
    
  QString t = "Spare Window #" + QString::number (lastWindowNumber++);
  IgView::setTitle (t.ascii ());   
    
  watchBrowser (new IgSpareBrowser (state (), 
				    page, 
				    spareModel));
  initCategories ();
  initMenu ();
  initToolBar ();    

  show ();
}

IgSpareWindow::~IgSpareWindow (void)
{
}

void
IgSpareWindow::initCategories (void)
{
  Ig3DBaseWindow::initCategories ();
    
  //Create a Lights Category and add it in control centre
  m_lightsCategory = Ig3DLightsCategory::get (page ()->state ());
  if (!m_lightsCategory)
  {
    m_lightsCategory = new Ig3DLightsCategory (page ()->state ());
  }    
  m_lightsCategory->registerBrowser (state (), browser ());

  // Create a Spare Window Properties Category and add it in control
  // centre
  m_spareViewPropertiesCategory = IgSpareViewPropertiesCategory::get (page ()->state ());
    
  if (!m_spareViewPropertiesCategory)
  {
    m_spareViewPropertiesCategory = new IgSpareViewPropertiesCategory (
      page ()->state (),
      dynamic_cast <IgSpareBrowser *> (browser ()));
  }

  m_spareViewPropertiesCategory->registerBrowser (state (),
						  dynamic_cast <IgSpareBrowser *> (browser ()));    
}

void
IgSpareWindow::initMenu (void)
{
  Ig3DBaseWindow::initMenu ();    

  ASSERT (IgQtAppMenuService::get (state ()));
    
  // Get the menu as it has already been initialised in the
  // baseClass.

  QMenuBar *menubar = IgQtAppMenuService::get (state ())->menuBar ();    
  QMenu *menuView = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuView"));
  ASSERT (menuView);

  // Submenus
  if (menuView->findChild <QAction *> (QString::fromUtf8 ("actionSpareProp")) == 0)
  {
    QAction *actionSpareProp = new QAction (menuView);
    actionSpareProp->setObjectName (QString::fromUtf8 ("actionSpareProp"));
    actionSpareProp->setEnabled (true);

    menuView->addAction (actionSpareProp);
    QObject::connect (actionSpareProp, SIGNAL(triggered()), this, SLOT(showSpareViewProperties ()));
    actionSpareProp->setText (QApplication::translate ("IgMainWindow", "&Spare View Properties...", 0, QApplication::UnicodeUTF8));
  }
}

void
IgSpareWindow::focusIn (void)
{
  Ig3DBaseWindow::focusIn ();
    
  ASSERT (IgQtObjectMenuService::get (state ()));
  qDebug () << "IgSpareWindow::focusIn should add the IgSpareWindow::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  // 	->add (lat::CreateCallback (this, &IgSpareWindow::repMenu));
}

void
IgSpareWindow::focusOut (void)
{
  Ig3DBaseWindow::focusOut ();

  ASSERT (IgQtObjectMenuService::get (state ()));
  qDebug () << "IgSpareWindow::focusOut should remove the IgSpareWindow::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  // 	->remove (lat::CreateCallback (this, &IgSpareWindow::repMenu));
}

void
IgSpareWindow::initToolBar (void)
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
IgSpareWindow::showSpareViewProperties (void)
{
  centre ()->selectCategory (m_spareViewPropertiesCategory);
  centre ()->show ();
}

void
IgSpareWindow::showLights (void)
{
  centre ()->selectCategory (m_lightsCategory);
  centre ()->show ();
}

//////////////////////////////////////////////////////////////////////
void
IgSpareWindow::repMenu (IgQtObjectMenuMessage message)
{
  static const IgQtObjectMenu::ItemDef defs [] = {
    { -1, Ig3DBaseBrowser::MENU_3D_OPS, 0, 0, -1 }
  };
  static const int	ndefs = sizeof (defs) / sizeof (defs [0]);

  IgRepresentable	*object = message.object ();

  IgRep *m_menuRep = dynamic_cast<IgRep *>
		     (IgRepSet::lookup (object, model (), false));

  // An irrelevant object or already added item, remove our items.
  message.menu ()->removeFromDefs (defs, ndefs);
  if (m_menuRep)
    // Add our items if they aren't there yet.
    message.menu ()->insertFromDefs (this, defs, ndefs);
}
