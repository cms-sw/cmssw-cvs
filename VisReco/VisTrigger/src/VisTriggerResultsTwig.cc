//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisTrigger/interface/VisTriggerResultsTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/Common/interface/TriggerResults.h"
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
	return new VisTriggerResultsTwig (state, parent, name, 
					  friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisTriggerResultsTwig::VisTriggerResultsTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID triggerID (typeid (edm::TriggerResults));
    tfService->registerTwig (triggerID.friendlyClassName (), &createThisTwig);
}

void
VisTriggerResultsTwig::onNewEvent (const edm::Event &event,
			    const edm::EventSetup &eventSetup)
{    
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_hlNames.clear ();

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    try
    {
	event.getByType (m_hltresults);
	if (m_hltresults.isValid ())
	{
	    const edm::TriggerResults c = *(m_hltresults).product ();
	    m_hlNames = c.getTriggerNames ();
	
	    std::cout << "edm::TriggerResults: " << m_hlNames.size () << std::endl;

	    QString triggerHeader;
	
	    for (std::vector<std::string>::const_iterator iName = m_hlNames.begin (), 
						       iNameEnd = m_hlNames.end (); 
		 iName != iNameEnd; ++iName)
	    {
		std::cout << (*iName) << std::endl;		    

		triggerHeader.append (QString::fromStdString (*iName));
		triggerHeader.append (": was run ");

		// decision for each HL algorithm
		int index = c.find (*iName);
		c.wasrun (index) ? triggerHeader.append ("Yes") : triggerHeader.append ("No");    
		triggerHeader.append ("; accept ");
		c.accept (index) ? triggerHeader.append ("Yes") : triggerHeader.append ("No");
		triggerHeader.append ("; error ");
		c.error (index) ? triggerHeader.append ("Yes") : triggerHeader.append ("No");
		triggerHeader.append (". <br>");

		std::cout << triggerHeader.latin1 () << std::endl;
		m_decision = triggerHeader;
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
VisTriggerResultsTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

//     if (m_hltresults.isValid ()) 
//     {
// 	try
// 	{    
// 	    // 	    for (std::vector<edm::Handle<edm::TriggerResults> >::iterator i = m_hltresults.begin (), 
// 	    // 								       iEnd = m_hltresults.end (); 
// 	    // 		 i != iEnd; ++i) 
// 	    // 	    {
// 	    if (m_hltresults.isValid ())
// 	    {		    
// 		const edm::TriggerResults c = *(m_hltresults).product ();
// 		std::vector<std::string>  hlNames = c.getTriggerNames ();

// 		QString triggerHeader;
		
// 		for (std::vector<std::string>::const_iterator iName = hlNames.begin (), iNameEnd = hlNames.end (); iName != iNameEnd; ++iName)
// 		{
// 		    triggerHeader.append (*iName);
// 		    triggerHeader.append (": was run ");

// 		    // decision for each HL algorithm
// 		    int index = c.find (*iName);
// 		    c.wasrun (index) ? triggerHeader.append ("Yes") : triggerHeader.append ("No");    
// 		    triggerHeader.append ("; accept ");
// 		    c.accept (index) ? triggerHeader.append ("Yes") : triggerHeader.append ("No");
// 		    triggerHeader.append ("; error ");
// 		    c.error (index) ? triggerHeader.append ("Yes") : triggerHeader.append ("No");
// 		    triggerHeader.append (". ");    
// 		}
		
// 		SoSeparator         *label = new SoSeparator;
// 		SoText2             *labelText = new SoText2;
// 		SoTranslation       *labelOffset = new SoTranslation;
// 		labelText->string = triggerHeader.latin1 ();
// 		labelOffset->translation
// 		    = SbVec3f (-0.6, 1, -0.6);
// 		label->addChild (labelOffset);
// 		label->addChild (labelText);
	    
// 		rep->clear ();
// 		rep->node ()->addChild (label);
// 	    }
//         }
// 	catch (...) 
// 	{
// 	}
//     }
}

void
VisTriggerResultsTwig::update (IgTextRep *rep)
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
	 << "<TH>Name</TH>"
	 << "<TH>Was run</TH>"
	 << "<TH>Accept</TH>"
	 << "<TH>Error</TH>"
	 << "</TR>";

    text << "<TR><TH>" << m_decision.latin1 () << "</TH></TR>";
    
//     if (m_hltresults.isValid ()) 
//     {
// 	try
// 	{    
// 	    // 	    for (std::vector<edm::Handle<edm::TriggerResults> >::iterator i = m_hltresults.begin (), 
// 	    // 								       iEnd = m_hltresults.end (); 
// 	    // 		 i != iEnd; ++i) 
// 	    // 	    {
// 	    // 		if ((*i).isValid ())
// 	    // 		{
		    
// 	    const edm::TriggerResults c = *(m_hltresults).product ();
// 	    std::vector<std::string>  hlNames = c.getTriggerNames ();

// 	    QString triggerHeader;
		
// 	    for (std::vector<std::string>::const_iterator iName = hlNames.begin (), iNameEnd = hlNames.end (); iName != iNameEnd; ++iName)
// 	    {
// 		text << "<TR align = right>"
// 		     << "<TD>" << std::setw (5) << std::setprecision (3) << *iName << "</TD>";

// // 		// decision for each HL algorithm
// // 		int index = c.find (*iName);
// // 		text << "<TD>" << std::setw (5) << std::setprecision (3);
		    
// // 		c.wasrun (index) ? (text << "Yes") : (text << "No");    
// // 		text << "</TD>";
// // 		c.accept (index) ? (text << "Yes") : (text << "No");
// // 		text << "</TD>";
// // 		c.error (index) ? (text << "Yes") : (text << "No");
// 		text << "</TD></TR>";
// 	    }
// 	}
// 	catch (...) 
// 	{
// 	    text << "No information available: an exception is caught.";
// 	}
//     }
    text << "</table>";  

    rep->setText (text.str ());
}
