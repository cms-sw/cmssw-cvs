//<<<<<< INCLUDES                                                       >>>>>>

#include "VisSimulation/VisSimHits/interface/VisSimVertexTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "Iguana/Studio/interface/IgQtLock.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
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
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

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
	return new VisSimVertexTwig (state, parent, name, friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisSimVertexTwig::VisSimVertexTwig (IgState *state, IgTwig *parent, const std::string &name /* = ""*/,
				    const std::string &friendlyName /* = ""*/,
				    const std::string &moduleLabel /* = ""*/,
				    const std::string &instanceName /* = ""*/,
				    const std::string &processName /* = "" */)
    : VisQueuedTwig (state, parent, name),
      m_text (name),
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
    edm::TypeID simVertexID (typeid (edm::SimVertexContainer));
    tfService->registerTwig (simVertexID.friendlyClassName (), &createThisTwig);
}

void
VisSimVertexTwig::onNewEvent (const edm::Event &event,
			      const edm::EventSetup &eventSetup)
{
    LOG (1, trace, LFfwvis, "VisSimVertexTwig[" << name ()
	 << "]::onNewEvent()\n");
        
    VisQueuedTwig::onBaseInvalidate ();

    m_vertices.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<edm::SimVertexContainer> > vertexCollections;
    
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, vertexCollections);
	}
	else
	{
	    event.getManyByType (vertexCollections);
	}
    }
    catch (...) 
    {
    }

    if (! vertexCollections.empty ())
    {
	std::vector<edm::Handle<edm::SimVertexContainer> >::iterator i;
	for (i = vertexCollections.begin (); i != vertexCollections.end (); i++) 
	{
	    const edm::SimVertexContainer& c = *(*i);

	    for (std::vector<SimVertex>::const_iterator isim = c.begin ();
		 isim != c.end (); ++isim)
	    {
		m_vertices.push_back ((*isim));
	    }    
	}
    }
    
    IgRepSet::invalidate (this, SELF_MASK);
}

void
VisSimVertexTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "Total " << m_vertices.size () << " vertices from";
    text << " run: " << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Position</TH>"
	 << "<TH>Parent Index</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if ((! m_vertices.empty ())) 
    {
	int nVertices = 0;
	try 
	{
	    for (std::vector<SimVertex>::const_iterator i = m_vertices.begin (); i != m_vertices.end (); ++i) 
	    {
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nVertices++ << "</TD>"
		     << "<TD>" << std::setw (3) << "(" << ((*i).position ().x () / 100.0) << ", "
		                                       << ((*i).position ().y () / 100.0) << ", "
						       << ((*i).position ().z () / 100.0) << ")</TD>"
		     << "<TD>" << std::setw (3) << (*i).parentIndex () << "</TD></TR>";
	    }
	}
	catch (...) 
	{
	    text << "No SimVertices.<br>";
	}
	text << "</table>";
    }

    // Send it over.
    IgQtLock ();
    rep->setText (text.str ());
}

void
VisSimVertexTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);      
    IgQtLock ();
    rep->clear ();

    if ((! m_vertices.empty ())) 
    {
	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (1.0, 0.0, 1.0);
	mat->emissiveColor.setValue(1.0, 0.0, 1.0);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 2.0;

	SoSeparator *sep = new SoSeparator; // global
	sep->addChild (mat);
	sep->addChild (drawStyle);

	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrtx = 0;

	    for (std::vector<SimVertex>::const_iterator i = m_vertices.begin (); i != m_vertices.end (); ++i) 
	    {
		vertices->vertex.set1Value (nVrtx++, SbVec3f ((*i).position ().x () / 100.0, (*i).position ().y () / 100.0, (*i).position ().z () / 100.0));
	    }
	    vertices->vertex.setNum (nVrtx);
	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrtx);

	    sep->addChild (points);
	}
	catch (...) 
	{
	}
	rep->node ()->addChild (sep);
    }
}

void
VisSimVertexTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisSimVertexTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisSimVertexTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

