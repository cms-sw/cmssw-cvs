#include "VisReco/VisMETReco/interface/VisMETRecoContent.h"
#include "VisReco/VisMETReco/interface/VisCaloMETTwig.h"
#include "VisReco/VisMETReco/interface/VisGenMETTwig.h"
#include "VisReco/VisMETReco/interface/VisMETTwig.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisMETRecoContent, "Data/Reco/MET");

/** Initialise the reconsructed MET data data proxy. */
VisMETRecoContent::VisMETRecoContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the MET twigs to the
    document data tree.  */
void
VisMETRecoContent::init (void)
{    
  LOG (0, trace, LFfwvis, "VisMETRecoContent::init()\n");

  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
  IgTwig *eventTwig = 0;
    
  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (topTwig, "CMS Event and Detector");

  eventTwig = topTwig->lookup ("CMS Event");
  if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "CMS Event");

  IgTwig *metEventTwig = new IgSimpleTwig (eventTwig, "MET data");

  new VisCaloMETTwig (state (), metEventTwig, "Calo MET");
  new VisGenMETTwig (state (), metEventTwig, "Gen MET");
  new VisMETTwig (state (), metEventTwig, "MET");
}
