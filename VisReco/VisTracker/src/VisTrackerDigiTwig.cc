//<<<<<< INCLUDES                                                       >>>>>>
#include "VisReco/VisTracker/interface/VisTrackerDigiTwig.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <qstring.h>
#include <iostream>
#include <iomanip>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

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
	IgTwig *rootTwig = IgDocumentData::get (state)->root ();
	IgTwig *eventTwig = 0;
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Tracker Event/SiStrip Digis");
	
	if (! eventTwig)
	    eventTwig = parent;

	return new VisTrackerDigiTwig (state, eventTwig, "[N/A] SiStrip Digi (" + name + ")", 
				       friendlyName, modLabel, instanceName, processName);
    }
}

VisTrackerDigiTwig::VisTrackerDigiTwig (IgState *state, IgTwig *parent,
					const std::string &name /* = "" */,
					const std::string &friendlyName /* = "" */,
					const std::string &moduleLabel /* = "" */,
					const std::string &instanceName /* = "" */,
					const std::string &processName /* = "" */)
    : VisQueuedTwig (state, parent, name),
      m_text (name),
      m_friendlyName (friendlyName),
      m_moduleLabel (moduleLabel),
      m_instanceName (instanceName),
      m_processName (processName),
      m_scale (state, lat::CreateCallback (this, &VisTrackerDigiTwig::twigChanged))      
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID siDigiCollID (typeid (edm::DetSetVector<SiStripDigi>));
    tfService->registerTwig (siDigiCollID.friendlyClassName (), &createThisTwig);
}

void
VisTrackerDigiTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisTrackerDigiTwig::onNewEvent (const edm::Event &event,
				const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_digis.clear ();
  
    m_text = QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ())
	     .arg (event.luminosityBlock ())
	     .arg (event.orbitNumber ())
	     .arg (event.bunchCrossing ())
	     .latin1 ();

    std::vector< edm::Handle<edm::DetSetVector<SiStripDigi> > > collections;
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, collections);
	}
	eventSetup.get<TrackerDigiGeometryRecord> ().get (m_pDD);
    }
    catch (lat::Error& e)
    {
	if (this->m_onError)
	    this->m_onError (&e);
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTrackerDigiTwig: ");
	e.append (this->name ());    
	
	if (this->m_onCmsException)
	    this->m_onCmsException (&e);
    }
    catch (std::exception& e)
    {
	if (this->m_onException)
	    this->m_onException (&e);
    }
    catch (...)
    {
	if (this->m_onUnhandledException)
	    this->m_onUnhandledException ();
    }

    if (! collections.empty ())
    {
	std::vector< edm::Handle<edm::DetSetVector<SiStripDigi> > >::iterator i;
	std::vector< edm::Handle<edm::DetSetVector<SiStripDigi> > >::iterator iEnd;
	for (i = collections.begin (), iEnd = collections.end (); i != iEnd; ++i) 
	{
	    const edm::DetSetVector<SiStripDigi> &c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    
	    this->name (nameStr.toStdString ());
	  
	    edm::DetSetVector<SiStripDigi>::const_iterator DSViter = c.begin ();
	    edm::DetSetVector<SiStripDigi>::const_iterator DSViterEnd = c.end ();

	    LOG (2, trace, LFfwvis, "There are " <<  c.size () << "SiStrip digis\n");

	    for (; DSViter != DSViterEnd; ++DSViter)
	    {
		edm::DetSet<SiStripDigi> ds = *DSViter;
		if (ds.data.size ())
		{
		    const uint32_t& detID = ds.id;
		    DetId detid (detID);
		    edm::DetSet<SiStripDigi>::const_iterator idigi = ds.data.begin ();
		    edm::DetSet<SiStripDigi>::const_iterator idigiEnd = ds.data.end ();
		    for(; idigi != idigiEnd; ++idigi)
		    { 
			std::pair<DetId, SiStripDigi> p;
			p.first = detid;
			p.second = (*idigi);
			m_digis.push_back (p);
		    } // for digis 
		}
	    }
	}
    }

    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisTrackerDigiTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    // Prepare property description.
    std::ostringstream  text;

    text << m_text << "<br>";
  
    text << "Total " << m_digis.size () << " Silicon Strip digis from ";
    text << m_text << "<br>";

    if (m_digis.size () > 100) 
    {
	text << "Printing only first 100." << "<br>";
    } 
    
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>GeomdetId</TH>"
	 << "<TH>Position</TH>"
	 << "<TH>Charge</TH>"
	 << "<TH>Strip</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
  
    if ((! m_digis.empty ()) && m_pDD.isValid ()) 
    {
	int nSDigis = 0;
	try 
	{
	    for (std::vector<std::pair<DetId, SiStripDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	    {
		const DetId& id = (*i).first;
		const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit*>(m_pDD->idToDet (id));
		const StripTopology * theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
		int strip = (*i).second.strip ();
		GlobalPoint pos =  (m_pDD->idToDet (id))->surface().toGlobal(theStripTopol->localPosition(strip));
	      
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nSDigis++ << "</TD>"
		     << "<TD>" << std::setw (11) << ((*i).first).rawId ()<< "</TD>"
		     << "<TD>" << pos.x () << ", " << pos.y () << ", " << pos.z () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.adc () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.strip () << "</TD>";
		if (nSDigis > 100) break;
	    }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerDigiTwig: ");
	    e.append (this->name ());    
	
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
VisTrackerDigiTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
  
    if (! m_digis.empty () && m_pDD.isValid ())
    {
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x03C03C00, rgbcomponents); // Dark pastel green
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
      
	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 4.0;
      
	SoSeparator *sep = new SoSeparator; // global
	sep->addChild (mat);
	sep->addChild (drawStyle);
      
	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrtx = 0;
	  
	    for (std::vector<std::pair<DetId, SiStripDigi> >::const_iterator 
		     i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i)
	    {
		const DetId& id = (*i).first;
		const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit*>(m_pDD->idToDet (id));
		const StripTopology * theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
		int strip=(*i).second.strip ();
		GlobalPoint pos =  (m_pDD->idToDet (id))->surface().toGlobal(theStripTopol->localPosition(strip));
		float x = pos.x () / 100.0;  // cm -> m
		float y = pos.y () / 100.0;  // cm -> m
		float z = pos.z () / 100.0;  // cm -> m
		vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
	    }
	    vertices->vertex.setNum (nVrtx);
	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrtx);
	  
	    sep->addChild (points);
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerDigiTwig: ");
	    e.append (this->name ());    
	
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

	rep->node ()->addChild (sep);
    }
}

