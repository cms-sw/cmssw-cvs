#include "VisReco/VisMuonRPC/interface/VisRPCContent.h"
#include "VisReco/VisMuonRPC/interface/VisRPCDigiTwig.h"
#include "VisReco/VisMuonRPC/interface/VisRPCRecHitTwig.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisRPCContent, "Data/Reco/MuonRPC");

/** Initialise the reconsructed RPC data data proxy. */
VisRPCContent::VisRPCContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual initialisation: add the RPC twigs to the
    document data tree.  */
void
VisRPCContent::init (void)
{    
  LOG (0, trace, LFfwvis, "VisRPCContent::init()\n");
    
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
  IgTwig *eventTwig = 0;

  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

  eventTwig = topTwig->lookup ("CMS Event");
  if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "CMS Event");
    
  IgTwig *rpcEventTwig = new IgSimpleTwig (eventTwig, "Muon RPC Event");
  new VisRPCDigiTwig (state (), rpcEventTwig, "RPC digis");
  new VisRPCRecHitTwig (state (), rpcEventTwig, "RPC RecHit");
}
