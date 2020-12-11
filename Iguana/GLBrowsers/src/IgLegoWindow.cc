//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/IgLegoWindow.h"
#include "Iguana/GLModels/interface/IgLegoModel.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/GLBrowsers/interface/IgLegoBrowser.h"
#include "Iguana/GLBrowsers/interface/IgControlCentre.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtObjectMenu.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/GLBrowsers/interface/IgLegoViewPropertiesCategory.h"
#include "Iguana/GLBrowsers/interface/Ig3DLightsCategory.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Studio/interface/IgModelManager.h"
#include "Iguana/Studio/interface/IgDocumentDataManager.h"
#include <QApplication>
#include <QDebug>
#include <QMenuBar>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgLegoWindow::IgLegoWindow (IgPage *page)
  : Ig3DBaseWindow (page),
    m_legoViewPropertiesCategory (0),
    m_lightsCategory (0)
{   
  static int lastWindowNumber = 0;    
  IgLegoModel *legoModel = 0;
  IgModelManager *manager = IgModelManager::get (page->state ());
  if (!manager)
    manager = new IgModelManager (page->state ());
  manager->lookup (&legoModel);
  if (!legoModel)
  {
    IgDocumentDataManager* dm = IgDocumentDataManager::get(page->state ());
    legoModel = new  IgLegoModel (dm->lookup(0));
    manager->add (dm->lookup(0), legoModel);
  }
    
  QString t = "Lego Window #" + QString::number (lastWindowNumber++);
  IgView::setTitle (t.ascii ());   
    
  watchBrowser (new IgLegoBrowser (state (), 
				   page, 
				   legoModel));
  initCategories ();
  initMenu ();
  initToolBar ();    

  show ();
}

IgLegoWindow::~IgLegoWindow (void)
{
}

void
IgLegoWindow::initCategories (void)
{
  Ig3DBaseWindow::initCategories ();
    
  //Create a Lights Category and add it in control centre
  m_lightsCategory = Ig3DLightsCategory::get (page ()->state ());
  if (!m_lightsCategory)
  {
    m_lightsCategory = new Ig3DLightsCategory (page ()->state ());
  }    
  /*    m_lightsCategory = IgLegoLightsCategory::get (page ()->state ());
	if (!m_lightsCategory)
	{
	m_lightsCategory = new IgLegoLightsCategory (page ()->state ());
	}    
  */    
  m_lightsCategory->registerBrowser (state (), browser ());

  // Create a Lego Window Properties Category and add it in control
  // centre
  m_legoViewPropertiesCategory = IgLegoViewPropertiesCategory::get (page ()->state ());
    
  if (!m_legoViewPropertiesCategory)
  {
    m_legoViewPropertiesCategory = new IgLegoViewPropertiesCategory (
      page ()->state (),
      dynamic_cast <IgLegoBrowser *> (browser ()));
  }

  m_legoViewPropertiesCategory->registerBrowser (state (),
						 dynamic_cast <IgLegoBrowser *> (browser ()));    
}

void
IgLegoWindow::initMenu (void)
{
  Ig3DBaseWindow::initMenu ();    

  ASSERT (IgQtAppMenuService::get (state ()));
    
  // Get the menu as it has already been initialised in the
  // baseClass.

  QMenuBar *menubar = IgQtAppMenuService::get (state ())->menuBar ();    
  QMenu *menuView = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuView"));
  ASSERT (menuView);

  // Submenus
  if (menuView->findChild <QAction *> (QString::fromUtf8 ("actionLegoProp")) == 0)
  {
    QAction *actionLegoProp = new QAction (menuView);
    actionLegoProp->setObjectName (QString::fromUtf8 ("actionLegoProp"));
    actionLegoProp->setEnabled (true);

    menuView->addAction (actionLegoProp);
    QObject::connect (actionLegoProp, SIGNAL(triggered()), this, SLOT(showLegoViewProperties ()));
    actionLegoProp->setText (QApplication::translate ("IgMainWindow", "L&ego View Properties...", 0, QApplication::UnicodeUTF8));
  }
}

void
IgLegoWindow::focusIn (void)
{
  Ig3DBaseWindow::focusIn ();
    
  ASSERT (IgQtObjectMenuService::get (state ()));
  qDebug () << "IgLegoWindow::focusIn should add the IgLegoWindow::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  // 	->add (lat::CreateCallback (this, &IgLegoWindow::repMenu));
}

void
IgLegoWindow::focusOut (void)
{
  Ig3DBaseWindow::focusOut ();

  ASSERT (IgQtObjectMenuService::get (state ()));
  qDebug () << "IgLegoWindow::focusOut should remove the IgLegoWindow::repMenu callback.";
  // FIXME Qt4
  //     IgQtObjectMenuService::get (state ())
  // 	->remove (lat::CreateCallback (this, &IgLegoWindow::repMenu));
}

void
IgLegoWindow::initToolBar (void)
{
  Ig3DBaseWindow::initToolBar ();    
  actionInvertCamera->setEnabled(true);
  actionYX->setEnabled(true);
  actionXZ->setEnabled(true);
  actionYZ->setEnabled(true);
  actionCameraToggle->setEnabled(true);
}

void
IgLegoWindow::showLegoViewProperties (void)
{
  centre ()->selectCategory (m_legoViewPropertiesCategory);
  centre ()->show ();
}

void
IgLegoWindow::showLights (void)
{
  centre ()->selectCategory (m_lightsCategory);
  centre ()->show ();
}

//////////////////////////////////////////////////////////////////////
void
IgLegoWindow::repMenu (IgQtObjectMenuMessage message)
{
  static const IgQtObjectMenu::ItemDef defs [] = {
    { -1, Ig3DBaseBrowser::MENU_3D_OPS, 0, 0, -1 }
    //	{ -1, IgLegoWindow::MENU_LEGO_MANIP, "Manipulate", SLOT(repNode ()), -1 },
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