void
VisTrackerDigiTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    rep->clear ();
  
    SoSeparator *sep = new SoSeparator; // global
    try 
    {
	for (std::vector<std::pair<DetId, SiStripDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	{
	    const DetId& id = (*i).first;
	    const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit*>(m_pDD->idToDet (id));
	    const StripTopology * theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
	    int strip=(*i).second.strip ();
	    GlobalPoint pos =  (m_pDD->idToDet (id))->surface().toGlobal(theStripTopol->localPosition(strip));
	    double eta = pos.eta ();
	    double phi = pos.phi ();
	    if (phi < 0.0) phi = 2 * M_PI + phi; // correction in absence of one convention
	    int adcCounts = (*i).second.adc ();
	  
	    IgSoTower *tower = new IgSoTower;
	    tower->position = SbVec2f (phi, eta); // eta, phi
	    tower->etaWidth = 0.01;
	    tower->phiWidth = 0.01;
	    tower->scaleFactor = m_scale.value ();
	    tower->energy = adcCounts * 250.0 * 3.61e-09;
	    tower->orderedRGBA.set1Value (6, SbColor (0.0, 1.0, 0.0).getPackedValue ());
	    sep->addChild (tower);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTrackerDigiTwig: ");
	e.append (this->name ());    
	
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

    rep->node ()->addChild (sep);
}

void
VisTrackerDigiTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    rep->clear ();
  
    if (! m_digis.empty () && m_pDD.isValid ()) 
    {
	SoSeparator *sep = new SoSeparator;    
      
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x03C03C00, rgbcomponents); // Dark pastel green
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
      
	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 4.0;
	sep->addChild (drawStyle);
      
	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrtx = 0;
	  
	    for (std::vector<std::pair<DetId, SiStripDigi> >::const_iterator 
		     i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i)
	    {
		const DetId& id = (*i).first;
		const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit *>(m_pDD->idToDet (id));
		// Draw only if in barrel.
  	        int subdet = theStripDet->geographicalId ().subdetId ();
  	        if (subdet == 1 || subdet == 3 || subdet == 5)
		{
		    const StripTopology * theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
		    int strip = (*i).second.strip ();
		    GlobalPoint pos =  (m_pDD->idToDet (id))->surface ().toGlobal(theStripTopol->localPosition (strip));
		    float x = pos.x () / 100.0;  // cm -> m
		    float y = pos.y () / 100.0;  // cm -> m
		    float z = 2.0;  // Z layer
		    vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
		}
	    }
	  
	    vertices->vertex.setNum (nVrtx);
	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrtx);
	  
	    sep->addChild (points);
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerDigiTwig: ");
	    e.append (this->name ());    
	
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

	rep->node ()->addChild (sep);
    }
}

void
VisTrackerDigiTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    rep->clear ();
  
    if (! m_digis.empty () && m_pDD.isValid ()) 
    {
	SoSeparator *sep = new SoSeparator;    
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x03C03C00, rgbcomponents); // Dark pastel green
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
      
	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 4.0;
	sep->addChild (drawStyle);
      
	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrx = 0;
	    for (std::vector<std::pair<DetId, SiStripDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	    {
		const DetId& id = (*i).first;
		const StripGeomDetUnit *theStripDet = dynamic_cast<const StripGeomDetUnit*>(m_pDD->idToDet (id));
		const StripTopology *theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
		int strip = (*i).second.strip ();
		GlobalPoint pos =  (m_pDD->idToDet (id))->surface ().toGlobal(theStripTopol->localPosition (strip));
                float x = -2.0;
                float y = sqrt (pos.x () * pos.x () + pos.y () * pos.y ()) / 100.0; 
		if (pos.y () < 0.) y = - y;
		float z = pos.z () / 100.0;  // cm -> m
	      
		vertices->vertex.set1Value (nVrx++, SbVec3f (x, y, z));
	    }
	    vertices->vertex.setNum (m_digis.size ());
	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (m_digis.size ());
	    sep->addChild (points);
	}    
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerDigiTwig: ");
	    e.append (this->name ());    
	
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

	rep->node ()->addChild (sep);
    }
}
