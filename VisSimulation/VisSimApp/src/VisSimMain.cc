//<<<<<< INCLUDES                                                       >>>>>>

#include "VisSimulation/VisSimApp/src/VisSimMain.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "Iguana/GLBrowsers/interface/Ig3DShapeManager.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Studio/interface/IgDocument.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgMDIView.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Studio/interface/IgStudioDriver.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/Framework/interface/IgRepSet.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (VisSimMain, "Services/Studio/Session Types/CMSSW Simulation");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisSimMain::VisSimMain (IgState *state)
    : m_state (state),
      m_studio (0),
      m_document (0)
{ ASSERT (m_state); m_state->put (s_key, this);}


bool
VisSimMain::setup (IgStudioDriver *into)
{
    ASSERT (m_state);
    ASSERT (! m_studio);

    ASSERT (into);
    m_studio = into;

    // Create document
    ASSERT (! m_document);
    m_document = new IgDocument (m_studio);
    
    // Change window caption.
    m_document->setName ("CMSSW Simulation Visualisation");
    m_document->addContentProxy ("Framework/Core");

    const edm::ParameterSet *pset = VisConfigurationService::pSet ();
    if (! pset)
    {
        edm::ParameterSet p;
	VisConfigurationService *visServise = new VisConfigurationService (p);
	pset = visServise->pSet ();
	delete visServise;
    }
    std::vector<std::string> proxies;
    proxies = pset->getUntrackedParameter<std::vector<std::string> >
	      ("ContentProxies", proxies);

    // Fill document
    IgPage *page = new IgPage (m_document);
    new IgMDIView (page, "3D Window");

    // Load all data proxies
    if (proxies.empty ())
    {
	proxies.push_back ("Simulation/Core");
	proxies.push_back ("Simulation/Data");
	proxies.push_back ("Simulation/Geometry");
	proxies.push_back ("Simulation/Hits"); 
    }
    
    for (unsigned int i = 0; i < proxies.size (); i++)
    {
        m_document->addContentProxy (proxies [i]);
    }
    m_document->load ("Services/Framework/GUI/", true);

    IgRepSet::invalidate (m_document->data ()->root (), IgTwig::STRUCTURE_MASK);
    return true;
}
