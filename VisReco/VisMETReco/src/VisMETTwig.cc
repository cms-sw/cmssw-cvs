//<<<<<< INCLUDES                                                       >>>>>>

#include "DataFormats/METReco/interface/MET.h"
#include "VisReco/VisMETReco/interface/VisMETTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "FWCore/Framework/interface/Event.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include <Inventor/nodes/SoMaterial.h>
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
	return new VisMETTwig (state, parent, "MET (" + name + ")", 
			       friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace reco;

VisMETTwig::VisMETTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID metCollID (typeid (METCollection));
    tfService->registerTwig (metCollID.friendlyClassName (), &createThisTwig);
}

void
VisMETTwig::onNewEvent (const edm::Event &event,
			const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());
    
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, m_handles);
	}
	else
	{
	    event.getManyByType (m_handles);
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
VisMETTwig::update (IgTextRep *rep)
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
	 << "<TH>Et</TH>"
	 << "<TH>Phi</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
    
    int nmets = 0;
    if (! m_handles.empty ())
    {
	try 
	{
	    for (std::vector<edm::Handle<METCollection> >::iterator i = m_handles.begin (), iEnd = m_handles.end (); 
		 i != iEnd; ++i) 
	    {
		const METCollection *met = (*i).product ();
		const MET caloMET = met->front ();
		Float_t phi = caloMET.phi ();
		Float_t pt = caloMET.pt (); 

		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << ++nmets << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << pt << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << phi << "</TD>"
		     << "</TR>";
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
    }

    text << "</table>";
    
    // Send it over.
    rep->setText (text.str ());
}

void
VisMETTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    SoSeparator *sep = new SoSeparator;

    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (1, 0, 0);
    sep->addChild (mat);
    
    rep->node ()->addChild (sep);
}

void
VisMETTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
}

void
VisMETTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
}

void
VisMETTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
}

