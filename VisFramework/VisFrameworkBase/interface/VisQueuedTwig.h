#ifndef VIS_FRAMEWORK_BASE_VIS_QUEUED_TWIG_H
# define VIS_FRAMEWORK_BASE_VIS_QUEUED_TWIG_H

# include "VisFramework/VisFrameworkBase/interface/VisEventObserver.h"
# include "FWCore/Utilities/interface/Exception.h"
# include "Iguana/Framework/interface/IgTwig.h"
# include <classlib/utils/Error.h>
# include <classlib/utils/Callback.h>
# include <exception>

namespace edm 
{
    class Event;
    class EventSetup;
}

class IgState;
class Ig3DRep;
class IgLegoRep;
class IgTextRep;
class IgRPhiRep;
class IgRZRep;
class IgSpareRep;

class VisQueuedTwig : public IgSimpleTwig, public VisEventObserver
{
public:
    VisQueuedTwig (IgState *state, IgTwig *parent, const std::string &name = "");

    virtual void	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);

    virtual void	update (Ig3DRep *rep);
    virtual void	update (IgLegoRep *rep);
    virtual void	update (IgTextRep *rep);
    virtual void        update (IgRPhiRep *rep);
    virtual void        update (IgRZRep *rep);
    virtual void        update (IgSpareRep *rep);

    IgState *		state (void);

    void                onException (lat::Callback1<std::exception *> callback);
    void                onError (lat::Callback1<lat::Error *> callback);
    void                onCmsException (lat::Callback1<cms::Exception *> callback);
    void                onUnhandledException (lat::Callback callback);

protected:
    void                onBaseInvalidate (void);
    lat::Callback1<std::exception *>   m_onException;
    lat::Callback1<lat::Error *>       m_onError;
    lat::Callback1<cms::Exception *>   m_onCmsException;
    lat::Callback       m_onUnhandledException;

private:

    class ExactMatch;
    friend class ExactMatch;

    IgState		       *m_state;		//< State context.
};

/** Return the #IgState context in which this object was created.
 */
inline IgState *
VisQueuedTwig::state (void)
{ return m_state; }

#endif // VIS_FRAMEWORK_BASE_VIS_QUEUED_TWIG_H
