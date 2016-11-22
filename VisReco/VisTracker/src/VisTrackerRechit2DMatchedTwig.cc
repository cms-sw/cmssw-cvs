//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisTracker/interface/VisTrackerRechit2DMatchedTwig.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "FWCore/Framework/interface/Event.h"
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
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMarkerSet.h>
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Tracker Event/SiStrip RecHit 2D Matched");
	
	if (! eventTwig)
	    eventTwig = parent;

	return new VisTrackerRechit2DMatchedTwig (state, eventTwig, "[N/A] SiStrip RecHit 2D Matched (" + name + ")", 
						  friendlyName, modLabel, instanceName, processName);
    }
}

VisTrackerRechit2DMatchedTwig::VisTrackerRechit2DMatchedTwig(IgState *state, IgTwig *parent,
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
      m_scale (state, lat::CreateCallback (this, &VisTrackerRechit2DMatchedTwig::twigChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID rechit2DMatchedCollID (typeid (SiStripMatchedRecHit2DCollection));
    tfService->registerTwig (rechit2DMatchedCollID.friendlyClassName (), &createThisTwig);
}

void
VisTrackerRechit2DMatchedTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisTrackerRechit2DMatchedTwig::onNewEvent (const edm::Event &event,
					   const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);
 
    m_hits.clear ();
   
    m_text = QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ())
	     .arg (event.luminosityBlock ())
	     .arg (event.orbitNumber ())
	     .arg (event.bunchCrossing ())
	     .latin1 ();

    std::vector< edm::Handle<SiStripMatchedRecHit2DCollection> > collections;
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, collections);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTrackerRechit2DMatchedTwig: ");
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

    if (! collections.empty ())
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	std::vector< edm::Handle<SiStripMatchedRecHit2DCollection> >::iterator i;
	std::vector< edm::Handle<SiStripMatchedRecHit2DCollection> >::iterator iEnd;
	for (i = collections.begin (), iEnd = collections.end (); i != iEnd; ++i) 
	{
	    const SiStripMatchedRecHit2DCollection&c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    
	    this->name (nameStr.toStdString ());
	  	  
            for (SiStripMatchedRecHit2DCollection::DataContainer::const_iterator matchedrechit = c.data().begin(); 
                matchedrechit != c.data().end(); ++matchedrechit)
            {
		    DetId detectorId = (*matchedrechit).geographicalId ();
		    unsigned int id = detectorId.rawId ();
		  
		    std::vector<LocalPoint> points;
		    esService->localPositions(&(*matchedrechit), points);

		    DetId detIdObject (id);
		    const SiStripCluster* Cluster_1 = 0 ;
		    if ((*matchedrechit).monoHit ()->cluster ().isNonnull ())
			Cluster_1 = ((*matchedrechit).monoHit ())->cluster ().get ();
		    else if ((*matchedrechit).monoHit ()->cluster_regional ().isNonnull ())
			Cluster_1 = ((*matchedrechit).monoHit ())->cluster_regional ().get ();
		    else continue;

		    LocalPoint & position_1 = points [0];

		    int clusterCharge = 0;
		    int clusterCharge_1 = 0;
		    int cas = Cluster_1->amplitudes ().size ();
		    for (int j = 0; j < cas; j++)
		    {
			clusterCharge += Cluster_1->amplitudes ()[j];
		    }
		    clusterCharge_1 = clusterCharge;

		    std::pair<GlobalPoint, int> pxd_1;
		    pxd_1.first = esService->toGlobal (position_1, DetId (Cluster_1->geographicalId ()));		    
		    pxd_1.second = clusterCharge_1;
		    m_hits.push_back (pxd_1);
		  
		    const SiStripCluster* Cluster_2 = 0;
		    if ((*matchedrechit).stereoHit ()->cluster ().isNonnull ())
		      Cluster_2 = ((*matchedrechit).stereoHit ())->cluster ().get ();
		    else if ((*matchedrechit).stereoHit ()->cluster_regional ().isNonnull ())
		      Cluster_2 = ((*matchedrechit).stereoHit ())->cluster_regional ().get ();
		    else continue;

		    LocalPoint & position_2 = points[1];

		    int clusterCharge_2 = 0;
		    int cus = Cluster_2->amplitudes ().size ();
		    for (int j = 0; j < cus; j++)
		    {
			clusterCharge +=Cluster_2->amplitudes()[j];
		    }
		    clusterCharge_2 = clusterCharge - clusterCharge_1;
		    clusterCharge = clusterCharge / 2;

		    std::pair<GlobalPoint, int> pxd_2;		    
		    pxd_2.first = esService->toGlobal (position_2, DetId (Cluster_2->geographicalId ()));		    
		    pxd_2.second = clusterCharge_2;
		    m_hits.push_back (pxd_2);
		}
	}
    } 
    
    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisTrackerRechit2DMatchedTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "Total " << m_hits.size () << " Silicon Strip matched 2D rechits from ";
    text << m_text << "<br>";
    if (m_hits.size () > 100)
    {
	text << "Printing only first 100." << "<br>"; 
    }

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Position (m): x, y, z</TH>"
	 << "<TH>Amplitude</TH>"
	 << "</TR>";

    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if (! m_hits.empty ()) 
    {
	int nRecHits = 0;
	try 
	{
	    for (std::vector<std::pair<GlobalPoint, int> >::const_iterator i = m_hits.begin (), iEnd = m_hits.end (); i != iEnd; ++i) 
	    {
		GlobalPoint pos = (*i).first;
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << ++nRecHits << "</TD>"
		     << "<TD>" << pos.x () / 100 << ", " << pos.y () / 100 << ", " << pos.z () / 100 << "</TD>"
		     << "<TD>" << std::setw (11) << (*i).second << "</TD></TR>";
		if (nRecHits > 100) break;
	    }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerRechit2DMatchedTwig: ");
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

	text << "</table>";
    }
    
    // Send it over.
    rep->setText (text.str ());
}


