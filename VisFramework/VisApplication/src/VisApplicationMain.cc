#define QT_NO_EMIT
#include "VisFramework/VisApplication/src/VisApplicationMain.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "Iguana/Framework/interface/IgDocumentDataRoot.h"
#include "Iguana/GLBrowsers/interface/Ig3DShapeManager.h"
#include "Iguana/Studio/interface/IgDocument.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Studio/interface/IgViewDB.h"
#include "Iguana/Studio/interface/IgView.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include "Iguana/Framework/interface/IgBrowser.h"
#include "Iguana/Framework/interface/IgBrowserDB.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <QApplication>
#include <QMdiArea>
#include <QTimer>
#include <classlib/utils/DebugAids.h>
#include <iostream>

IG_DEFINE_STATE_ELEMENT (VisApplicationMain, "Services/Studio/Session Types/CMSSW");

VisApplicationMain::VisApplicationMain (IgState *state)
  : m_state (state),
    m_studio (0),
    m_document (0)
{
  ASSERT (m_state);
  m_state->put (s_key, this);
}

bool
VisApplicationMain::setup (IgStudioDriver *into)
{
  ASSERT (m_state);
  ASSERT (! m_studio);

  ASSERT (into);
  m_studio = into;

  // Try to instantiate it
  QApplication::setOverrideCursor (Qt::waitCursor);
  IgQtAppStatusBarService::get (m_state)
    ->setMessage ("Loading CMSSW Application...");
    
  Ig3DShapeManager *shapeManager = Ig3DShapeManager::get (m_state);
  if (! shapeManager) shapeManager = new Ig3DShapeManager (m_state);
  ASSERT (shapeManager);
    
  // Create document
  ASSERT (! m_document);
  m_document = new IgDocument (m_studio);
  m_document->setName ("CMSSW Visualisation");
  IgDocumentDataRoot::currentRoot ("CMSSW");

  // Fill a document
  IgPage *page = new IgPage (m_document);

  // Load Framework core data proxy first
  // which are always needed and MUST be before 
  // VisConfigurationService accesses the ParameterSet
  m_document->addContentProxy ("Framework/Core");

  const edm::ParameterSet *pset = VisConfigurationService::pSet ();
  if(! pset)
  {
    edm::ParameterSet p;
    VisConfigurationService *visServise = new VisConfigurationService (p);
    pset = visServise->pSet ();
    delete visServise;
  }

  std::vector<IgBrowser *> browserDB;
  bool flag = true;
    
  // Checking for Twig Browser
  flag = pset->getUntrackedParameter<bool> ("TwigBrowser", true);

  if (flag)
  {
    IgBrowser *fullTreeBrowser = IgBrowserDB::get()->create ("Twig", page->state (), page);
    ASSERT (fullTreeBrowser);

    IgTwig *root = IgDocumentData::get (m_document->state ())->root ("", true);
    ASSERT (root);
    new IgSimpleTwig (root, "CMS Event and Detector");

    fullTreeBrowser->browse (root);
  }
    
  // Checking for Text Browser
  flag = pset->getUntrackedParameter<bool> ("TextBrowser", true);
  if (flag)
  {
    browserDB.push_back (IgBrowserDB::get()->create ("Text", page->state (), page));
  } 
    
  IgQtSite::selfFrom (page)->show ();
    
  std::vector<std::string> views;
  views = pset->getUntrackedParameter<std::vector<std::string> > ("Views", views);

  // By default
  if (views.empty ()) 
  {
    views.push_back ("RPhi Window");
  }
    
  for (unsigned int i = 0; i < views.size (); i++)
  {
    //std::cout << "View:" << views [i] << "." << std::endl;
    IgBrowser *browser = dynamic_cast<IgBrowser *>(IgViewDB::get ()->create (views [i], page));
	
    ASSERT (browser);
    browserDB.push_back (browser);
  }
    
  IgRepresentable *obj = IgDocumentData::get (page->state ())->root ();
  for (unsigned int i = 0; i < browserDB.size (); i++)
    browserDB [i]->browse (obj);
    
  std::vector<std::string> proxies;
  proxies = pset->getUntrackedParameter<std::vector<std::string> >
	    ("ContentProxies", proxies);

  // Always load the dynamic event content proxy.
  // It prints also run and event number on the status bar.
  m_document->addContentProxy ("Event/Core Content");
    
  // When there is no proxies defined in 
  // the parameter set we'll load all of them:
  if (proxies.empty ())
  {
    proxies.push_back ("Framework/EventSetup");
    proxies.push_back ("Reco/MuonCSC");
    proxies.push_back ("Reco/MuonRPC");
    proxies.push_back ("Reco/MuonDT");
    proxies.push_back ("Reco/Ecal");
    proxies.push_back ("Reco/Hcal");
    proxies.push_back ("Reco/Jets");
    proxies.push_back ("Reco/Tracker");
    proxies.push_back ("Reco/Muon");
    proxies.push_back ("Reco/Detector");
    proxies.push_back ("Reco/CMS Magnetic Field");

    // 	proxies.push_back ("Simulation/Data Formats");
    // 	proxies.push_back ("Simulation/Geometry");
    //	proxies.push_back ("Simulation/Hits"); 
  }
    
  for (unsigned int i = 0; i < proxies.size (); i++)
  {
    m_document->addContentProxy (proxies [i]);
  }
    
  std::vector<std::string> services;
  services.push_back ("Services/Framework/GUI/");
  services = pset->getUntrackedParameter<std::vector<std::string> >
	     ("Services", services);
  for (unsigned int i = 0; i < services.size (); i++)
  {
    std::string service = services [i];
    if (service.size () == 0) continue;
    if (service [service.size () - 1] == '/')
      m_document->load (service, true);
    else
      m_document->load (service, false);
  }

  IgRepSet::invalidate (m_document->data ()->root (), IgTwig::STRUCTURE_MASK);
    
  std::string appMode = "normal";
  appMode = pset->getUntrackedParameter<std::string> ("VisAppMode", appMode);
  if (appMode == "maximized")
    page->showMaximized ();
  else if (appMode == "full screen")
    page->showFullScreen ();
    
  QMdiArea *ws = page->workspace ();
  QTimer::singleShot (500, ws, SLOT (tileSubWindows ()));
    
  IgQtAppStatusBarService::get (m_state)
    ->setMessage ("CMSSW Application is loaded.");

  QApplication::restoreOverrideCursor ();

  return true;
}
