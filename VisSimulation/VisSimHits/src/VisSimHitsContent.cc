#include "VisSimulation/VisSimHits/interface/VisSimHitsContent.h"
#include "VisSimulation/VisSimHits/interface/VisPCaloHitTwig.h"
#include "VisSimulation/VisSimHits/interface/VisPSimHitTwig.h"
#include "VisSimulation/VisSimHits/interface/VisSimTrackTwig.h"
#include "VisSimulation/VisSimHits/interface/VisSimVertexTwig.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisFramework/VisFrameworkBase/interface/VisExceptionService.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgQtLock.h"

IG_DEFINE_STATE_ELEMENT (VisSimHitsContent, "Data/Simulation/Hits");

/** Initialise the simulated hits data data proxy. */
VisSimHitsContent::VisSimHitsContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the sim hits twigs to the
    document data tree.  */
void
VisSimHitsContent::init (void)
{
    LOG (0, trace, LFfwvis, "VisSimHitsContent::init()\n");
    
    IgQtLock ();
    
    IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
    IgTwig *topTwig = 0;
    IgTwig *eventTwig = 0;

    topTwig = rootTwig->lookup ("CMS Event and Detector");
    if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

    eventTwig = topTwig->lookup ("CMS Event");
    if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "CMS Event");

    VisExceptionService *exService = VisExceptionService::get (state ());
    if (! exService)
    {
	exService = new VisExceptionService (state ());
    }
    
    std::vector<std::string> twigs (4);
    twigs.push_back ("VisPCaloHitTwig");
    twigs.push_back ("VisPSimHitTwig");
    twigs.push_back ("VisSimTrackTwig");
    twigs.push_back ("VisSimVertexTwig");

    const edm::ParameterSet *pset = VisConfigurationService::pSet ();
    if (pset)
    {
	twigs = pset->getUntrackedParameter<std::vector<std::string> > ("VisSimContent", twigs);
    }

    IgTwig *hitsEventTwig = new IgSimpleTwig (eventTwig, "Simulated Data");
    if (std::find (twigs.begin (), twigs.end (), "VisPCaloHitTwig")!= twigs.end()) 
    {
        VisPCaloHitTwig *caloHits = new VisPCaloHitTwig (state (), hitsEventTwig, "Sim Calo Hits");
	caloHits->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
	caloHits->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
	caloHits->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
	caloHits->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
    }
    if (std::find (twigs.begin (), twigs.end (), "VisPSimHitTwig")!= twigs.end()) 
    {
        VisPSimHitTwig *simHits = new VisPSimHitTwig (state (), hitsEventTwig, "Sim Hits");
	simHits->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
	simHits->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
	simHits->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
	simHits->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
    }
    if (std::find (twigs.begin (), twigs.end (), "VisSimTrackTwig")!= twigs.end()) 
    {
        VisSimTrackTwig *tracks = new VisSimTrackTwig (state (), hitsEventTwig, "Sim Tracks");
	tracks->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
	tracks->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
	tracks->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
	tracks->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
    }    
    if (std::find (twigs.begin (), twigs.end (), "VisSimVertexTwig")!= twigs.end()) 
    {
        VisSimVertexTwig *vtx = new VisSimVertexTwig (state (), hitsEventTwig, "Sim Vertices");    
	vtx->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
	vtx->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
	vtx->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
	vtx->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
    }
}
