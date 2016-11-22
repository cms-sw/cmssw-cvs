#include "VisSimulation/VisSimData/interface/VisSimDataContent.h"
#include "VisSimulation/VisSimData/interface/VisHepMCProductTwig.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgQtLock.h"

IG_DEFINE_STATE_ELEMENT (VisSimDataContent, "Data/Simulation/Data Formats");

/** Initialise the simulated data formats data proxy. */
VisSimDataContent::VisSimDataContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}


/** Actual (in-event-thread) initialisation: add the sim hits twigs to the
    document data tree.  */
void
VisSimDataContent::init (void)
{
  IgQtLock ();
    
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
  IgTwig *eventTwig = 0;

  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");

  eventTwig = topTwig->lookup ("CMS Event");
  if (!eventTwig) eventTwig = new IgSimpleTwig (topTwig, "CMS Event");
    
  IgTwig *simEventTwig = new IgSimpleTwig (eventTwig, "Simulated Event Data");
  new VisHepMCProductTwig (state (), simEventTwig, "HepMC product");
}
