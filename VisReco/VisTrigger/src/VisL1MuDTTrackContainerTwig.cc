//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisTrigger/interface/VisL1MuDTTrackContainerTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackContainer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/SbLinear.h>
#include <qstring.h>
#include <sstream>
#include <iomanip>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

namespace 
{
    VisQueuedTwig *
    createThisTwig (IgState *state, IgTwig *parent,
		    const std::string &name,
		    const std::string &friendlyName,
		    const std::string &modLabel,
		    const std::string &instanceName,
		    const std::string &processName)
    {
	return new VisL1MuDTTrackContainerTwig (state, parent, name, 
						friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisL1MuDTTrackContainerTwig::VisL1MuDTTrackContainerTwig (IgState *state, IgTwig *parent,
							  const std::string &name /* = "" */,
							  const std::string &friendlyName /* = "" */,
							  const std::string &moduleLabel /* = "" */,
							  const std::string &instanceName /* = "" */,
							  const std::string &processName /* = ""*/)
    : VisQueuedTwig (state, parent, name),
      m_state (state),
      m_text ("no info"),
      m_friendlyName (friendlyName),
      m_moduleLabel (moduleLabel),
      m_instanceName (instanceName),
      m_processName (processName)
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID triggerID (typeid (L1MuDTTrackContainer));
    tfService->registerTwig (triggerID.friendlyClassName (), &createThisTwig);
}

void
VisL1MuDTTrackContainerTwig::onNewEvent (const edm::Event &event,
					 const edm::EventSetup &eventSetup)
{    
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());
    try
    {
	edm::Handle<L1MuDTTrackContainer> l1MuDTTracks;
	event.getByType (l1MuDTTracks);
	if (l1MuDTTracks.isValid ())
	{
	    m_container = l1MuDTTracks->getContainer ();
	    std::cout << "L1MuDTTrackContainer: " << m_container->size () << std::endl;

	    for (std::vector<L1MuDTTrackCand>::const_iterator i = m_container->begin (), iEnd = m_container->end (); 
		 i != iEnd; ++i) 
	    {
		if (! i->empty ())
		{
		  //float phi = i->phiValue ();
		  // float eta = i->etaValue ();
		  //float pt = i->ptValue ();
		    i->print ();
		}
	    }
	}
    }
    catch (cms::Exception& e)
    {
	if (this->m_onCmsException)
	    this->m_onCmsException (&e);
    }
    catch (lat::Error &e) 
    {
	if (this->m_onError)
	    this->m_onError (&e);
    }
    catch (std::exception &e) 
    {
	if (this->m_onException)
	    this->m_onException (&e);
    }
    catch (...) 
    {
	if (this->m_onUnhandledException)
	    this->m_onUnhandledException ();
    }

    VisQueuedTwig::onBaseInvalidate ();
}

void
VisL1MuDTTrackContainerTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisL1MuDTTrackContainerTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare the property description.
    std::ostringstream text;

    text << m_text;
  
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>DetId</TH>"
	 << "<TH>Wheel</TH>"
	 << "<TH>Station</TH>"
	 << "<TH>Sector</TH>"
	 << "</TR>";

    // text << "<TR><TH>" << m_decision.latin1 () << "</TH></TR>";
    
    text << "</table>";  
    rep->setText (text.str ());
}
