//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisCustomTracker/interface/VisCuTkRecoContent.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkGeometryTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Framework/interface/IgRepSet.h"

IG_DEFINE_STATE_ELEMENT (VisCuTkRecoContent, "Data/Reco/CustomTracker");

/** Initialise the reconsructed Tracker detector data proxy. */
VisCuTkRecoContent::VisCuTkRecoContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the detector twig to the
    document data tree.  */
void
VisCuTkRecoContent::init (void)
{    
  LOG (0, trace, LFfwvis, "VisCuTkRecoContent::init()\n");
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *topTwig = 0;
  IgTwig *detTwig = 0;

  topTwig = rootTwig->lookup ("CMS Event and Detector");
  if (!topTwig) topTwig = new IgSimpleTwig (rootTwig, "CMS Event and Detector");
    
  detTwig = topTwig->lookup ("Reco Detector");
  if (!detTwig) detTwig = new IgSimpleTwig (topTwig, "Reco Detector");   
    
  IgTwig *trackerDet = new IgSimpleTwig (detTwig, "CustomTracker");
  VisCuTkGeometryTwig *geom0Twig = new VisCuTkGeometryTwig (state (), trackerDet, "View3D");

  if (VisEventProcessorService *procService = VisEventProcessorService::get (state ())) 
  {
    (*procService)->postProcessEventSignal().connect (sigc::mem_fun (geom0Twig, &VisCuTkGeometryTwig::onNewEvent));
  }
    
  IgRepSet::invalidate (rootTwig, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}
