#include "VisReco/VisRecoJets/interface/VisRecoJetContent.h"
#include "VisReco/VisRecoJets/interface/VisCaloJetTwig.h"
#include "VisReco/VisRecoJets/interface/VisCaloTowerTwig.h"
#include "VisReco/VisRecoJets/interface/VisGenJetCollectionTwig.h"
#include "VisReco/VisRecoJets/interface/VisJetTagTwig.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisRecoJetContent, "Data/Reco/Jets");

/** Initialise the reconsructed Jets data data proxy. */
VisRecoJetContent::VisRecoJetContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the detector twig to the
    document data tree.  */
void
VisRecoJetContent::init (void)
{    
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;

  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

  new VisCaloTowerTwig (state (), topTwig, "CaloTowers");
  new VisCaloJetTwig (state (), topTwig, "CaloJets");
  new VisGenJetCollectionTwig (state (), topTwig, "GenJets");
  // FIXME: new VisJetTagTwig (state (), eventTwig, "JetTags");
}

