#include "VisReco/VisMagneticField/interface/VisCMSMagFieldContent.h"
#include "VisReco/VisMagneticField/interface/VisCMSMagFieldTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgDocumentData.h"

IG_DEFINE_STATE_ELEMENT (VisCMSMagFieldContent, "Data/Reco/CMS Magnetic Field");

/** Initialise the CMS magnetic field data content. */
VisCMSMagFieldContent::VisCMSMagFieldContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual initialisation: add the magnetic field twig to the
    document data tree.  */
void
VisCMSMagFieldContent::init (void)
{
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
    
  VisCMSMagFieldTwig *magFieldTwig = new VisCMSMagFieldTwig (rootTwig, state ());

  VisEventProcessorService *procService = VisEventProcessorService::get (state ());
  ASSERT(procService);
  (*procService)->postProcessEventSignal().connect (sigc::mem_fun (magFieldTwig, &VisCMSMagFieldTwig::onNewEvent));
    
  IgRepSet::invalidate (rootTwig, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}
