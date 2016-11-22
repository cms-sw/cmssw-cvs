#include "VisSimulation/VisSimBase/interface/VisG4MagFieldContent.h"
#include "VisGeant4/VisG4Fields/interface/VisG4MagFieldTwig.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgRepSet.h"

IG_DEFINE_STATE_ELEMENT (VisG4MagFieldContent, "Data/Simulation/MagField");

/** Initialise the Simulation magnetic field data content. */
VisG4MagFieldContent::VisG4MagFieldContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual initialisation: add the hits twig to the
    document data tree.  */
void
VisG4MagFieldContent::init (void)
{
  IgTwig *root = IgDocumentData::get (state ())->root ();
  new VisG4MagFieldTwig (root, state ());

  IgRepSet::invalidate (root, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}
