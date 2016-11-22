//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include <QRegExp>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

class VisQueuedTwig::ExactMatch
{
    std::string	m_name;

public:
    ExactMatch (std::string name) : m_name (name) {}

    bool operator() (const std::string twig)
	{ 
	    QRegExp rx (QString::fromStdString (twig));
	    rx.setWildcard (true);
	    rx.setCaseSensitive (false);
	    return rx.exactMatch (QString::fromStdString (m_name));
	}
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisQueuedTwig::VisQueuedTwig (IgState *state, IgTwig *parent,
			      const std::string &name /* = "" */)
    : IgSimpleTwig (parent, name, true, false, true),
      VisEventObserver (state),
      m_state (state)
{ 
    ASSERT (m_state); 
    const edm::ParameterSet *pset = VisConfigurationService::pSet ();
    if (pset)
    {
        std::vector<std::string> twigs;
        twigs = pset->getUntrackedParameter<std::vector<std::string> > ("EnabledTwigs", twigs);
	
	if (std::find_if (twigs.begin (), twigs.end (), ExactMatch (fullName ())) != twigs.end ())
	    selfVisible (true);
    }
}

void
VisQueuedTwig::onNewEvent (const edm::Event &, const edm::EventSetup &) 
{ LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
       << "]::onNewEvent(edm::Event,edm::EventSetup)\n");}

void
VisQueuedTwig::onBaseInvalidate (void)
{
    LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
         << "]::onBaseInvalidate()\n");
    
    IgRepSet::invalidate (this, SELF_MASK);
}

void
VisQueuedTwig::onException (lat::Callback1<std::exception *> callback)
{
    LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
         << "]::onException()\n");

    m_onException = callback;
}

void
VisQueuedTwig::onUnhandledException (lat::Callback callback)
{
    LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
         << "]::onUnhandledException()\n");

    m_onUnhandledException = callback;    
}

void
VisQueuedTwig::onError (lat::Callback1<lat::Error *> callback)
{
    LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
         << "]::onError()\n");

    m_onError = callback;
}

void
VisQueuedTwig::onCmsException (lat::Callback1<cms::Exception *> callback)
{
    LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
         << "]::onCmsException()\n");

    m_onCmsException = callback;
}

//////////////////////////////////////////////////////////////////////
void
VisQueuedTwig::update (Ig3DRep *)
{ LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
       << "]::update(Ig3DRep)\n"); }

void
VisQueuedTwig::update (IgLegoRep *)
{ LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
       << "]::update(IgLegoRep)\n"); }

void
VisQueuedTwig::update (IgTextRep *)
{ LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
       << "]::update(IgTextRep)\n"); }

void
VisQueuedTwig::update (IgRPhiRep *)
{ LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
       << "]::update(IgRPhiRep)\n"); }


void
VisQueuedTwig::update (IgRZRep *)
{ LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
       << "]::update(IgRZRep)\n"); }
void
VisQueuedTwig::update (IgSpareRep *)
{ LOG (0, trace, LFfwvis, "VisQueuedTwig[" << name ()
       << "]::update(IgSpareRep)\n"); }

