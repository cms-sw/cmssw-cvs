#ifndef VIS_FRAMEWORK_BASE_VIS_EVENT_PROCESSOR_SERVICE_H
# define VIS_FRAMEWORK_BASE_VIS_EVENT_PROCESSOR_SERVICE_H

# include "Iguana/Framework/interface/IgStateElement.h"
# include "FWCore/ServiceRegistry/interface/ServiceLegacy.h"
# include "FWCore/ServiceRegistry/interface/ServiceToken.h"
# include "FWCore/Framework/interface/EventProcessor.h"
# include "FWCore/MessageLogger/interface/JobReport.h"
# include <classlib/utils/Error.h>
# include <classlib/utils/Callback.h>
# include <exception>

namespace edm 
{
    class Presence;
}

class VisEventProcessorService : public IgStateElement
{    
    IG_DECLARE_STATE_ELEMENT (VisEventProcessorService);
public:
    VisEventProcessorService (IgState *state);
    ~VisEventProcessorService (void);

    void on () {
	m_callEndJob = true;
    }
    void off () {
	m_callEndJob = false;
    }
     
    edm::EventProcessor* operator-> () {
	return m_ep.get ();
    }
    edm::JobReport* jobReport (void) {
	return m_jobRep.get ();
    }
    
    std::string			configFile (void);
    std::string                 configRootFile (std::string fileName);
    void 			initEventProcessor (void);
    void 			onStartingProcessor (lat::Callback callback);
    void 			onStoppingProcessor (lat::Callback callback);
    void 			onProcessorStarted (lat::Callback callback);
    void 			onProcessorStopped (lat::Callback callback);
    void 			onException (lat::Callback1<std::exception *> callback);
    void                	onCmsException (lat::Callback1<cms::Exception *> callback);
    void 			onFrameworkException (lat::Callback1<lat::Error *> callback);
    void 			onUnhandledException (lat::Callback callback);
    void			onConfigFailure (lat::Callback1<cms::Exception *> callback);
    void			onStatusMessage (lat::Callback1<const std::string&> callback);
    void			scheduleAbort (void);
    bool			running (void);

private:
    std::auto_ptr<edm::EventProcessor> 	m_ep;
    boost::shared_ptr<edm::Presence>    m_messageServicePresence;
    std::auto_ptr<edm::JobReport>       m_jobRep;

    bool 			m_callEndJob;
    bool			m_running;
    IgState			*m_state;
    lat::Callback		m_onStartingProcessor;
    lat::Callback		m_onStoppingProcessor;
    lat::Callback		m_onProcessorStarted;    
    lat::Callback		m_onProcessorStopped;    
    lat::Callback1<cms::Exception *>   	m_onCmsException;
    lat::Callback1<std::exception *>   	m_onException;
    lat::Callback1<lat::Error *>       	m_onFrameworkException;
    lat::Callback 			m_onUnhandledException;
    lat::Callback1<cms::Exception *>    m_onConfigFailure;
    lat::Callback1<const std::string&> 	m_onStatusMessage;
    
    void 			init (void);
};

#endif // VIS_FRAMEWORK_BASE_VIS_EVENT_PROCESSOR_SERVICE_H
