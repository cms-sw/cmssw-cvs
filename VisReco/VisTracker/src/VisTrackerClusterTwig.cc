//<<<<<< INCLUDES                                                       >>>>>>
#include "VisReco/VisTracker/interface/VisTrackerClusterTwig.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
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
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Inventor/nodes/SoIndexedLineSet.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoTranslation.h>
#include "Inventor/nodes/SoIndexedLineSet.h"
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Tracker Event/SiStrip Clusters");
	
	if (! eventTwig)
	    eventTwig = parent;

      return new VisTrackerClusterTwig (state, eventTwig, "[N/A] SiStrip Cluster (" + name + ")", 
					friendlyName, modLabel, instanceName, processName);
    }
}

using namespace edm;
VisTrackerClusterTwig::VisTrackerClusterTwig (IgState *state, IgTwig *parent,
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
      m_clusterPointSize (state, lat::CreateCallback (this, &VisTrackerClusterTwig::twigChanged)),
      m_siStripClusterRep (state, lat::CreateCallback (this, &VisTrackerClusterTwig::twigChanged)),
      m_scale (state, lat::CreateCallback (this, &VisTrackerClusterTwig::twigChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID clusterCollID (typeid (edmNew::DetSetVector<SiStripCluster>));
    tfService->registerTwig (clusterCollID.friendlyClassName (), &createThisTwig);
}

void
VisTrackerClusterTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisTrackerClusterTwig::onNewEvent (const edm::Event &event,
				   const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);
    
    s_cluster.clear ();
  
    m_text = QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ())
	     .arg (event.luminosityBlock ())
	     .arg (event.orbitNumber ())
	     .arg (event.bunchCrossing ())
	     .latin1 ();

    std::vector< edm::Handle<edmNew::DetSetVector<SiStripCluster> > > cluster_collections;
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, cluster_collections);
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
	e.append (" from VisTrackerClusterTwig: ");
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
  
    if (! cluster_collections.empty ())
    {
	std::vector< edm::Handle<edmNew::DetSetVector<SiStripCluster> > >::iterator i;
	std::vector< edm::Handle<edmNew::DetSetVector<SiStripCluster> > >::iterator iEnd;
	for (i = cluster_collections.begin (), iEnd = cluster_collections.end (); i != iEnd; ++i) 
	{
	    const edmNew::DetSetVector<SiStripCluster> &c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    
	    this->name (nameStr.toStdString ());
	  
	    edmNew::DetSetVector<SiStripCluster>::const_iterator DSViter = c.begin ();
	    edmNew::DetSetVector<SiStripCluster>::const_iterator DSViterEnd = c.end ();

	    LOG (2, trace, LFfwvis, "There are " <<  c.size () << "SiStrip culsters\n");

	    for (; DSViter != DSViterEnd; ++DSViter)
	    {
		edmNew::DetSet<SiStripCluster> ds = *DSViter;
		const uint32_t detID = DSViter->detId();
		DetId detid (detID);
		edmNew::DetSet<SiStripCluster>::const_iterator icluster = DSViter->begin();
		edmNew::DetSet<SiStripCluster>::const_iterator iclusterEnd = DSViter->end();
		for (; icluster != iclusterEnd; ++icluster)
		{ 
		    std::pair<DetId, SiStripCluster> p;
		    p.first = detid;
		    p.second = (*icluster);
		    s_cluster.push_back (p);
		}
	    }
	}
    }

    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisTrackerClusterTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    // Prepare property description.
    std::ostringstream  text;

    text << m_text << "<br>";

    text << "Total " << s_cluster.size () << " Silicon Strip clusters from";

    if (s_cluster.size () > 100)
    {
	text << " input file: printing only first 100 " << "<br>";
    }
    else 
	text << " input file: " << m_text << "<br>";
  
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>GeomdetId</TH>"
	 << "<TH>Position</TH>"
	 << "<TH>ClusterSize</TH>"
	 << "<TH>Barycenter</TH>"
	 << "</TR>";
  
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
  
    if ((! s_cluster.empty ()) && m_pDD.isValid ()) 
    {     
	int nSClusters = 0;
	try 
	{
	    for (std::vector<std::pair<DetId, SiStripCluster> >::const_iterator i = s_cluster.begin (), iEnd = s_cluster.end (); i != iEnd; ++i) 
	    {
		const DetId& id = (*i).first;
	      
		SiStripCluster c = (*i).second;
		short firststrip = c.firstStrip (); 
		const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit*>((m_pDD->idToDet (id)));
		const StripTopology* theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
		GlobalPoint pos =  (m_pDD->idToDet (id))->surface ().toGlobal (theStripTopol->localPosition (firststrip));
	      
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nSClusters++ << "</TD>"
		     << "<TD>" << std::setw (11) << ((*i).first).rawId()<< "</TD>"
		     << "<TD>" << pos.x () << ", " << pos.y () << ", " << pos.z () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.amplitudes().size() << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.barycenter() << "</TD>";
		if (nSClusters>100) break;
	    }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerClusterTwig: ");
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
VisTrackerClusterTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
  
    if (! s_cluster.empty () && m_pDD.isValid ())
    {
	SoMaterial *mat = new SoMaterial;	
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x6600FF00, rgbcomponents); // Electric indigo
	mat->diffuseColor.setValue (SbColor (rgbcomponents));

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = m_clusterPointSize.value ();
      
	SoSeparator *sep = new SoSeparator; // global
	sep->addChild (mat);
	sep->addChild (drawStyle);
      
	try 
	{            
	    SoVertexProperty *vertices = new SoVertexProperty;
	    SoVertexProperty *vtx = new SoVertexProperty;
	    SoIndexedLineSet *strips = new SoIndexedLineSet;
	    int nVrtx = 0;
	    int n = 0;
	    int coord = 0;
	    SoMFInt32 coords;

	    for (std::vector<std::pair<DetId, SiStripCluster> >::const_iterator 
		     i = s_cluster.begin (), iEnd = s_cluster.end (); i != iEnd; ++i)
	    {
		const DetId& id = (*i).first;

	        SiStripCluster c = (*i).second;	
		short firststrip = c.firstStrip (); 
		const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit*>( (m_pDD->idToDet (id)));
		const StripTopology* theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
		GlobalPoint pos =  (m_pDD->idToDet (id))->surface().toGlobal(theStripTopol->localPosition(firststrip));
		if (m_siStripClusterRep.value () == "strip")
		{
		    MeasurementPoint mp1((float)firststrip,(float)-0.5);LocalPoint lp1=theStripTopol->localPosition(mp1); 
		    MeasurementPoint mp2((float)firststrip,(float)+0.5);LocalPoint lp2=theStripTopol->localPosition(mp2);
		    GlobalPoint pos1 =  (m_pDD->idToDet (id))->surface().toGlobal(lp1);
		    GlobalPoint pos2 =  (m_pDD->idToDet (id))->surface().toGlobal(lp2);
		    vtx->vertex.set1Value(n,SbVec3f(pos1.x()/100.0,pos1.y()/100.0,pos1.z()/100.0));
		    coords.set1Value (coord, n);
		    coord++;n++;
		    vtx->vertex.set1Value(n,SbVec3f(pos2.x()/100.0,pos2.y()/100.0,pos2.z()/100.0));
		    coords.set1Value (coord, n);
		    coord++;n++;
		    coords.set1Value (coord, SO_END_LINE_INDEX);
		    coord++;
                }

		//Surface::GlobalPoint pos =  (m_pDD->idToDet (id))->surface ().position ();
		float x = pos.x () / 100.0;  // cm -> m
		float y = pos.y () / 100.0;  // cm -> m
		float z = pos.z () / 100.0;  // cm -> m

		vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
	    }
	    
	    vertices->vertex.setNum (nVrtx);
	    SoMFInt32 markerIndex;
	    markerIndex.setValue (SoMarkerSet::CIRCLE_FILLED_5_5);

	    SoMarkerSet *points = new SoMarkerSet;
	    points->markerIndex = markerIndex;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrtx);

	    sep->addChild (points);
	    if (m_siStripClusterRep.value () == "strip")
	    {
		strips->vertexProperty = vtx;
		strips->coordIndex = coords;
		sep->addChild (strips);
            }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerClusterTwig: ");
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
VisTrackerClusterTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator; // global

    if (! s_cluster.empty () && m_pDD.isValid ())
    {    
	try 
	{
	    for (std::vector<std::pair<DetId, SiStripCluster> >::const_iterator i = s_cluster.begin (); i != s_cluster.end (); ++i) 
	    {
		const DetId& id = (*i).first;

		SiStripCluster c = (*i).second;
		short firststrip = c.firstStrip(); 
		const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit*>( (m_pDD->idToDet (id)));
		const StripTopology* theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
		GlobalPoint pos =  (m_pDD->idToDet (id))->surface().toGlobal(theStripTopol->localPosition(firststrip));

		double eta = pos.eta ();
		double phi = pos.phi ();
		if (phi < 0.0) phi = 2 * M_PI + phi; // correction in absence of one convention
	    
		int clusterCharge = 0;
		int sas = (*i).second.amplitudes ().size ();

		for (int j = 0; j < sas; j++)
		{
		    clusterCharge += (*i).second.amplitudes ()[j];
		}
	    
		IgSoTower *tower = new IgSoTower;
		tower->position = SbVec2f (phi, eta); // eta, phi
		tower->etaWidth = 0.02;
		tower->phiWidth = 0.02;
		tower->scaleFactor = m_scale.value ();
		tower->energy = clusterCharge  * 250.0 * 3.61e-09;
		tower->orderedRGBA.set1Value (6, SbColor (0.4, 0.0, 1.0).getPackedValue ());
		sep->addChild (tower);
	    }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerClusterTwig: ");
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
    
    rep->node ()->addChild (sep);
}

void
VisTrackerClusterTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    if (! s_cluster.empty () && m_pDD.isValid ()) 
    {
	SoSeparator *sep = new SoSeparator;    
	SoSeparator *colorSep = new SoSeparator;    
	sep->addChild (colorSep);

	SoTranslation *trans = new SoTranslation;
	trans->translation = SbVec3f (0, 0, 0.05);
	sep->addChild (trans);

	SoSeparator *yellowSep = new SoSeparator;    
	sep->addChild (yellowSep);

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x6600FF00, rgbcomponents); // Electric indigo
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	colorSep->addChild (mat);
	
	// Electric lime
	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = m_clusterPointSize.value ();
	colorSep->addChild (drawStyle);

	SoMaterial *yellowMat = new SoMaterial;
	IgSbColorMap::unpack (0xCCFF0000, rgbcomponents); // Electric lime
	yellowMat->diffuseColor.setValue (SbColor (rgbcomponents));
	yellowSep->addChild (yellowMat);

	SoDrawStyle *yellowDrawStyle = new SoDrawStyle;
	if (m_clusterPointSize.value () > 2)
	    yellowDrawStyle->pointSize = m_clusterPointSize.value () - 2;
	yellowSep->addChild (yellowDrawStyle);

	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrtx = 0;
	    for (std::vector<std::pair<DetId, SiStripCluster> >::const_iterator 
		     i = s_cluster.begin (), iEnd = s_cluster.end (); i != iEnd; ++i)
	    {
		const DetId& id = (*i).first;

		SiStripCluster c = (*i).second;
		short firststrip = c.firstStrip (); 
		const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit*>( (m_pDD->idToDet (id)));
                // Draw only if in barrel.
  	        int subdet = theStripDet->geographicalId().subdetId();
  	        if (subdet == 1 || subdet == 3 || subdet == 5)
		{
		    const StripTopology* theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
		    GlobalPoint pos =  (m_pDD->idToDet (id))->surface().toGlobal(theStripTopol->localPosition(firststrip));

		    float x = pos.x () / 100.0;  // cm -> m
		    float y = pos.y () / 100.0;  // cm -> m
		    float z = 3.1;  // Z layer

		    vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
		}
	    }
	    
	    vertices->vertex.setNum (nVrtx);
	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrtx);

	    colorSep->addChild (points);
	    yellowSep->addChild (points);
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerClusterTwig: ");
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
VisTrackerClusterTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! s_cluster.empty () && m_pDD.isValid ()) 
    {
	SoSeparator *sep = new SoSeparator;    
	SoSeparator *colorSep = new SoSeparator;    
	sep->addChild (colorSep);

	SoTranslation *trans = new SoTranslation;
	trans->translation = SbVec3f (-0.05, 0, 0);
	sep->addChild (trans);

	SoSeparator *yellowSep = new SoSeparator;    
	sep->addChild (yellowSep);

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x6600FF00, rgbcomponents); // Electric indigo
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	colorSep->addChild (mat);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = m_clusterPointSize.value ();
	colorSep->addChild (drawStyle);
 
	SoMaterial *yellowMat = new SoMaterial;
	IgSbColorMap::unpack (0xCCFF0000, rgbcomponents); // Electric lime
	yellowMat->diffuseColor.setValue (SbColor (rgbcomponents));
	yellowSep->addChild (yellowMat);

	SoDrawStyle *yellowDrawStyle = new SoDrawStyle;
	if (m_clusterPointSize.value () > 2)
	    yellowDrawStyle->pointSize = m_clusterPointSize.value () - 2;
	yellowSep->addChild (yellowDrawStyle);
   
	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrx = 0;

	    for (std::vector<std::pair<DetId, SiStripCluster> >::const_iterator i = s_cluster.begin (), iEnd = s_cluster.end (); i != iEnd; ++i) 
	    {
		const DetId& id = (*i).first;

		SiStripCluster c = (*i).second;
		short firststrip = c.firstStrip (); 
		const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit*>( (m_pDD->idToDet (id)));
		const StripTopology* theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
		GlobalPoint pos = (m_pDD->idToDet (id))->surface ().toGlobal (theStripTopol->localPosition (firststrip));
                float x = -3.1;
                float y = sqrt (pos.x () * pos.x () + pos.y () * pos.y ()) / 100.0; 
		if (pos.y () < 0.0) y = - y;   
		float z = pos.z () / 100.0;  // cm -> m
		
		vertices->vertex.set1Value (nVrx++, SbVec3f (x, y, z));
	    }

            vertices->vertex.setNum (s_cluster.size());

	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
            points->numPoints.setValue (s_cluster.size());

	    colorSep->addChild (points);
	    yellowSep->addChild (points);
	}    
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerClusterTwig: ");
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

