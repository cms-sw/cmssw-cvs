#include "Iguana/GLBrowsers/interface/Ig2DWindow.h"
#include "Iguana/GLModels/interface/Ig2DModel.h"
#include "Iguana/GLBrowsers/interface/Ig3DSystem.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/GLBrowsers/interface/Ig2DBrowser.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtAppContextService.h"
#include "Iguana/Studio/interface/IgQtSiteStack.h"
#include "Iguana/GLBrowsers/interface/IgControlCentre.h"
#include "Iguana/GLBrowsers/interface/IgStudioCentre.h"
#include "Iguana/GLBrowsers/interface/Ig2DViewPropertiesCategory.h"
#include "Iguana/Studio/interface/IgModelManager.h"
#include "Iguana/Studio/interface/IgDocumentDataManager.h"

Ig2DWindow::Ig2DWindow (IgPage *page)
    : Ig3DBaseWindow (page),
      m_2dmodel (0)
{   
    static int lastWindowNumber = 0;
    IgModelManager *manager = IgModelManager::get (page->state ());
    if (!manager)
	manager = new IgModelManager (page->state ());
    manager->lookup (&m_2dmodel);
    if (!m_2dmodel)
    {
      IgDocumentDataManager* dm = IgDocumentDataManager::get(page->state ());
      m_2dmodel = new Ig2DModel (dm->lookup(0), model ());
      manager->add (dm->lookup(0), m_2dmodel);
    }
    
    QString t = "2D Window #" + QString::number (lastWindowNumber++);
    IgView::setTitle (t.ascii ());   
    
    watchBrowser (new Ig2DBrowser (state (), 
				   page, 
				   m_2dmodel));
    initCategories ();
    initMenu ();
    initToolBar ();    

    show ();
}

Ig2DWindow::~Ig2DWindow (void)
{
}

void
Ig2DWindow::initCategories (void)
{
    Ig3DBaseWindow::initCategories ();
    
    // Create a 2D Window Properties Category and add it in control
    // centre
    m_2DViewPropertiesCategory = Ig2DViewPropertiesCategory::get (page ()->state ());
    
    if (!m_2DViewPropertiesCategory)
    {
	m_2DViewPropertiesCategory = new Ig2DViewPropertiesCategory (
	    page ()->state (),
	    dynamic_cast <Ig2DBrowser *> (browser ()));
    }

    m_2DViewPropertiesCategory->registerBrowser (state (), browser ());    
}

void
Ig2DWindow::initMenu (void)
{
    Ig3DBaseWindow::initMenu ();    
}

void
Ig2DWindow::initToolBar (void)
{
    Ig3DBaseWindow::initToolBar ();    
}


void
Ig2DWindow::show2DViewProperties (void)
{
    centre ()->selectCategory (m_2DViewPropertiesCategory);
    centre ()->show ();
}
