#include "VisFramework/VisEvent/interface/VisEventCoreContent.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventIdTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisFEDRawDataCollectionTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "VisFramework/VisEvent/interface/VisEventContentTwig.h"
#include "VisFramework/VisEvent/interface/VisEventContentTwigOps.h"
#include "Iguana/Studio/interface/IgTwigOps.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "Iguana/Studio/interface/IgDocumentData.h"

IG_DEFINE_STATE_ELEMENT (VisEventCoreContent, "Data/Event/Core Content");

/** Initialise the Event data content. */
VisEventCoreContent::VisEventCoreContent (IgState *state)
  : m_state(state)
{
  m_state->put (s_key, this);
  init();
}

/** Actual (in-event-thread) initialisation: add the event twig to the
    document data tree.  */
void
VisEventCoreContent::init (void)
{
  IgTwig *rootTwig = IgDocumentData::get (state ())->root ();
  IgTwig *eventTwig = 0;
    
  eventTwig = IgDocumentData::get (state ())->find ("Event Collections");
  if (!eventTwig) eventTwig = new IgSimpleTwig (rootTwig, "Event Collections");
    
  VisEventContentTwig *evtCntTwig = new VisEventContentTwig (state (), eventTwig, "Alias (FriendlyName:ModuleLabel:InstanceName:ProcessName)");

  new VisEventIdTwig (state (), eventTwig, "Run and Event Number");
  new VisFEDRawDataCollectionTwig (state (), eventTwig, "FEDRawData");
    
  ASSERT (IgQtObjectMenuService::get (state ()));
  IgQtObjectMenuService::get (state ())
    ->add (lat::CreateCallback (new IgTwigOps, &IgTwigOps::menuAction));

  if (VisEventProcessorService *procService = VisEventProcessorService::get (state ()))
  {
    (*procService)->preProcessEventSignal().connect (sigc::mem_fun (evtCntTwig, &VisEventContentTwig::onPreEvent));
    (*procService)->postProcessEventSignal().connect (sigc::mem_fun (evtCntTwig, &VisEventContentTwig::onNewEvent));
  }
    
  // Register object action menus
  ASSERT (IgQtObjectMenuService::get (state ()));
  IgQtObjectMenuService::get (state ())
    ->add (lat::CreateCallback (new VisEventContentTwigOps, &VisEventContentTwigOps::menuAction));
}
