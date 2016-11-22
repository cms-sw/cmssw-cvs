//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisTracker/interface/VisTrackerRechit2DTwig.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
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
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <qstring.h>
#include <iostream>
#include <iomanip>

#include <DataFormats/SiStripDetId/interface/StripSubdetector.h>
#include <DataFormats/SiPixelDetId/interface/PixelSubdetector.h>
#include <DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h>
#include <DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h>

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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Tracker Event/SiStrip RecHit 2D");
	
	if (! eventTwig)
	    eventTwig = parent;

	return new VisTrackerRechit2DTwig (state, eventTwig, "[N/A] SiStrip RecHit 2D (" + name + ")", 
					   friendlyName, modLabel, instanceName, processName);
    }
}

VisTrackerRechit2DTwig::VisTrackerRechit2DTwig (IgState *state, IgTwig *parent,
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
      m_scale (state, lat::CreateCallback (this, &VisTrackerRechit2DTwig::twigChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID rechit2DCollID (typeid (SiStripRecHit2DCollection));
    tfService->registerTwig (rechit2DCollID.friendlyClassName (), &createThisTwig);
}

void
VisTrackerRechit2DTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisTrackerRechit2DTwig::onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);
 
    rtemp.clear ();
   
    m_text = QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ())
	     .arg (event.luminosityBlock ())
	     .arg (event.orbitNumber ())
	     .arg (event.bunchCrossing ())
	     .latin1 ();

    std::vector< edm::Handle<SiStripRecHit2DCollection> > rechit2d_collections;
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, rechit2d_collections);
	}
	eventSetup.get<TrackerDigiGeometryRecord> ().get (m_pDD);
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTrackerRechit2DTwig: ");
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

    if (! rechit2d_collections.empty ())
    {
	std::vector< edm::Handle<SiStripRecHit2DCollection> >::iterator i;
	std::vector< edm::Handle<SiStripRecHit2DCollection> >::iterator iEnd;
	for (i = rechit2d_collections.begin (), iEnd = rechit2d_collections.end (); i != iEnd; ++i) 
	{
	    const SiStripRecHit2DCollection &c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    
	    this->name (nameStr.toStdString ());
	  
            for (SiStripRecHit2DCollection::DataContainer::const_iterator rphirechit = c.data().begin(); rphirechit != c.data().end(); ++rphirechit)
            {
		    DetId detectorId = (*rphirechit).geographicalId ();
		    unsigned int id = detectorId.rawId ();
		  
		    DetId detIdObject (id);

		    const SiStripCluster* Cluster = 0;
		    
		    if ((*rphirechit).cluster().isNonnull())
		      Cluster = (*rphirechit).cluster ().get ();
		    else if ((*rphirechit).cluster_regional().isNonnull())
		      Cluster = (*rphirechit).cluster_regional().get();
		    else continue;

		    std::vector<LocalPoint> points;
		    localPositions(&(*rphirechit), m_pDD.product(), points);
		    LocalPoint & position=points.front();
		 
		    int clusterCharge = 0;
		    int cas = Cluster->amplitudes ().size ();
		    for (int j = 0; j < cas; j++)
		    {
			clusterCharge += Cluster->amplitudes () [j];
		    }
		  
		    std::pair<std::pair<DetId, LocalPoint>, int> pxd;
		    pxd.first.first = detIdObject;
		    pxd.first.second = position;
		    pxd.second = clusterCharge;
		    rtemp.push_back (pxd);
	    }
	}      
    }
    
    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

LocalPoint VisTrackerRechit2DTwig::localPosition(const TrackingRecHit * rechit , const TrackingGeometry * geometry){

  if (rechit->geographicalId().det() == DetId::Tracker && rechit->geographicalId().subdetId() != PixelSubdetector::PixelBarrel && rechit->geographicalId().subdetId() != PixelSubdetector::PixelEndcap ){
    std::vector<LocalPoint> points;
    localPositions(rechit, geometry,points);
    if (points.size()==1)
      return points.front();
    else{
      //this is not really nice, but that's all we can do there.
      const SiStripMatchedRecHit2D * matched = dynamic_cast<const SiStripMatchedRecHit2D * >(rechit);
      if (matched){
	GlobalPoint pos_1 = geometry->idToDetUnit( matched->monoHit()->geographicalId() )->surface().toGlobal(points[0]);
	GlobalPoint pos_2 = geometry->idToDetUnit( matched->stereoHit()->geographicalId() )->surface().toGlobal(points[1]);
	GlobalPoint average((pos_1.x()+pos_2.x())/2,
			    (pos_1.y()+pos_2.y())/2,
			    (pos_1.z()+pos_2.z())/2);      
	return  geometry->idToDet(rechit->geographicalId())->surface().toLocal(average);
      }
      else return LocalPoint();
    }
  }else{
    return rechit->localPosition();
  }
}


