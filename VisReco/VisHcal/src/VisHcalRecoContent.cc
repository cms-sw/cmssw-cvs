#include "VisReco/VisHcal/interface/VisHcalRecoContent.h"
#include "VisFramework/VisFrameworkBase/interface/VisExceptionService.h"
#include "VisReco/VisHcal/interface/VisHBHERecHitTwig.h"
#include "VisReco/VisHcal/interface/VisHFRecHitTwig.h"
#include "VisReco/VisHcal/interface/VisHORecHitTwig.h"
#include "VisReco/VisHcal/interface/VisHFDataFrameTwig.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisHcalRecoContent, "Data/Reco/Hcal");

/** Initialise the reconsructed HCAL data data proxy. */
VisHcalRecoContent::VisHcalRecoContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual initialisation: add the detector twig to the
    document data tree.  */
void
VisHcalRecoContent::init (void)
{    
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
  IgTwig *eventTwig = 0;

  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");
    
  eventTwig = topTwig->lookup ("CMS Event");
  if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "CMS Event");

  IgTwig *hcalEventTwig = new IgSimpleTwig (eventTwig, "HCAL Event");

  VisHBHERecHitTwig *hbheHits = new VisHBHERecHitTwig (state (), hcalEventTwig, "Hcal Barrel and Endcap RecHits");
  VisHFRecHitTwig *hfHits = new VisHFRecHitTwig (state (), hcalEventTwig, "Hcal Forward RecHits");
  VisHORecHitTwig *hoHits = new VisHORecHitTwig (state (), hcalEventTwig, "Hcal Outer RecHits", "", "", "", "", 0x0011ff00);

  VisHFDataFrameTwig *hfDataFrame = new VisHFDataFrameTwig (state (), hcalEventTwig, "Hcal Forward Digis");

  VisExceptionService *exService = VisExceptionService::get (state ());
  if (! exService)
  {
    exService = new VisExceptionService (state ());
  }
  hbheHits->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
  hbheHits->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
  hbheHits->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
  hbheHits->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));

  hfHits->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
  hfHits->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
  hfHits->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
  hfHits->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));

  hoHits->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
  hoHits->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
  hoHits->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
  hoHits->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));

  hfDataFrame->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
  hfDataFrame->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
  hfDataFrame->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
  hfDataFrame->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
}
