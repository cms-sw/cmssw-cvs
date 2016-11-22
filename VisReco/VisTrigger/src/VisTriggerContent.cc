#include "VisReco/VisTrigger/interface/VisTriggerContent.h"
#include "VisReco/VisTrigger/interface/VisTriggerResultsTwig.h"
#include "VisReco/VisTrigger/interface/VisL1MuDTChambPhContainerTwig.h"
#include "VisReco/VisTrigger/interface/VisL1MuDTChambThContainerTwig.h"
#include "VisReco/VisTrigger/interface/VisL1MuDTTrackContainerTwig.h"
#include "VisReco/VisTrigger/interface/VisL1GlobalTriggerReadoutRecordTwig.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisTriggerContent, "Data/Reco/Trigger");

/** Initialise the Trigger data data proxy. */
VisTriggerContent::VisTriggerContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the Trigger twigs to the
    document data tree.  */
void
VisTriggerContent::init (void)
{
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
  IgTwig *eventTwig = 0;
    
  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

  eventTwig = topTwig->lookup ("CMS Event");
  if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "CMS Event");
    
  IgTwig *triggerTwig = new IgSimpleTwig (eventTwig, "Trigger");
  new VisTriggerResultsTwig (state (), triggerTwig, "Trigger Results");
  new VisL1MuDTChambPhContainerTwig (state (), triggerTwig, "L1 Muon DT Ph");
  new VisL1MuDTChambThContainerTwig (state (), triggerTwig, "L1 Muon DT Th");
  new VisL1MuDTTrackContainerTwig (state (), triggerTwig, "L1 Muon DT Tracks");
  new VisL1GlobalTriggerReadoutRecordTwig (state (), triggerTwig, "L1GlobalTriggerReadoutRecord");
}