void  VisTrackerRechit2DTwig::localPositions(const TrackingRecHit * rechit , const TrackingGeometry * geometry , std::vector<LocalPoint> & points){
  
  if (rechit->geographicalId().det() == DetId::Tracker && rechit->geographicalId().subdetId() != PixelSubdetector::PixelBarrel && rechit->geographicalId().subdetId() != PixelSubdetector::PixelEndcap ){
    
    const RecHit2DLocalPos * rechit2D = dynamic_cast<const RecHit2DLocalPos *>(rechit);
    const SiStripRecHit2D * single = dynamic_cast<const SiStripRecHit2D * >(rechit);
    //matched or single rechits
    if (single){
      DetId detectorId = rechit2D->geographicalId();
    
      const SiStripCluster* Cluster= 0 ;
      if (single->cluster().isNonnull())
	Cluster=single->cluster().get();
      else if (single->cluster_regional().isNonnull())
	Cluster=single->cluster_regional().get();
      else points.push_back(LocalPoint());
      const StripTopology  * topology = dynamic_cast<const StripTopology*>(&(geometry->idToDetUnit(detectorId)->topology()));
      ASSERT(topology);
			
      points.push_back(topology -> localPosition(Cluster->barycenter()));
    }
    else{
      const SiStripMatchedRecHit2D * matched = dynamic_cast<const SiStripMatchedRecHit2D*>(rechit);
      if (matched){
	localPositions(matched->monoHit(), geometry, points);
	localPositions(matched->stereoHit(), geometry, points);
      }
    }
  }   
  else{
    points.push_back(rechit->localPosition());
  }
}

