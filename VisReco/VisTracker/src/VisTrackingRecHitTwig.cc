//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisTracker/interface/VisTrackingRecHitTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <qstring.h>
#include <iostream>
#include <iomanip>

#include "VisReco/VisTracker/interface/VisTrackerRechit2DTwig.h"

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
	IgTwig *rootTwig = IgDocumentData::get (state)->root ();
	IgTwig *eventTwig = 0;
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Tracker Event/Tracking RecHits");
	
	if (! eventTwig) 
	    eventTwig = parent;

	return new VisTrackingRecHitTwig (state, eventTwig, "[N/A] Tracking RecHit (" + name + ")", 
					  friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisTrackingRecHitTwig::VisTrackingRecHitTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID collectionID (typeid (TrackingRecHitCollection));
    tfService->registerTwig (collectionID.friendlyClassName (), &createThisTwig);
}

void
VisTrackingRecHitTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisTrackingRecHitTwig::onNewEvent (const edm::Event &event,
				   const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_text = QString ("Run # %1, event # %2")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ()).latin1 ();
    m_recHits.clear ();
    
    std::vector<edm::Handle<TrackingRecHitCollection> > collections;

    try 
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, collections);
	}
// 	else
// 	{
// 	    event.getManyByType (collections);
// 	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTrackingRecHitTwig: ");
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
    if (! m_trackingGeometry.isValid ())
    {
	try
	{ 
	    eventSetup.get<GlobalTrackingGeometryRecord> ().get (m_trackingGeometry);
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackingRecHitTwig: ");
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
    
    if (! collections.empty ())
      {
        std::vector<edm::Handle<TrackingRecHitCollection> >::const_iterator i (collections.begin ()), end (collections.end ());
	for (; i != end; ++i)
	{
	    try
	    { 
		const TrackingRecHitCollection c = *(*i).product ();
		QString sizeStr = (QString ("%1").arg (c.size ()));
		QString nameStr = QString::fromStdString (this->name ());
		int ib = nameStr.find ("[");
		int ie = nameStr.find ("]");
		nameStr.replace (ib + 1, ie - 1, sizeStr);
		this->name (nameStr.toStdString ());
	    
		if (m_trackingGeometry.isValid ())
		{
		    for (TrackingRecHitCollection::const_iterator hit = c.begin (), hitEnd = c.end (); 
			 hit != hitEnd; ++hit)
		    {
			if ((*hit).isValid () && !(*hit).geographicalId ().null ())
			{
			    LocalPoint point = VisTrackerRechit2DTwig::localPosition(&(*hit), m_trackingGeometry.product());
			    float x = m_trackingGeometry->idToDet ((*hit).geographicalId ())->surface ().toGlobal ( point).x () / 100.0;
			    float y = m_trackingGeometry->idToDet ((*hit).geographicalId ())->surface ().toGlobal ( point).y () / 100.0;
			    float z = m_trackingGeometry->idToDet ((*hit).geographicalId ())->surface ().toGlobal ( point).z () / 100.0;

			    m_recHits.push_back (SbVec3f (x, y, z));
			}
		    }
		}
	    }
	    catch (cms::Exception& e)
	    {
		e.append (" from VisTrackingRecHitTwig: ");
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
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisTrackingRecHitTwig::update (IgTextRep *rep)
    {
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;

    text << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>x, y, z</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    int n = 1;
    
    for (std::vector<SbVec3f>::const_iterator it = m_recHits.begin (), itEnd = m_recHits.end (); it != itEnd; ++it) 
    {
	SbVec3f xyzPoint = (*it);
	float x = xyzPoint [0];
	float y = xyzPoint [1];
	float z = xyzPoint [2];
	text << "<TR align = right>"
	     << "<TD>" << std::setw (3) << n++ << "</TD>"
	     << "<TD>" << std::setw (2) << std::setprecision (3) << x << "," << y << "," << z  << "</TD>"
	     << "</TR>";
    }
    
    text << "</table>";
    
    // Send it over.
    rep->setText (text.str ());
}
 
void
VisTrackingRecHitTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.  
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_recHits.empty ())
    {
	SoSeparator *sep = new SoSeparator;    
      
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xee2c2c, rgbcomponents);
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

        //set line width
        SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 8.0;
	sep->addChild (drawStyle);
      
	SoVertexProperty *vertices = new SoVertexProperty;
	int nVrtx = 0;
	
	for (std::vector<SbVec3f>::const_iterator hit = m_recHits.begin (), hitEnd = m_recHits.end (); hit != hitEnd; ++hit)
	    vertices->vertex.set1Value (nVrtx++, (*hit));

	vertices->vertex.setNum (nVrtx);
	SoPointSet *points = new SoPointSet;
	points->vertexProperty.setValue (vertices);
	points->numPoints.setValue (nVrtx);
	  
	sep->addChild (points);

	rep->node ()->addChild (sep);
    }
}

void
VisTrackingRecHitTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
}

void
VisTrackingRecHitTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_recHits.empty ())
    {
	SoSeparator *sep = new SoSeparator;    
      
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xee2c2c, rgbcomponents);
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

        //set line width
        SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 8.0;
	sep->addChild (drawStyle);
      
	SoVertexProperty *vertices = new SoVertexProperty;
	int nVrtx = 0;
	
	for (std::vector<SbVec3f>::const_iterator hit = m_recHits.begin (), hitEnd = m_recHits.end (); hit != hitEnd; ++hit)
	    vertices->vertex.set1Value (nVrtx++, (*hit));

	vertices->vertex.setNum (nVrtx);
	SoPointSet *points = new SoPointSet;
	points->vertexProperty.setValue (vertices);
	points->numPoints.setValue (nVrtx);
	  
	sep->addChild (points);

	rep->node ()->addChild (sep);
    }
}

void
VisTrackingRecHitTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    if (! m_recHits.empty ())
    {
	SoSeparator *sep = new SoSeparator;    
      
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xee2c2c, rgbcomponents);
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

        //set line width
        SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 8.0;
	sep->addChild (drawStyle);
      
	SoVertexProperty *vertices = new SoVertexProperty;
	int nVrtx = 0;
	
	for (std::vector<SbVec3f>::const_iterator hit = m_recHits.begin (), hitEnd = m_recHits.end (); 
	     hit != hitEnd; ++hit)
	{
	    float x;
	    float y;
	    float z;
	    (*hit).getValue (x, y, z);
	    (y < 0.) ? y = - sqrt (x * x + y * y) : y = sqrt (x * x + y * y);
	    
	    vertices->vertex.set1Value (nVrtx++, SbVec3f (0.0, y, z));
	}
	
	vertices->vertex.setNum (nVrtx);
	SoPointSet *points = new SoPointSet;
	points->vertexProperty.setValue (vertices);
	points->numPoints.setValue (nVrtx);
	  
	sep->addChild (points);

	rep->node ()->addChild (sep);
    }
}
