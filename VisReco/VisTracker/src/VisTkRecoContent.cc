#include "VisReco/VisTracker/interface/VisTkRecoContent.h"
#include "VisReco/VisTracker/interface/VisTrackerPiDigiTwig.h"
#include "VisReco/VisTracker/interface/VisTrackerDigiTwig.h"
#include "VisReco/VisTracker/interface/VisTkRecTracksTwig.h"
#include "VisReco/VisTracker/interface/VisTkIdealHelixTracksTwig.h"
#include "VisReco/VisTracker/interface/VisTkRecTrackDetsTwig.h"
#include "VisReco/VisTracker/interface/VisTrajectorySeedTwig.h"
#include "VisReco/VisTracker/interface/VisTrackerClusterTwig.h"
#include "VisReco/VisTracker/interface/VisTrackerPiClusterTwig.h"
#include "VisReco/VisTracker/interface/VisTrackerPiRechitTwig.h"
#include "VisReco/VisTracker/interface/VisTrackerRechit2DTwig.h"
#include "VisReco/VisTracker/interface/VisTrackerRechit2DMatchedTwig.h"
#include "VisReco/VisTracker/interface/VisGsfTrackTwig.h"
#include "VisReco/VisTracker/interface/VisTrackingRecHitTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisExceptionService.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisTkRecoContent, "Data/Reco/Tracker");

/** Initialise the reconsructed Tracker detector data proxy. */
VisTkRecoContent::VisTkRecoContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the tracker detector twig to the
    document data tree.  */