void
VisTrackerRechit2DTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "Total " << rtemp.size () << " Silicon Strip rechits from";
    if(rtemp.size() > 100){
	text << " input file: printing only first 100 " << "<br>"; 
    }else text << " input file: " << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>GeomdetId</TH>"
	 << "<TH>Global Position (m)</TH>"
	 << "</TR>";

    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if ((! rtemp.empty ()) && m_pDD.isValid ()) 
    { 
	int nRecHits = 0;
	try 
	{
	    for (std::vector<std::pair<std::pair<DetId, LocalPoint>, int> >::const_iterator i = rtemp.begin (), iEnd = rtemp.end (); i != iEnd; ++i) 
	    {
		GlobalPoint pos =  (m_pDD->idToDet ((*i).first.first))->surface().toGlobal((*i).first.second);
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nRecHits++ << "</TD>"
		     << "<TD>" << std::setw (11) << ((*i).first.first).rawId()<< "</TD>"
		     << "<TD>" << pos.x () / 100 << ", " << pos.y () / 100 << ", " << pos.z () / 100 << "</TD>";
		if (nRecHits > 100) break;
	    }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerRechit2DTwig: ");
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
VisTrackerRechit2DTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
  
    if (! rtemp.empty ())
    {
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xDE316300, rgbcomponents); // Cerise
	mat->diffuseColor.setValue (SbColor (rgbcomponents));

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->lineWidth = 2.0;
	drawStyle->pointSize = 4.0;

	SoSeparator *sep = new SoSeparator; // global
	sep->addChild (mat);
	sep->addChild (drawStyle);

	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrtx = 0;

	    for (std::vector<std::pair<std::pair<DetId, LocalPoint>, int> >::const_iterator 
		     i = rtemp.begin (); i != rtemp.end (); ++i)
	    {
		GlobalPoint pos =  (m_pDD->idToDet ((*i).first.first))->surface().toGlobal((*i).first.second);
		float x = pos.x () / 100.0;  // cm -> m
		float y = pos.y () / 100.0;  // cm -> m
		float z = pos.z () / 100.0;  // cm -> m

		vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
	    }
	    
	    vertices->vertex.setNum (nVrtx);

	    SoMFInt32 markerIndex;
	    markerIndex.setValue (SoMarkerSet::CIRCLE_LINE_7_7);

	    SoMarkerSet *points = new SoMarkerSet;
	    points->markerIndex = markerIndex;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrtx);

	    sep->addChild (points);
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerRechit2DTwig: ");
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
VisTrackerRechit2DTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator; // global
    
    try 
    {
	for (std::vector<std::pair<std::pair<DetId, LocalPoint>, int> >::const_iterator i = rtemp.begin (); i != rtemp.end (); ++i) 
	{
	    GlobalPoint pos =  (m_pDD->idToDet ((*i).first.first))->surface().toGlobal((*i).first.second);

	    double eta = pos.eta ();
	    double phi = pos.phi ();
	    if (phi < 0.0) phi = 2 * M_PI + phi; // correction in absence of one convention
	    
	    int recCharge =(*i).second;
	 	    
	    IgSoTower *tower = new IgSoTower;
	    tower->position = SbVec2f (phi, eta); // eta, phi
	    tower->etaWidth = 0.03;
	    tower->phiWidth = 0.03;
	    tower->scaleFactor = m_scale.value ();
	    tower->energy = recCharge * 250.0 * 3.61e-09;
	    tower->orderedRGBA.set1Value (6, SbColor (0.9, 0.0, 0.1).getPackedValue ());
	    sep->addChild (tower);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTrackerRechit2DTwig: ");
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
VisTrackerRechit2DTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    if (! rtemp.empty ()) 
    {
	SoSeparator *sep = new SoSeparator;    
	SoSeparator *colorSep = new SoSeparator;    
	sep->addChild (colorSep);

	SoTranslation *trans = new SoTranslation;
	trans->translation = SbVec3f (0, 0, 0.05);
	sep->addChild (trans);
	
	SoSeparator *whiteSep = new SoSeparator;    
	sep->addChild (whiteSep);
	
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xDE316300, rgbcomponents); // Cerise
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	colorSep->addChild (mat);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 6.0;
	colorSep->addChild (drawStyle);
    
	SoMaterial *whiteMat = new SoMaterial;
	whiteMat->diffuseColor.setValue (1.0, 1.0, 1.0);
	whiteSep->addChild (whiteMat);

	SoDrawStyle *whiteDrawStyle = new SoDrawStyle;
	whiteDrawStyle->pointSize = 2.0;
	whiteSep->addChild (whiteDrawStyle);

	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrtx = 0;
	    for (std::vector<std::pair<std::pair<DetId, LocalPoint>, int> >::const_iterator i = rtemp.begin (), iEnd = rtemp.end (); i != iEnd; ++i)
	    {
		const DetId& id = (*i).first.first;
		const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit *>(m_pDD->idToDet (id));
		// Draw only if in barrel.
  	        int subdet = theStripDet->geographicalId ().subdetId ();
  	        if (subdet == 1 || subdet == 3 || subdet == 5)
		{
		    GlobalPoint pos =  (m_pDD->idToDet ((*i).first.first))->surface().toGlobal((*i).first.second);
		    float x = pos.x () / 100.0;  // cm -> m
		    float y = pos.y () / 100.0;  // cm -> m
		    float z = 3; // pos.z () / 100.0;  // cm -> m

		    vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
		}
	    }
	    
	    vertices->vertex.setNum (nVrtx);
	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrtx);

	    colorSep->addChild (points);
	    whiteSep->addChild (points);
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerRechit2DTwig: ");
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
VisTrackerRechit2DTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! rtemp.empty ()) 
    {
	SoSeparator *sep = new SoSeparator;    
	SoSeparator *colorSep = new SoSeparator;    
	sep->addChild (colorSep);

	SoTranslation *trans = new SoTranslation;
	trans->translation = SbVec3f (-0.05, 0, 0);
	sep->addChild (trans);
	
	SoSeparator *whiteSep = new SoSeparator;    
	sep->addChild (whiteSep);
	
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xDE316300, rgbcomponents); // Cerise
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	colorSep->addChild (mat);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 6.0;
	colorSep->addChild (drawStyle);
    
	SoMaterial *whiteMat = new SoMaterial;
	whiteMat->diffuseColor.setValue (1.0, 1.0, 1.0);
	whiteSep->addChild (whiteMat);

	SoDrawStyle *whiteDrawStyle = new SoDrawStyle;
	whiteDrawStyle->pointSize = 2.0;
	whiteSep->addChild (whiteDrawStyle);

	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrx = 0;

	    for (std::vector<std::pair<std::pair<DetId, LocalPoint>,int> >::const_iterator i = rtemp.begin (); i != rtemp.end (); ++i) 
	    {
		GlobalPoint pos =  (m_pDD->idToDet ((*i).first.first))->surface().toGlobal((*i).first.second);
                float x = -3; // the layer in RZ
                float y = sqrt (pos.x () * pos.x () + pos.y () * pos.y ()) / 100.0; 
		if (pos.y () < 0.) y = -y;
		float z = pos.z () / 100.0;  // cm -> m
		
		vertices->vertex.set1Value (nVrx++, SbVec3f (x, y, z));
	    }

	    vertices->vertex.setNum (rtemp.size());

	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (rtemp.size());
	    colorSep->addChild (points);
	    whiteSep->addChild (points);
	}    
	catch (cms::Exception& e)
	{
	    e.append (" from VisTrackerRechit2DTwig: ");
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

