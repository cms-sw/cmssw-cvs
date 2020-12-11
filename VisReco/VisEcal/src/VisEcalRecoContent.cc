#include "VisReco/VisEcal/interface/VisEcalRecoContent.h"
#include "VisReco/VisEcal/interface/VisEcalUncalibratedRecHitTwig.h"
#include "VisReco/VisEcal/interface/VisEcalRecHitTwig.h"
#include "VisReco/VisEcal/interface/VisBasicClusterCollectionTwig.h"
#include "VisReco/VisEcal/interface/VisSuperClusterCollectionTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisExceptionService.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisEcalRecoContent, "Data/Reco/Ecal");

/** Initialise the reconsructed Ecal data data proxy. */
VisEcalRecoContent::VisEcalRecoContent (IgState *state)    
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the Ecal twigs to the
    document data tree.  */
void
VisEcalRecoContent::init (void)
{
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
    
  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

  std::vector<std::string> twigs (3);
  twigs.push_back ("VisEcalUncalibratedRecHitTwig");
  twigs.push_back ("VisEcalRecHitTwig");
  twigs.push_back ("VisBasicClusterCollectionTwig");
  twigs.push_back ("VisSuperClusterCollectionTwig");

  const edm::ParameterSet *pset = VisConfigurationService::pSet ();
  if (pset)
  {
    twigs = pset->getUntrackedParameter<std::vector<std::string> > ("VisEcalContent", twigs);
  }

  VisExceptionService *exService = VisExceptionService::get (state ());
  if (! exService)
  {
    exService = new VisExceptionService (state ());
  }
   
  IgTwig *ecalEventTwig = new IgSimpleTwig (topTwig, "ECAL Event");

  if (std::find (twigs.begin (), twigs.end (), "VisEcalUncalibratedRecHitTwig") != twigs.end ()) 
  {
    VisEcalUncalibratedRecHitTwig *ucRecHits = new VisEcalUncalibratedRecHitTwig (state (), ecalEventTwig, "Uncalibrated Rec Hits");
    ucRecHits->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    ucRecHits->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    ucRecHits->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    ucRecHits->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
    
  if (std::find (twigs.begin (), twigs.end (), "VisEcalRecHitTwig") != twigs.end ()) 
  {
    VisEcalRecHitTwig *recHits = new VisEcalRecHitTwig (state (), ecalEventTwig, "Rec Hits");
    recHits->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    recHits->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    recHits->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    recHits->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
  if (std::find (twigs.begin (), twigs.end (), "VisBasicClusterCollectionTwig") != twigs.end ()) 
  {
    VisBasicClusterCollectionTwig *bclustres = new VisBasicClusterCollectionTwig (state (), ecalEventTwig, "Basic Clusters");
    bclustres->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    bclustres->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    bclustres->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    bclustres->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
  if (std::find (twigs.begin (), twigs.end (), "VisSuperClusterCollectionTwig") != twigs.end ()) 
  {
    VisSuperClusterCollectionTwig *sclustres = new VisSuperClusterCollectionTwig (state (), ecalEventTwig, "Super Clusters");
    sclustres->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
    sclustres->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
    sclustres->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
    sclustres->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
  }
}
