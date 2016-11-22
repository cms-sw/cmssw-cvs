#include "VisSimulation/VisSimBase/interface/VisSimContent.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "Iguana/Studio/interface/IgTwigOps.h"
#include "VisGeant4/VisG4Core/interface/VisG4VisManager.h"
#include "VisGeant4/VisG4Core/interface/VisG4UIManager.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4TwigOps.h"
#include "Iguana/Studio/interface/IgQtLock.h"

IG_DEFINE_STATE_ELEMENT (VisSimContent, "Data/Simulation/Core");

/** Initialise the G4 content proxy. */
VisSimContent::VisSimContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: initialise Geant4.  */
void
VisSimContent::init (void)
{
  IgQtLock ();

  // Setup G4
  new VisG4VisManager (state ());
  new VisG4UIManager (state ());
    
  // Register object action menus
  ASSERT (IgQtObjectMenuService::get (state ()));
  IgQtObjectMenuService::get (state ())
    ->add (lat::CreateCallback (new IgTwigOps, &IgTwigOps::menuAction));
  IgQtObjectMenuService::get (state ())
    ->add (lat::CreateCallback (new VisG4TwigOps, &VisG4TwigOps::menuAction));
}
