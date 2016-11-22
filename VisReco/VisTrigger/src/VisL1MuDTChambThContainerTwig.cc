//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisTrigger/interface/VisL1MuDTChambThContainerTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
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
	return new VisL1MuDTChambThContainerTwig (state, parent, name, 
						  friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisL1MuDTChambThContainerTwig::VisL1MuDTChambThContainerTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID triggerID (typeid (L1MuDTChambThContainer));
    tfService->registerTwig (triggerID.friendlyClassName (), &createThisTwig);
}

void
VisL1MuDTChambThContainerTwig::onNewEvent (const edm::Event &event,
					   const edm::EventSetup &eventSetup)
{    
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    try
    {
	edm::Handle<L1MuDTChambThContainer> l1dtlocalphi;
	event.getByType (l1dtlocalphi);
	if (l1dtlocalphi.isValid ())
	{
	    m_phiContainer = l1dtlocalphi->getContainer();
	    std::cout << "ThiContainer: " << m_phiContainer->size () << std::endl;

	    for (std::vector<L1MuDTChambThDigi>::const_iterator i = m_phiContainer->begin (), iEnd = m_phiContainer->end (); i != iEnd; ++i) 
	    {
		int thwheel = i->whNum ();
		int thsec   = i->scNum () + 1; // SM The track finder goes from 0 to 11. I need them from 1 to 12 !!!!!
		int thst    = i->stNum ();
		//int thbx    = i->bxNum ();
		
		DTChamberId dtChId (thwheel, thst, thsec);
		//uint32_t indexCh = dtChId.rawId ();   
		//uint32_t indexScWh = 5 * (thsec - 1) + (thwheel + 3) ;    // wheel + sector identifier

		std::cout << "Wheel " << thwheel << "; Station " << thst << "; Sector " << thsec << std::endl;
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
VisL1MuDTChambThContainerTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisL1MuDTChambThContainerTwig::update (IgTextRep *rep)
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

    text << "<TR><TH>" << m_decision.latin1 () << "</TH></TR>";
    
    text << "</table>";  

    rep->setText (text.str ());
}