void
VisTkRecoContent::init (void)
{    
  LOG (0, trace, LFfwvis, "VisTkRecoContent::init()\n");
    
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
  IgTwig *eventTwig = 0;

  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

  eventTwig = topTwig->lookup ("CMS Event");
  if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "CMS Event");
    
  std::vector<std::string> twigs (13);
  twigs.push_back ("VisTrackerPiDigiTwig");
  twigs.push_back ("VisTrackerPiClusterTwig");
  twigs.push_back ("VisTrackerPiRechitTwig");
  twigs.push_back ("VisTrackerDigiTwig");
  twigs.push_back ("VisTrackerClusterTwig");
  twigs.push_back ("VisTrackerRechit2DTwig");
  twigs.push_back ("VisTrackerRechit2DMatchedTwig");
  twigs.push_back ("VisTkRecTracksTwig");
  twigs.push_back ("VisTkIdealHelixTracksTwig");
  twigs.push_back ("VisTkRecTrackDetsTwig");
  twigs.push_back ("VisTrajectorySeedTwig");
  twigs.push_back ("VisGsfTrackTwig");
  twigs.push_back ("VisTrackingRecHitTwig");
    
  const edm::ParameterSet *pset = VisConfigurationService::pSet ();
  if (pset)
  {
    twigs = pset->getUntrackedParameter<std::vector<std::string> > ("VisTrackerContent", twigs);
  }

  VisExceptionService *exService = VisExceptionService::get (state ());
  if (! exService)
  {
    exService = new VisExceptionService (state ());
  }
    
  IgTwig *trackerEvent = new IgSimpleTwig (eventTwig, "Tracker Event");

  if (std::find (twigs.begin (), twigs.end (), "VisTrackerPiDigiTwig")!= twigs.end()) 
  {
    VisTrackerPiDigiTwig *pdigi = new VisTrackerPiDigiTwig (state (), trackerEvent, "Pixel Digis");
    pdigi->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    pdigi->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    pdigi->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    pdigi->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
	
  if (std::find (twigs.begin (), twigs.end (), "VisTrackerPiClusterTwig")!= twigs.end()) 
  {
    VisTrackerPiClusterTwig *pcluster = new VisTrackerPiClusterTwig (state (), trackerEvent, "Pixel Clusters");
    pcluster->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    pcluster->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    pcluster->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    pcluster->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
    
  if (std::find (twigs.begin (), twigs.end (), "VisTrackerPiRechitTwig")!= twigs.end()) 
  {
    VisTrackerPiRechitTwig *prechit = new VisTrackerPiRechitTwig (state (), trackerEvent, "Pixel RecHits");
    prechit->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    prechit->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    prechit->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    prechit->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
    
  if (std::find (twigs.begin (), twigs.end (), "VisTrackerDigiTwig")!= twigs.end()) 
  {
    VisTrackerDigiTwig *sdigi = new VisTrackerDigiTwig (state (), trackerEvent, "SiStrip Digis");
    sdigi->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    sdigi->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    sdigi->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    sdigi->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
    
  if (std::find (twigs.begin (), twigs.end (), "VisTrackerClusterTwig")!= twigs.end()) 
  {
    VisTrackerClusterTwig *scluster = new VisTrackerClusterTwig (state (), trackerEvent, "SiStrip Clusters");
    scluster->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    scluster->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    scluster->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    scluster->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
    
  if (std::find (twigs.begin (), twigs.end (), "VisTrackerRechit2DTwig")!= twigs.end()) 
  {
    VisTrackerRechit2DTwig *rechit2D = new VisTrackerRechit2DTwig (state (), trackerEvent, "SiStrip RecHit 2D");
    rechit2D->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    rechit2D->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    rechit2D->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    rechit2D->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
    
  if (std::find (twigs.begin (), twigs.end (), "VisTrackerRechit2DMatchedTwig")!= twigs.end()) 
  {
    VisTrackerRechit2DMatchedTwig *rechit2DMatched = new VisTrackerRechit2DMatchedTwig (state (), trackerEvent, "SiStrip RecHit 2D Matched");
    rechit2DMatched->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    rechit2DMatched->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    rechit2DMatched->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    rechit2DMatched->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
    
  if (std::find (twigs.begin (), twigs.end (), "VisTkRecTracksTwig")!= twigs.end()) 
  {
    VisTkRecTracksTwig *rectracks = new VisTkRecTracksTwig (state (), trackerEvent, "Reconstructed Tracks");
    rectracks->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    rectracks->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    rectracks->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    rectracks->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
    
  if (std::find (twigs.begin (), twigs.end (), "VisTkIdealHelixTracksTwig")!= twigs.end()) 
  {
    VisTkIdealHelixTracksTwig *recidealtracks = new VisTkIdealHelixTracksTwig (state (), trackerEvent, "Rec Ideal Tracks");
    recidealtracks->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    recidealtracks->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    recidealtracks->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    recidealtracks->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
    
  if (std::find (twigs.begin (), twigs.end (), "VisTkRecTrackDetsTwig")!= twigs.end()) 
  {
    VisTkRecTrackDetsTwig *rectrackdets = new VisTkRecTrackDetsTwig (state (), trackerEvent, "Rec Track Dets");
    rectrackdets->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    rectrackdets->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    rectrackdets->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    rectrackdets->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
    
  if (std::find (twigs.begin (), twigs.end (), "VisTrajectorySeedTwig")!= twigs.end()) 
  {
    VisTrajectorySeedTwig *seeds = new VisTrajectorySeedTwig (state (), trackerEvent, "Trajectory Seeds"); 
    seeds->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    seeds->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    seeds->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    seeds->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }  
    
  if (std::find (twigs.begin (), twigs.end (), "VisGsfTrackTwig")!= twigs.end()) 
  {
    VisGsfTrackTwig *gsftracks = new VisGsfTrackTwig (state (), trackerEvent, "Gsf Tracks"); 
    gsftracks->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    gsftracks->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    gsftracks->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    gsftracks->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }  
    
  if (std::find (twigs.begin (), twigs.end (), "VisTrackingRecHitTwig")!= twigs.end()) 
  {
    VisTrackingRecHitTwig *trechits = new VisTrackingRecHitTwig (state (), trackerEvent, "Tracking RecHits"); 
    trechits->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    trechits->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    trechits->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    trechits->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }  
}