void
VisTrackerRechit2DMatchedTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
  
    if (! m_hits.empty ())
    {
	SoSeparator *sep = new SoSeparator; // global

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x238E2300, rgbcomponents); // Forest Green
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 4.0;
	sep->addChild (drawStyle);

	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrtx = 0;

	    for (std::vector<std::pair<GlobalPoint, int> >::const_iterator 
		     i = m_hits.begin (), iEnd = m_hits.end (); i != iEnd; ++i)
	    {
		GlobalPoint pos = (*i).first;
		float x = pos.x () / 100.0;  // cm -> m
		float y = pos.y () / 100.0;  // cm -> m
		float z = pos.z () / 100.0;  // cm -> m

		vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
	    }
	    
	    vertices->vertex.setNum (nVrtx);
	    SoMFInt32 markerIndex;
	    markerIndex.setValue (SoMarkerSet::CROSS_9_9);
	    
	    SoMarkerSet *points = new SoMarkerSet;
	    points->markerIndex = markerIndex;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrtx);

	    sep->addChild (points);
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerRechit2DMatchedTwig: ");
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
VisTrackerRechit2DMatchedTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator; // global

    try 
    {
	for (std::vector<std::pair<GlobalPoint, int> >::const_iterator i = m_hits.begin (), iEnd = m_hits.end (); i != iEnd; ++i) 
	{
	    GlobalPoint pos =  (*i).first;

	    double eta = pos.eta ();
	    double phi = pos.phi ();
	    if (phi < 0.0) phi = 2 * M_PI + phi; // correction in absence of one convention
	    
	    int recCharge = (*i).second;
	 	    
	    IgSoTower *tower = new IgSoTower;
	    tower->position = SbVec2f (phi, eta); // eta, phi
	    tower->etaWidth = 0.01;
	    tower->phiWidth = 0.01;
	    tower->scaleFactor = m_scale.value ();
	    tower->energy = recCharge * 250.0 * 3.61e-09;
	    tower->orderedRGBA.set1Value (6, SbColor (0.0, 1.0, 0.0).getPackedValue ());

	    sep->addChild (tower);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTrackerRechit2DMatchedTwig: ");
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
VisTrackerRechit2DMatchedTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    if (! m_hits.empty ()) 
    {
	SoSeparator *sep = new SoSeparator;    

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x238E2300, rgbcomponents); // Forest Green
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 8.0;
	sep->addChild (drawStyle);

	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrtx = 0;
	    for (std::vector<std::pair<GlobalPoint, int> >::const_iterator i = m_hits.begin (), iEnd = m_hits.end (); i != iEnd; ++i)
	    {
		GlobalPoint pos =  (*i).first;
		float x = pos.x () / 100.0;  // cm -> m
		float y = pos.y () / 100.0;  // cm -> m
		float z = 3.2;  // cm -> m

		vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
	    }
	    
	    vertices->vertex.setNum (nVrtx);

	    SoMFInt32 markerIndex;
	    markerIndex.setValue (SoMarkerSet::CROSS_9_9);

	    SoMarkerSet *points = new SoMarkerSet;
	    points->markerIndex = markerIndex;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrtx);

	    sep->addChild (points);
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerRechit2DMatchedTwig: ");
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
VisTrackerRechit2DMatchedTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_hits.empty ()) 
    {
	SoSeparator *sep = new SoSeparator;    
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x238E2300, rgbcomponents); // Forest Green
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 8.0;
	sep->addChild (drawStyle);
    
	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrx = 0;

	    for (std::vector<std::pair<GlobalPoint, int> >::const_iterator i = m_hits.begin (), iEnd = m_hits.end (); i != iEnd; ++i) 
	    {
		GlobalPoint pos =  (*i).first;
                float x = -3.2;
                float y = sqrt (pos.x () * pos.x () + pos.y () * pos.y ()) / 100.0; 
		(pos.y () < 0.) ? y = -y : y;
		float z = pos.z () / 100.0;  // cm -> m
		
		vertices->vertex.set1Value (nVrx++, SbVec3f (x, y, z));
	    }

	    vertices->vertex.setNum (nVrx);
	    SoMFInt32 markerIndex;
	    markerIndex.setValue (SoMarkerSet::CROSS_9_9);

	    SoMarkerSet *points = new SoMarkerSet;
	    points->markerIndex = markerIndex;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrx);
	    sep->addChild (points);
	}    
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerRechit2DMatchedTwig: ");
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

