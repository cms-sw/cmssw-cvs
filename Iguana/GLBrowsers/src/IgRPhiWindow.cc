#include "Iguana/GLBrowsers/interface/IgRPhiWindow.h"
#include "Iguana/GLModels/interface/IgRPhiModel.h"
#include "Iguana/GLModels/interface/Ig3DBaseRep.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/GLBrowsers/interface/IgControlCentre.h"
#include "Iguana/GLBrowsers/interface/IgRPhiBrowser.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtObjectMenu.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/GLBrowsers/interface/IgRPhiViewPropertiesCategory.h"
#include "Iguana/GLBrowsers/interface/Ig3DLightsCategory.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgModelManager.h"
#include "Iguana/Studio/interface/IgDocumentDataManager.h"
#include <QApplication>
#include <QDebug>
#include <QMenuBar>

IgRPhiWindow::IgRPhiWindow (IgPage *page)
  : Ig3DBaseWindow (page),
    m_rphiViewPropertiesCategory (0),
    m_lightsCategory (0)
{   
  static int lastWindowNumber = 0;    
  IgRPhiModel *rphiModel = 0;
  IgModelManager *manager = IgModelManager::get (page->state ());
  if (!manager)
    manager = new IgModelManager (page->state ());
  manager->lookup (&rphiModel);
  if (!rphiModel)
  {
    IgDocumentDataManager* dm = IgDocumentDataManager::get(page->state ());
    rphiModel = new  IgRPhiModel (dm->lookup(0));
    manager->add (dm->lookup(0), rphiModel);
  }
    
  QString t = "RPhi Window #" + QString::number (lastWindowNumber++);
  IgView::setTitle (t.ascii ());   
    
  watchBrowser (new IgRPhiBrowser (state (), 
				   page, 
				   rphiModel));

  initCategories ();
  initMenu ();
  initToolBar ();    

  show ();
}

IgRPhiWindow::~IgRPhiWindow (void)
{
}

void
IgRPhiWindow::initCategories (void)
{
  Ig3DBaseWindow::initCategories ();
    
  //Create a Lights Category and add it in control centre
  m_lightsCategory = Ig3DLightsCategory::get (page ()->state ());
  if (!m_lightsCategory)
  {
    m_lightsCategory = new Ig3DLightsCategory (page ()->state ());
  }    
  m_lightsCategory->registerBrowser (state (), browser ());

  // Create a RPhi Window Properties Category and add it in control
  // centre
  m_rphiViewPropertiesCategory = IgRPhiViewPropertiesCategory::get (page ()->state ());
    
  if (!m_rphiViewPropertiesCategory)
  {
    m_rphiViewPropertiesCategory = new IgRPhiViewPropertiesCategory (
      page ()->state (),
      dynamic_cast <IgRPhiBrowser *> (browser ()));
  }

  m_rphiViewPropertiesCategory->registerBrowser (state (),
						 dynamic_cast <IgRPhiBrowser *> (browser ()));    
}

void
IgRPhiWindow::initMenu (void)
{
  Ig3DBaseWindow::initMenu ();    

  ASSERT (IgQtAppMenuService::get (state ()));
    
  // Get the menu as it has already been initialised in the
  // baseClass.

  QMenuBar *menubar = IgQtAppMenuService::get (state ())->menuBar ();    
  QMenu *menuView = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuView"));
  ASSERT (menuView);

  // Submenus
  if (menuView->findChild <QAction *> (QString::fromUtf8 ("actionRPhiProp")) == 0)
  {
    QAction *actionRPhiProp = new QAction (menuView);
    actionRPhiProp->setObjectName (QString::fromUtf8 ("actionRPhiProp"));
    actionRPhiProp->setEnabled (true);

    menuView->addAction (actionRPhiProp);
    QObject::connect (actionRPhiProp, SIGNAL(triggered()), this, SLOT(showRPhiViewProperties ()));
    actionRPhiProp->setText (QApplication::translate ("IgMainWindow", "&R-Phi View Properties...", 0, QApplication::UnicodeUTF8));
  }
}

void
IgRPhiWindow::focusIn (void)
{
  Ig3DBaseWindow::focusIn ();
    
  ASSERT (IgQtObjectMenuService::get (state ()));
  qDebug () << "IgRPhiWindow::focusIn should add the IgRPhiWindow::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  // 	->add (lat::CreateCallback (this, &IgRPhiWindow::repMenu));
}

void
IgRPhiWindow::focusOut (void)
{
  Ig3DBaseWindow::focusOut ();

  ASSERT (IgQtObjectMenuService::get (state ()));
  qDebug () << "IgRPhiWindow::focusOut should remove the IgRPhiWindow::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  // 	->remove (lat::CreateCallback (this, &IgRPhiWindow::repMenu));
}

void
IgRPhiWindow::showRPhiViewProperties (void)
{
  centre ()->selectCategory (m_rphiViewPropertiesCategory);
  centre ()->show ();
}

void
IgRPhiWindow::showLights (void)
{
  centre ()->selectCategory (m_lightsCategory);
  centre ()->show ();
}

//////////////////////////////////////////////////////////////////////
void
IgRPhiWindow::repMenu (IgQtObjectMenuMessage message)
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
