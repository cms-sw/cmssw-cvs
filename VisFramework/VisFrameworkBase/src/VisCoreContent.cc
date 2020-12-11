#include "VisFramework/VisFrameworkBase/interface/VisCoreContent.h"
#include "VisFramework/VisFrameworkBase/interface/VisMainProgram.h"

IG_DEFINE_STATE_ELEMENT (VisCoreContent, "Data/Framework/Core");

/** Initialise the Framework event data proxy. */
VisCoreContent::VisCoreContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-main-thread) initialisation.  Instantiates the
    FW EDM main program.  */
void
VisCoreContent::init (void)
{
  new VisMainProgram (state ());
}
