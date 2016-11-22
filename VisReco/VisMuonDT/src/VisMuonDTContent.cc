#include "VisReco/VisMuonDT/interface/VisMuonDTContent.h"
#include "VisReco/VisMuonDT/interface/VisDTDigiTwig.h"
#include "VisReco/VisMuonDT/interface/VisDTRecHitsTwig.h"
#include "VisReco/VisMuonDT/interface/VisDT2DSegmentTwig.h"
#include "VisReco/VisMuonDT/interface/VisDT4DSegmentTwig.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisMuonDTContent, "Data/Reco/MuonDT");

/** Initialise the reconsructed Muon DT data data proxy. */
VisMuonDTContent::VisMuonDTContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the Muon data twigs to the
    document data tree.  */
void
VisMuonDTContent::init (void)
{    
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
  IgTwig *eventTwig = 0;
    
  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

  eventTwig = topTwig->lookup ("CMS Event");
  if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "CMS Event");

  std::string wname ("Wheel ");
  std::string stname ("Station ");
  std::string sename ("Sector ");

  IgTwig *muonEventTwig = new IgSimpleTwig (eventTwig, "Muon DT Event");
  new VisDTDigiTwig (state (), muonEventTwig, "DT Digis", "", "", "");
  new VisDTRecHitsTwig (state (), muonEventTwig, "DT Hits");
  new VisDT2DSegmentTwig (state (), muonEventTwig, "DT 2DSegments");
  new VisDT4DSegmentTwig (state (), muonEventTwig, "DT 4DSegments");
}
