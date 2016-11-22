#include "VisSimulation/VisSimBase/interface/VisG4GeomContent.h"
#include "VisSimulation/VisSimBase/interface/VisG4GeomTwig.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/Framework/interface/IgRepSet.h"

IG_DEFINE_STATE_ELEMENT (VisG4GeomContent, "Data/Simulation/Geometry");

/** Initialise the Simulation geometry data content. */
VisG4GeomContent::VisG4GeomContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual initialisation: add the geometry twig to the
    document data tree.  */
void
VisG4GeomContent::init (void)
{
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *eventTwig = 0;
  eventTwig = rootTwig->lookup ("CMS Event and Detector");
  if (! eventTwig)
    eventTwig = rootTwig;
  
  new VisG4GeomTwig (state (), eventTwig);

  ASSERT (IgQtObjectMenuService::get (state ()));

  IgRepSet::invalidate (rootTwig, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}
