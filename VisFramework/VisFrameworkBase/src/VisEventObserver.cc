#include "VisFramework/VisFrameworkBase/interface/VisEventObserver.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"

VisEventObserver::VisEventObserver (IgState *state)
    : m_state (state)
{    
    ASSERT (m_state);

    if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
    {
	procService->onProcessorStarted (
	    lat::CreateCallback (this, 
				 &VisEventObserver::init));
    }
    init ();
}

void
VisEventObserver::init (void)
{    
    if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
    {
	if (m_connection) 
	{
	    m_connection.disconnect ();
	}
	
	m_connection = (*procService)->postProcessEventSignal().connect (
	    sigc::mem_fun (this, 
			   &VisEventObserver::onNewEvent));
    }
}

VisEventObserver::~VisEventObserver (void)
{    
    m_connection.disconnect ();
}
