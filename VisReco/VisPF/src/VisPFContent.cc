#include "VisReco/VisPF/interface/VisPFContent.h"

#include "VisReco/VisPF/interface/VisPFClusterTwig.h"
#include "VisReco/VisPF/interface/VisPFRecHitTwig.h"
#include "VisReco/VisPF/interface/VisPFRecTrackTwig.h"
#include "VisReco/VisPF/interface/VisTrackingParticleTwig.h"
#include "VisReco/VisPF/interface/VisGsfPFRecTrackTwig.h"
 
#include "VisFramework/VisFrameworkBase/interface/VisExceptionService.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisPFContent, "Data/Reco/PF");

VisPFContent::VisPFContent (IgState *state)    
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

void VisPFContent::init (void)
{
    IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
    IgTwig *topTwig = 0;
    IgTwig *eventTwig = 0;
    
    topTwig = rootTwig->lookup ("CMS Event and Detector");
    if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

    eventTwig = topTwig->lookup ("CMS Event");
    if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "CMS Event");

    std::vector<std::string> twigs (3);

    twigs.push_back("VisPFClusterTwig");
    twigs.push_back("VisPFRecHitTwig");
    twigs.push_back("VisPFRecTrackTwig");
    twigs.push_back("VisTrackingParticleTwig");
    twigs.push_back("VisGsfPFRecTrackTwig");
    
    const edm::ParameterSet *pset = VisConfigurationService::pSet ();
    if (pset)
    {
	twigs = pset->getUntrackedParameter<std::vector<std::string> > ("VisPFContent", twigs);
    }

    VisExceptionService *exService = VisExceptionService::get (state ());
    if (! exService)
    {
	exService = new VisExceptionService (state ());
    }
   
    IgTwig *pfEventTwig = new IgSimpleTwig(eventTwig, "PF Event");

    if (std::find (twigs.begin (), twigs.end (), "VisPFClusterTwig")!= twigs.end()) 
    {
	VisPFClusterTwig* pfClusters = new VisPFClusterTwig(state(), pfEventTwig, "PF clusters");

	pfClusters->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
	pfClusters->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
	pfClusters->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
	pfClusters->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
    }
    
    if (std::find (twigs.begin (), twigs.end (), "VisPFRecHitTwig")!= twigs.end()) 
    {
	VisPFRecHitTwig* recHits = new VisPFRecHitTwig(state (), pfEventTwig, "PF rec hits");
	
	recHits->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
	recHits->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
	recHits->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
	recHits->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
    }

    if (std::find (twigs.begin (), twigs.end (), "VisPFRecTrackTwig")!= twigs.end()) 
    {
	VisPFRecTrackTwig* pfTracks = new VisPFRecTrackTwig(state (), pfEventTwig, "PF rec tracks");
	
	pfTracks->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
	pfTracks->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
	pfTracks->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
	pfTracks->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
    }

    if (std::find (twigs.begin (), twigs.end (), "VisTrackingParticleTwig")!= twigs.end()) 
    {
	VisTrackingParticleTwig* trackingPs = new VisTrackingParticleTwig(state (), pfEventTwig, "tracking particles");
	
	trackingPs->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
	trackingPs->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
	trackingPs->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
	trackingPs->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
    }

    if (std::find (twigs.begin (), twigs.end (), "VisGsfPFRecTrackTwig")!= twigs.end()) 
    {
	VisGsfPFRecTrackTwig* gsfRTs = new VisGsfPFRecTrackTwig(state (), pfEventTwig, "Gsf PF rec tracks");
	
	gsfRTs->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
	gsfRTs->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
	gsfRTs->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
	gsfRTs->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
    }
}
