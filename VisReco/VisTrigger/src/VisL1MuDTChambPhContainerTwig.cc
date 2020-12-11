#include "VisReco/VisTrigger/interface/VisL1MuDTChambPhContainerTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
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
	return new VisL1MuDTChambPhContainerTwig (state, parent, name, 
						  friendlyName, modLabel, instanceName, processName);
    }
}

VisL1MuDTChambPhContainerTwig::VisL1MuDTChambPhContainerTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID triggerID (typeid (L1MuDTChambPhContainer));
    tfService->registerTwig (triggerID.friendlyClassName (), &createThisTwig);
}

void
VisL1MuDTChambPhContainerTwig::onNewEvent (const edm::Event &event,
					   const edm::EventSetup &eventSetup)
{    
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());
    try 
    {
	eventSetup.get<MuonGeometryRecord> ().get (m_muonGeom);
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
    
    try
    {
	edm::Handle<L1MuDTChambPhContainer> l1dtlocalphi;
	event.getByType (l1dtlocalphi);
	if (l1dtlocalphi.isValid ())
	{
	    m_phiContainer = l1dtlocalphi->getContainer();
	    std::cout << "PhiContainer: " << m_phiContainer->size () << std::endl;

	    int phcode_best [6][5][13];
	    std::vector<L1MuDTChambPhDigi>::const_iterator ibest [6][5][13];

	    // define best quality phi trigger segment in any station
	    // start from 1 and zero is kept empty
	    for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 6; ++j)
		    for (int k = 0; k < 13; ++k)
			phcode_best [j][i][k] = -1;

	    for (std::vector<L1MuDTChambPhDigi>::const_iterator i = m_phiContainer->begin (), iEnd = m_phiContainer->end (); i != iEnd; ++i) 
	    {
		int phwheel = i->whNum ();
		int phsec   = i->scNum () + 1; // SM The track finder goes from 0 to 11. I need them from 1 to 12 !!!!!
		int phst    = i->stNum ();
		//int phbx    = i->bxNum ();
		int phcode  = i->code ();
		//int phi1st  = i->Ts2Tag ();
		int phphi   = i->phi ();
		//int phphiB  = i->phiB ();

		if (phcode > phcode_best [phwheel + 3][phst][phsec] && phcode < 7) 
		{
		    phcode_best [phwheel + 3][phst][phsec] = phcode; 
		    ibest [phwheel + 3][phst][phsec] = i;
		    std::cout << "--> Best Phi: " << phphi << std::endl;
		}
		
		DTChamberId dtChId (phwheel, phst, phsec);
		//uint32_t indexCh = dtChId.rawId ();
		//uint32_t indexScWh = 5 * (phsec - 1) + (phwheel + 3);    // wheel + sector identifier
		std::cout << "Wheel " << phwheel << "; Station " << phst << "; Sector " << phsec << std::endl;
		std::cout << "Phi: " << phphi << std::endl;
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
VisL1MuDTChambPhContainerTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisL1MuDTChambPhContainerTwig::update (IgTextRep *rep)
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
