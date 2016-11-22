#include "VisReco/VisRecoJets/interface/VisRecoCandidateContent.h"
#include "VisReco/VisRecoJets/interface/VisCandidateTwig.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"

IG_DEFINE_STATE_ELEMENT (VisRecoCandidateContent, "Data/Reco/Candidate");

/** Initialise the reconsructed Candidates data data proxy. */
VisRecoCandidateContent::VisRecoCandidateContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the detector twig to the
    document data tree.  */
void
VisRecoCandidateContent::init (void)
{    
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
  IgTwig *eventTwig = 0;

  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

  eventTwig = topTwig->lookup ("CMS Event");
  if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "CMS Event");
    
  new VisCandidateTwig (state (), eventTwig, "Candidates");
}
