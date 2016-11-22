//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuonRPC/interface/VisRPCDigiTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/RPCDigi/interface/RPCDigi.h"
#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "DataFormats/GeometrySurface/interface/RectangularPlaneBounds.h"
#include "DataFormats/GeometrySurface/interface/TrapezoidalPlaneBounds.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoFont.h>
#include <classlib/utils/DebugAids.h>
#include <qstring.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

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
	return new VisRPCDigiTwig (state, parent, "RPC digis (" + name + ")", 
				   friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisRPCDigiTwig::VisRPCDigiTwig (IgState *state, IgTwig *parent,
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
      m_processName (processName)
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID digiCollID (typeid (RPCDigiCollection));
    tfService->registerTwig (digiCollID.friendlyClassName (), &createThisTwig);
}

void
VisRPCDigiTwig::onNewEvent (const edm::Event &event,
			    const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_digis.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<RPCDigiCollection> > digiCollections;
    
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, digiCollections);
	}

	eventSetup.get<MuonGeometryRecord> ().get (m_pDD);
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

    if (! digiCollections.empty ())
    {
	std::vector<edm::Handle<RPCDigiCollection> >::iterator i;
	std::vector<edm::Handle<RPCDigiCollection> >::iterator iEnd;
	for (i = digiCollections.begin (), iEnd = digiCollections.end (); i != iEnd; ++i) 
	{
	    const RPCDigiCollection& c = *(*i);
    
	    // Loop over the DetUnits with digis
	    RPCDigiCollection::DigiRangeIterator detUnitIt;
	    RPCDigiCollection::DigiRangeIterator detUnitItEnd;
	    for (detUnitIt = c.begin (), detUnitItEnd = c.end (); detUnitIt != detUnitItEnd; ++detUnitIt)
	    {
		const RPCDetId &id = (*detUnitIt).first;
		const RPCDigiCollection::Range &range = (*detUnitIt).second;

		// Loop over the digis of this DetUnit
		for (RPCDigiCollection::const_iterator digiIt = range.first;
		     digiIt != range.second;
		     ++digiIt)
		{
		    std::pair< RPCDetId, RPCDigi > p;
		    p.first = id;
		    p.second = (*digiIt);
		    
		    m_digis.push_back (p);
		} // for digis in layer
	    } // for layers 
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisRPCDigiTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "Total " << m_digis.size () << " RPC strip digis from";
    text << " Cosmic run: " << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Position</TH>"
	 << "<TH>Strip</TH>"
	 << "<TH>BX</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if ((! m_digis.empty ()) && m_pDD.isValid ()) 
    {   
	int nDigis = 0;
	try 
	{
	    for (std::vector<std::pair<RPCDetId, RPCDigi> >::const_iterator i = m_digis.begin (); i != m_digis.end (); ++i) 
	    {
		Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << ++nDigis << "</TD>"
		     << "<TD>" << pos.x () << ", " << pos.y () << ", " << pos.z () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.strip () << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).second.bx () << "</TD></TR>";
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
VisRPCDigiTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if (! m_digis.empty () && m_pDD.isValid ())
    {
	const float width  = 0.01; // strip visualization 
	const float depth  = 0.01; // strip visualization

	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (1.0, 1.0, 2.0); // white
	mat->emissiveColor.setValue(1.0, 1.0, 2.0);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 1.0; // big point; useful for the centers of the layers

	SoSeparator *sep = new SoSeparator; // global
	sep->addChild (mat);
	sep->addChild (drawStyle);

	try 
	{
	    SoMaterial *matl = new SoMaterial;
	    //matl->diffuseColor.setValue (0.7, 1.0, 0.0); // yellow
	    // matl->emissiveColor.setValue(0.7, 1.0, 0.0);
	    matl->diffuseColor.setValue (1, 0.0, 0.0); // yellow
	    matl->emissiveColor.setValue(1, 0.0, 0.0);
	    matl->shininess = 1.;

	    SoDrawStyle *drawStylel = new SoDrawStyle;
	    drawStylel->pointSize = 10.;

	    SoSeparator	*pulses = new SoSeparator; // strips
	    pulses->addChild (matl);
	    pulses->addChild (drawStylel);

	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrtx = 0;
	    for (std::vector<std::pair<RPCDetId, RPCDigi> >::const_iterator 
		     i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i)
	    {
		const RPCDetId& id = (*i).first;

		// This is the center of the (active) layer 
		Surface::GlobalPoint ccpos =  (m_pDD->idToDet (id))->surface ().position ();
		float x = ccpos.x () / 100.0;  // cm -> m
		float y = ccpos.y () / 100.0;  // cm -> m
		float z = ccpos.z () / 100.0;  // cm -> m
		vertices->vertex.set1Value (++nVrtx, SbVec3f (x, y, z));

		const RPCRoll *roll = m_pDD->roll (id);
		const BoundSurface &bSurface = roll->surface ();
		const Topology &top = roll->topology ();
		
		// Trapezoidal
		const Bounds *b = &(bSurface.bounds ());
		if (dynamic_cast<const TrapezoidalPlaneBounds *> (b))
		{
		    const TrapezoidalPlaneBounds *b2 = dynamic_cast<const TrapezoidalPlaneBounds *> (b);
		    const std::vector< float > trapezBounds = b2->parameters ();

		    // This is a frame around the (active) layer
		    std::vector<Surface::GlobalPoint> crossPoint;
		    crossPoint.push_back ((m_pDD->idToDet (id))->surface ().toGlobal
					  (LocalPoint (-trapezBounds [0], -trapezBounds [3], 0.0)));
		    crossPoint.push_back ((m_pDD->idToDet (id))->surface ().toGlobal
					  (LocalPoint ( trapezBounds [0], -trapezBounds [3], 0.0)));
		    crossPoint.push_back ((m_pDD->idToDet (id))->surface ().toGlobal
					  (LocalPoint ( trapezBounds [1],  trapezBounds [3], 0.0)));
		    crossPoint.push_back ((m_pDD->idToDet (id))->surface ().toGlobal
					  (LocalPoint (-trapezBounds [1],  trapezBounds [3], 0.0)));
		    SoVertexProperty *vert = new SoVertexProperty;
		    int ncVrx = 0;
		    for (std::vector<Surface::GlobalPoint>::iterator crossPit 
			     = crossPoint.begin (); crossPit != crossPoint.end (); ++crossPit)
		    {
			x = crossPit->x () / 100.0;  // cm -> m
			y = crossPit->y () / 100.0;  // cm -> m
			z = crossPit->z () / 100.0;  // cm -> m
			vert->vertex.set1Value (ncVrx++, SbVec3f (x, y, z));
		    }
		    std::vector<Surface::GlobalPoint>::reference closeCircle 
			=  crossPoint.front ();
		    x = closeCircle.x () / 100.0;  // cm -> m
		    y = closeCircle.y () / 100.0;  // cm -> m
		    z = closeCircle.z () / 100.0;  // cm -> m
		    vert->vertex.set1Value (ncVrx++, SbVec3f (x, y, z));

		    vert->vertex.setNum (ncVrx);
		    SoLineSet *trapez = new SoLineSet;
		    trapez->vertexProperty.setValue (vert);
		    trapez->numVertices [crossPoint.size () + 1];

		    SoMaterial *mats = new SoMaterial;
		    mats->diffuseColor.setValue (2.0, 1.0, 1.0); // white
		    mats->emissiveColor.setValue(2.0, 1.0, 1.0);

		    SoSeparator *shape = new SoSeparator;// layer
		    shape->addChild (mats);
		    shape->addChild (trapez);
			
		    // Visualize the layer bounds
		    sep->addChild (shape);
			
		    //
		    int stripId = ((*i).second).strip ();

		    if (dynamic_cast<const StripTopology *> (&top))
		    {
			const StripTopology *stop = dynamic_cast<const StripTopology *> (&top);
			//			LocalPoint spos = stop->localPosition (stripId);
			LocalPoint spos = roll->centreOfStrip(stripId);
						
			Surface::GlobalPoint pos = (m_pDD->idToDet (id))->surface ().toGlobal (spos);

			// for Vis. purposes
			x = pos.x () / 100.0;  // cm -> m
			y = pos.y () / 100.0;  // cm -> m
			z = pos.z () / 100.0;  // cm -> m
			//
			SoTranslation	*inc = new SoTranslation;
			inc->translation = SbVec3f (x, y, z);
			//
			SbVec3f axis (0.0, 0.0, 1.0);
			float angle = -atan (x / y);
			SbRotation r (axis, angle);
			//
			SoTransform *xform = new SoTransform;
			xform ->rotation = r;
			//  
			SoCube *hit = new SoCube;
			hit->width  = width;
			hit->height = stop->stripLength () / 100.;
			hit->depth  = depth;
			// 
			SoSeparator	*pulse = new SoSeparator;// strip
			pulse->addChild (inc);
			pulse->addChild (xform);
			pulse->addChild (hit);
			pulses->addChild (pulse);
			// Visualize strips with signal
			sep->addChild (pulses);// strips
		    }
		}


		//AGGIUNTO DA NOI
		// Rectangular
		const Bounds *b1 = &(bSurface.bounds ());
		if (dynamic_cast<const RectangularPlaneBounds *> (b1))
		{
		    const RectangularPlaneBounds *b2 = dynamic_cast<const RectangularPlaneBounds *> (b1);
		    //		    const std::vector< float > trapezBounds = b2->parameters ();

		    // This is a frame around the (active) layer
		    std::vector<Surface::GlobalPoint> crossPoint;
		    crossPoint.push_back ((m_pDD->idToDet (id))->surface ().toGlobal
					  (LocalPoint (-b2->width()/2, -b2->length()/2, 0.0)));
		    crossPoint.push_back ((m_pDD->idToDet (id))->surface ().toGlobal
					  (LocalPoint (b2->width()/2, -b2->length()/2, 0.0)));
		    crossPoint.push_back ((m_pDD->idToDet (id))->surface ().toGlobal
					  (LocalPoint (b2->width()/2, b2->length()/2, 0.0)));
		    crossPoint.push_back ((m_pDD->idToDet (id))->surface ().toGlobal
					  (LocalPoint (-b2->width()/2, b2->length()/2, 0.0)));
		    SoVertexProperty *vert = new SoVertexProperty;
		    int ncVrx = 0;
		    for (std::vector<Surface::GlobalPoint>::iterator crossPit 
			     = crossPoint.begin (); crossPit != crossPoint.end (); ++crossPit)
		    {
			x = crossPit->x () / 100.0;  // cm -> m
			y = crossPit->y () / 100.0;  // cm -> m
			z = crossPit->z () / 100.0;  // cm -> m
			vert->vertex.set1Value (ncVrx++, SbVec3f (x, y, z));
		    }
		    std::vector<Surface::GlobalPoint>::reference closeCircle 
			=  crossPoint.front ();
		    x = closeCircle.x () / 100.0;  // cm -> m
		    y = closeCircle.y () / 100.0;  // cm -> m
		    z = closeCircle.z () / 100.0;  // cm -> m
		    vert->vertex.set1Value (ncVrx++, SbVec3f (x, y, z));

		    vert->vertex.setNum (ncVrx);
		    SoLineSet *trapez = new SoLineSet;
		    trapez->vertexProperty.setValue (vert);
		    trapez->numVertices [crossPoint.size () + 1];

		    SoMaterial *mats = new SoMaterial;
		    mats->diffuseColor.setValue (1.0, 0.0, 0.0); // white
		    mats->emissiveColor.setValue(1.0, 0.0, 0.0);

		    SoSeparator *shape = new SoSeparator;// layer
		    shape->addChild (mats);
		    shape->addChild (trapez);
			
		    // Visualize the layer bounds
		    sep->addChild (shape);
			
		    //
		    int stripId = ((*i).second).strip ();

		    if (dynamic_cast<const StripTopology *> (&top))
		    {
			const StripTopology *stop = dynamic_cast<const StripTopology *> (&top);
			//LocalPoint spos = stop->localPosition (stripId);
			LocalPoint spos = roll->centreOfStrip(stripId);

			Surface::GlobalPoint pos = (m_pDD->idToDet (id))->surface ().toGlobal (spos);

			// for Vis. purposes
			x = pos.x () / 100.0;  // cm -> m
			y = pos.y () / 100.0;  // cm -> m
			z = pos.z () / 100.0;  // cm -> m
			//
			SoTranslation	*inc = new SoTranslation;
			inc->translation = SbVec3f (x, y, z);
			//
			SbVec3f axis (1.0, 0.0, 0.0);
			float angle = 3.14/2;
			SbRotation r (axis, angle);
			//
			SoTransform *xform = new SoTransform;
 			xform ->rotation = r;
			//  
			SoCube *hit = new SoCube;
			hit->width  = width;
			hit->height = stop->stripLength () / 100.;
			hit->depth  = depth;
			// 
			SoSeparator	*pulse = new SoSeparator;// strip
			pulse->addChild (inc);
			pulse->addChild (xform);
			pulse->addChild (hit);
			pulses->addChild (pulse);
			// Visualize strips with signal
			sep->addChild (pulses);// strips
		    }
		}
	    }
	    
	    vertices->vertex.setNum (nVrtx);
	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrtx);

	    // Visualize the center of the layer
	    sep->addChild (points);
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
    
	rep->node ()->addChild (sep);
    }// end of if (m_digis.empty)
}

void
VisRPCDigiTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    
//     SoSeparator *sep = new SoSeparator;    
//     SoSeparator *posSep = new SoSeparator;    
//     SoSeparator *negSep = new SoSeparator;

//     SoMaterial *mat = new SoMaterial;
//     mat->diffuseColor.setValue (0.3, 0.8, 0.0);
//     posSep->addChild (mat);

//     SoMaterial *negMat = new SoMaterial;
//     negMat->diffuseColor.setValue (1.0, 0.0, 0.0);
//     negSep->addChild (negMat);

//     sep->addChild (posSep);
//     sep->addChild (negSep);
    
//     try 
//       {
// 	for (std::vector<std::pair<RPCDetId, RPCDigi> >::const_iterator i = m_digis.begin (); i != m_digis.end (); ++i) 
// 	  {
// 	  }
//       }
//     catch (...) 
//       {
// 	std::cout << "No RPCDigi Digis." << std::endl;
//       }
    
//     rep->node ()->addChild (sep);
}

void
VisRPCDigiTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    //
    rep->clear ();
    //
//     if (! m_digis.empty () && m_pDD.isValid ()) 
//     {
// 	SoSeparator *sep = new SoSeparator;    
// 	//
// 	SoMaterial *mat = new SoMaterial;
// 	mat->diffuseColor.setValue (0.0, 0.0, 0.0);
// 	sep->addChild (mat);
// 	try 
// 	{
// 	}
// 	catch (...) 
// 	{
// 	    std::cout << "No RPCDigi Digis." << std::endl;
// 	}
// 	//
// 	rep->node ()->addChild (sep);
//     }
}

void
VisRPCDigiTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if (! m_digis.empty ()) 
    {
	SoSeparator *sep = new SoSeparator;    
	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (0.3, 0.8, 0.0);
	sep->addChild (mat);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 3.0;
	sep->addChild (drawStyle);
    
	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrx = 0;

	    for (std::vector<std::pair<RPCDetId, RPCDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	    {
		Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
	    
		float x = pos.x () / 100.0;  // cm -> m
		float y = pos.y () / 100.0;  // cm -> m
		float z = pos.z () / 100.0;  // cm -> m
		
		vertices->vertex.set1Value (++nVrx, SbVec3f (x, y, z));
	    }
	    vertices->vertex.setNum (m_digis.size ());

	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (m_digis.size ());
	    sep->addChild (points);
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
	rep->node ()->addChild (sep);
    }
}
