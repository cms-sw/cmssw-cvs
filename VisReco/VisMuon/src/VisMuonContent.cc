#include "VisReco/VisMuon/interface/VisMuonContent.h"
#include "VisReco/VisMuon/interface/VisMuonTwig.h"
#include "VisReco/VisMuon/interface/VisTrackTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisExceptionService.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisMuonContent, "Data/Reco/Muon");

VisMuonContent::VisMuonContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the Muon data twigs to the
    document data tree.  */
void
VisMuonContent::init (void)
{    
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
  IgTwig *eventTwig = 0;
    
  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

  eventTwig = topTwig->lookup ("Event");
  if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "Event");
   
  VisExceptionService *exService = VisExceptionService::get (state ());
  if (! exService)
  {
    exService = new VisExceptionService (state ());
  }

  VisTrackTwig *tracks = new VisTrackTwig (state (), eventTwig, "Tracks");    
  tracks->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
  tracks->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
  tracks->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
  tracks->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));

  VisMuonTwig *muons = new VisMuonTwig (state (), eventTwig, "Muons");
  muons->onException (lat::CreateCallback (exService, &VisExceptionService::exceptionCallback));
  muons->onError (lat::CreateCallback (exService, &VisExceptionService::errorCallback));
  muons->onCmsException (lat::CreateCallback (exService, &VisExceptionService::cmsExceptionCallback));
  muons->onUnhandledException (lat::CreateCallback (exService, &VisExceptionService::unhandledExceptionCallback));
}
