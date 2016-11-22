#include "VisReco/VisTracker/interface/VisTrackerPiRechitTwig.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
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
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <QString>
#include <iostream>
#include <iomanip>

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
    eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Tracker Event/Pixel RecHits");
	
    if (! eventTwig) 
      eventTwig = parent;
    return new VisTrackerPiRechitTwig (state, eventTwig, "[N/A] Pixel RecHit (" + name + ")", friendlyName, modLabel, instanceName, processName);
  }
}

VisTrackerPiRechitTwig::VisTrackerPiRechitTwig (IgState *state, IgTwig *parent,
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
  edm::TypeID pixelrechitCollID (typeid (SiPixelRecHitCollection));
  tfService->registerTwig (pixelrechitCollID.friendlyClassName (), &createThisTwig);
}

void
VisTrackerPiRechitTwig::onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup)
{
  // Get debugging dump.
  VisQueuedTwig::onNewEvent (event, eventSetup);
 
  rtemp.clear();
  
  m_text = (QString ("Run # %1, event # %2")
	    .arg (event.id ().run ())
	    .arg (event.id ().event ()).latin1 ());

  std::vector< edm::Handle<SiPixelRecHitCollection> > pixelrechit_collections;
  try
  {
    if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
    {
      VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
      event.getMany (visSel,  pixelrechit_collections);
    }
  }
  catch (cms::Exception& e)
  {
    e.append (" from VisTrackerPiRechitTwig: ");
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

  if (! pixelrechit_collections.empty ())
  {
    std::vector< edm::Handle<SiPixelRecHitCollection> >::iterator i;
    for (i = pixelrechit_collections.begin (); i != pixelrechit_collections.end (); i++) 
    {
      const SiPixelRecHitCollection&c = *(*i);
      QString sizeStr = (QString ("%1").arg (c.size ()));
      QString nameStr = QString::fromStdString (this->name ());
      int ib = nameStr.find ("[");
      int ie = nameStr.find ("]");
      nameStr.replace (ib + 1, ie - 1, sizeStr);
      this->name (nameStr.toStdString ());
	  
      for (SiPixelRecHitCollection::DataContainer::const_iterator pixel_rechit = c.data().begin(), pEnd = c.data().end();
	   pixel_rechit!= pEnd; ++pixel_rechit)
      {
	DetId detIdObject = (*pixel_rechit).geographicalId();
             
	LocalPoint position;
	if((*pixel_rechit).hasPositionAndError())
	  position=(*pixel_rechit).localPosition(); else break;
              
	// const SiPixelCluster* Cluster=(*pixel_rechit).cluster().get();
              
	int clusterCharge = 0;
	/*int cas = Cluster->amplitudes().size();
	  for (int j=0; j<cas; j++)
	  {
	  clusterCharge +=Cluster->amplitudes()[j];
	  }
	*/
             
	std::pair<std::pair<DetId, LocalPoint>, int> pxd;
	pxd.first.first = detIdObject;
	//pxd.second = position;
	pxd.first.second = position;
	pxd.second =clusterCharge;
	rtemp.push_back(pxd);
      }
    }
  } 
  IgRepSet::invalidate (this->parent (), IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);   
}

void
VisTrackerPiRechitTwig::update (IgTextRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);

  // Prepare property description.
  std::ostringstream  text;
  if (m_friendlyName.empty () && m_moduleLabel.empty () && m_instanceName.empty () && m_processName.empty ())
  {
    text << "Please, select the branches of this Twig for more information.";	
  }
  else
  {
    text << "Total " << rtemp.size () << " Silicon Pixel rechits from";
    if(rtemp.size() > 100){
      text << " input file: printing only first 100 " << "<br>"; 
    }else text << " input file: " << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>GeomdetId</TH>"
	 << "<TH>Position</TH>"
	 << "</TR>";

    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if (! rtemp.empty ())
    {
      VisEventSetupService *esService = VisEventSetupService::get (state ());
      ASSERT (esService);
    
      int nRecHits = 0;
      try 
      {
	for (std::vector<std::pair<std::pair<DetId, LocalPoint>, int> >::const_iterator i = rtemp.begin (), iEnd = rtemp.end();
	     i != iEnd; ++i) 
	{
	  GlobalPoint pos =  (esService->idToDet ((*i).first.first))->surface().toGlobal((*i).first.second);
	  text << "<TR align = right>"
	       << "<TD>" << std::setw (3) << nRecHits++ << "</TD>"
	       << "<TD>" << std::setw (11) << ((*i).first.first).rawId()<< "</TD>"
	       << "<TD>" << pos.x()/100 << ", " << pos.y()/100 << ", " << pos.z()/100 << "</TD>";
	  if (nRecHits>100) break;
	}
      }
      catch (cms::Exception& e)
      {
	e.append (" from VisTrackerPiRechitTwig: ");
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
  }
  
  // Send it over.
  rep->setText (text.str ());
}


void
VisTrackerPiRechitTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
  
    if (!rtemp.empty())
    {
      VisEventSetupService *esService = VisEventSetupService::get (state ());
      ASSERT (esService);
	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (1.0, 0.0, 0.0);//red
	mat->emissiveColor.setValue(1.0, 0.0, 0.0);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 4.0;

	SoSeparator *sep = new SoSeparator; // global
	sep->addChild (mat);
	sep->addChild (drawStyle);

	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrtx = 0;

	    for (std::vector<std::pair<std::pair<DetId, LocalPoint>, int> >::const_iterator 
		     i = rtemp.begin (), iEnd = rtemp.end (); i != iEnd; ++i)
	    {
		GlobalPoint pos =  (esService->idToDet ((*i).first.first))->surface().toGlobal((*i).first.second);
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
	    e.append (" from VisTrackerPiRechitTwig: ");
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
VisTrackerPiRechitTwig::update (IgLegoRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  rep->clear ();
    
  if (! rtemp.empty ())
  {      
    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    SoSeparator *sep = new SoSeparator; // global
    
    try 
    {
      for (std::vector<std::pair<std::pair<DetId, LocalPoint>, int> >::const_iterator i = rtemp.begin (), iEnd = rtemp.end ();
	   i != iEnd; ++i) 
      {
	GlobalPoint pos =  (esService->idToDet ((*i).first.first))->surface().toGlobal((*i).first.second);

	double eta = pos.eta ();
	double phi = pos.phi ();
	if (phi < 0.0) phi = 2 * M_PI + phi; // correction in absence of one convention
	    
	int recCharge =(*i).second;
	 	    
	IgSoTower *tower = new IgSoTower;
	tower->position = SbVec2f (phi, eta); // eta, phi
	tower->scaleFactor = 20.0;
	tower->energy = recCharge;
	sep->addChild (tower);
      }
    }
    catch (cms::Exception& e)
    {
      e.append (" from VisTrackerPiRechitTwig: ");
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
VisTrackerPiRechitTwig::update (IgRPhiRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  rep->clear ();
    
  if (! rtemp.empty ()) 
  {
    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    SoSeparator *sep = new SoSeparator;    

    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (1.0, 0.0, 0.0);
    sep->addChild (mat);

    SoDrawStyle *drawStyle = new SoDrawStyle;
    drawStyle->pointSize = 4.0;
    sep->addChild (drawStyle);

    try 
    {
      SoVertexProperty *vertices = new SoVertexProperty;
      int nVrtx = 0;
      for (std::vector<std::pair<std::pair<DetId, LocalPoint>, int> >::const_iterator i = rtemp.begin (), iEnd = rtemp.end ();
	   i != iEnd; ++i)
      {
	GlobalPoint pos = (esService->idToDet ((*i).first.first))->surface().toGlobal((*i).first.second);
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
      e.append (" from VisTrackerPiRechitTwig: ");
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
VisTrackerPiRechitTwig::update (IgRZRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  rep->clear ();

  if (! rtemp.empty ()) 
  {
    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    SoSeparator *sep = new SoSeparator;    
    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (1.0, 0.0, 0.0);
    sep->addChild (mat);

    SoDrawStyle *drawStyle = new SoDrawStyle;
    drawStyle->pointSize = 3.0;
    sep->addChild (drawStyle);
    
    try 
    {
      SoVertexProperty *vertices = new SoVertexProperty;
      int nVrx = 0;

      for (std::vector<std::pair<std::pair<DetId, LocalPoint>,int> >::const_iterator i = rtemp.begin (), iEnd = rtemp.end ();
	   i != iEnd; ++i) 
      {
	GlobalPoint pos =  (esService->idToDet ((*i).first.first))->surface().toGlobal((*i).first.second);
	float x = 0.0;
	float y = sqrt(pos.x ()*pos.x()+pos.y()*pos.y()) / 100.0; if(pos.y()< 0. )y = - y;
	float z = pos.z () / 100.0;  // cm -> m
		
	vertices->vertex.set1Value (nVrx++, SbVec3f (x, y, z));
      }

      vertices->vertex.setNum (rtemp.size());

      SoPointSet *points = new SoPointSet;
      points->vertexProperty.setValue (vertices);
      points->numPoints.setValue (rtemp.size());
      sep->addChild (points);
    }    
    catch (cms::Exception& e)
    {
      e.append (" from VisTrackerPiRechitTwig: ");
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

