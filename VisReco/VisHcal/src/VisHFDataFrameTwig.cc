//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisHcal/interface/VisHFDataFrameTwig.h"
#include "VisReco/VisHcal/interface/VisHcalSubdetector.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HFDataFrame.h"
#include "FWCore/Framework/interface/Event.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include <Inventor/nodes/SoSeparator.h>
#include <qstring.h>
#include <iostream>
#include <sstream>
#include <iomanip>

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
	return new VisHFDataFrameTwig (state, parent, "HF data frames (" + name + ")", 
				       friendlyName, modLabel, instanceName, processName);
    }
}
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisHFDataFrameTwig::VisHFDataFrameTwig (IgState *state, IgTwig *parent,
					const std::string &name /* = "" */,
					const std::string &friendlyName /* = "" */,
					const std::string &moduleLabel /* = "" */,
					const std::string &instanceName /* = "" */,
					const std::string &processName /* = "" */)    
    : VisQueuedTwig (state, parent, name),
      m_name (name),
      m_friendlyName (friendlyName),
      m_moduleLabel (moduleLabel),
      m_instanceName (instanceName),
      m_processName (processName),
      m_text ("no info")
{    
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID digiHitCollID (typeid (HFDigiCollection));
    tfService->registerTwig (digiHitCollID.friendlyClassName (), &createThisTwig);

}

void
VisHFDataFrameTwig::onNewEvent (const edm::Event &event,
				const edm::EventSetup &eventSetup)
{
    VisQueuedTwig::onBaseInvalidate ();
    m_digis.clear ();

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());
    
// FIXME: We may need the CaloGeometry later...
//     if (! m_done)
//     {
// 	try
// 	{
// 	    eventSetup.get<CaloGeometryRecord> ().get (m_pDD);
// 	    m_done = true;
// 	}
// 	catch (...) 
// 	{
// 	}
//     }

    std::vector<edm::Handle<HFDigiCollection> > digis;
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, digis);
	}
	else
	{
	    event.getManyByType (digis);
	}
    }
    catch (...) 
    {
    }
    if (! digis.empty ())
    {
	for (std::vector<edm::Handle<HFDigiCollection> >::iterator i = digis.begin (), iEnd = digis.end (); i != iEnd; ++i) 
	{
	    const HFDigiCollection& c = *(*i);
	    for (std::vector<HFDataFrame>::const_iterator idigi = c.begin (), idigiEnd = c.end (); 
		 idigi != idigiEnd; ++idigi)
	    {
		m_digis.push_back (*idigi);
	    }
	}
    }
    
    IgRepSet::invalidate (this, SELF_MASK);
}

void
VisHFDataFrameTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << m_name << " from ";
    text << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Subdetector</TH>"
	 << "<TH>ietaAbs</TH>"
	 << "<TH>ieta</TH>"
	 << "<TH>iphi [rad]</TH>"
	 << "<TH>Depth</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
    
    int nDigis = 0;
    try 
    {
	for (std::vector<HFDataFrame>::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	{
	    text << "<TR align = right>"
		 << "<TD>" << std::setw (3) << nDigis++ << "</TD>"
		 << "<TD>" << std::setw (2) << VisHcalSubdetector::subDetName ((*i).id ().subdet ()) << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).id ().ietaAbs () << "</TD>"
		 << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).id ().ieta () << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).id ().iphi () << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).id ().depth() << "</TD>"
		 << "</TR>";
        }
    }
    catch (...)
    {
	text << "No " << m_name << " HF digis.<br>";
    }
    text << "</table>";
    
    // Send it over.
    rep->setText (text.str ());
}

void
VisHFDataFrameTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    SoSeparator *sep = new SoSeparator;
    
    rep->node ()->addChild (sep);
}

void
VisHFDataFrameTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    SoSeparator *sep = new SoSeparator;
    
    rep->node ()->addChild (sep);
}

void
VisHFDataFrameTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;
    
    rep->node ()->addChild (sep);
}
