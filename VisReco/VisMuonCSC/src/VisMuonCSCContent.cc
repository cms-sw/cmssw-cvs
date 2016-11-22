#include "VisReco/VisMuonCSC/interface/VisMuonCSCContent.h"
#include "VisReco/VisMuonCSC/interface/VisCSCStripDigiTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCWireDigiTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCComparatorDigiTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCCorrelatedLCTDigiTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCALCTDigiTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCCLCTDigiTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCSegmentTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCRecHit2DTwig.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisMuonCSCContent, "Data/Reco/MuonCSC");

/** Initialise the reconsructed Muon CSC data data proxy. */
VisMuonCSCContent::VisMuonCSCContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the Muon CSC twigs to the
    document data tree.  */
void
VisMuonCSCContent::init (void)
{    
    LOG (0, trace, LFfwvis, "VisMuonCSCContent::init()\n");
    
    IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
    IgTwig *topTwig = 0;
    IgTwig *eventTwig = 0;

    topTwig = rootTwig->lookup ("CMS Event and Detector");
    if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

    eventTwig = topTwig->lookup ("CMS Event");
    if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "CMS Event");
    
    IgTwig *muonEventTwig = new IgSimpleTwig (eventTwig, "Muon CSC Event");

    new VisCSCStripDigiTwig (state (), muonEventTwig, "Strip digis");
    new VisCSCWireDigiTwig (state (), muonEventTwig, "Wire digis");
    new VisCSCComparatorDigiTwig (state (), muonEventTwig, "Comparator digis");
    new VisCSCCorrelatedLCTDigiTwig (state (), muonEventTwig, "Correlated LCT digis");
    new VisCSCALCTDigiTwig (state (), muonEventTwig, "ALCT digis");
    new VisCSCCLCTDigiTwig (state (), muonEventTwig, "CLCT digis");
    new VisCSCRecHit2DTwig (state (), muonEventTwig, "Rec Hit 2D");
    new VisCSCSegmentTwig (state (), muonEventTwig, "Segments");
}

