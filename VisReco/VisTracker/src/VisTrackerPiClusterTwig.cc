#include "VisReco/VisTracker/interface/VisTrackerPiClusterTwig.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
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
#include <qstring.h>
#include <iostream>
#include <sstream>
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
    eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Tracker Event/Pixel Clusters");
	
    if (! eventTwig) 
      eventTwig = parent;
    return new VisTrackerPiClusterTwig (state, eventTwig, "[N/A] Pixel Cluster (" + name + ")", 
					friendlyName, modLabel, instanceName, processName);
  }
}

VisTrackerPiClusterTwig::VisTrackerPiClusterTwig(IgState *state, IgTwig *parent,
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
  edm::TypeID pclusterCollID (typeid (edmNew::DetSetVector<SiPixelCluster>));
  tfService->registerTwig (pclusterCollID.friendlyClassName (), &createThisTwig);
  m_pset = VisConfigurationService::pSet ();
  if (! m_pset)
  {
    edm::ParameterSet p;
    VisConfigurationService *visService = new VisConfigurationService(p);
    m_pset = visService->pSet();
    delete visService;
  }
  m_clusterPointSize = m_pset->getUntrackedParameter<double> ("clusterPointSize", 4.0);
}

void
VisTrackerPiClusterTwig::onNewEvent (const edm::Event &event,
				     const edm::EventSetup &eventSetup)
{
  // Get debugging dump.
  VisQueuedTwig::onNewEvent (event, eventSetup);

  p_cluster.clear ();
  
  m_text = (QString ("Run # %1, event # %2")
	    .arg (event.id ().run ())
	    .arg (event.id ().event ()).latin1 ());
  
  std::vector< edm::Handle<edmNew::DetSetVector<SiPixelCluster> > > p_cluster_collections;
  try
  {
    if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
    {
      VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
      event.getMany (visSel, p_cluster_collections);
    }
  }
  catch (cms::Exception& e)
  {
    e.append (" from VisTrackerPiClusterTwig: ");
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
  
  if (! p_cluster_collections.empty ())
  {
    std::vector< edm::Handle<edmNew::DetSetVector<SiPixelCluster> > >::iterator i;
    std::vector< edm::Handle<edmNew::DetSetVector<SiPixelCluster> > >::iterator iEnd;
    for (i = p_cluster_collections.begin (), iEnd = p_cluster_collections.end (); i != iEnd; ++i) 
    {
      const edmNew::DetSetVector<SiPixelCluster> &c = *(*i);
      QString sizeStr = (QString ("%1").arg (c.size ()));
      QString nameStr = QString::fromStdString (this->name ());
      int ib = nameStr.find ("[");
      int ie = nameStr.find ("]");
      nameStr.replace (ib + 1, ie - 1, sizeStr);
      this->name (nameStr.toStdString ());
	  
      edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter=c.begin();
	  
      for (; DSViter!=c.end(); ++DSViter)
      {
	edmNew::DetSet<SiPixelCluster> ds = *DSViter;
	const uint32_t detID = DSViter->detId();
	DetId detid (detID);
	edmNew::DetSet<SiPixelCluster>::const_iterator icluster = DSViter->begin();
	edmNew::DetSet<SiPixelCluster>::const_iterator iclusterEnd = DSViter->end();
	for(; icluster != iclusterEnd; ++icluster)
	{ 
	  std::pair<DetId, SiPixelCluster> p;
	  p.first = detid;
	  p.second = (*icluster);
	  p_cluster.push_back (p);
	}
      }
    }
  }

  IgRepSet::invalidate (this->parent (), IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);   
}

void
VisTrackerPiClusterTwig::update (IgTextRep *rep)
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
    text << "Total " << p_cluster.size () << " Pixel clusters from";
    if (p_cluster.size () > 100)
    {
      text << " input file: printing only first 100 " << "<br>";
    }
    else text << " input file: " << m_text << "<br>";
  
  
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>GeomdetId</TH>"
	 << "<TH>Position</TH>"
	 << "<TH>Column</TH>"
	 << "<TH>Row</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
  
    if (! p_cluster.empty ())
    {      
      VisEventSetupService *esService = VisEventSetupService::get (state ());
      ASSERT (esService);

      int nPClusters = 0;
      try 
      {
	for (std::vector<std::pair<DetId, SiPixelCluster> >::const_iterator i = p_cluster.begin (), iEnd = p_cluster.end (); i != iEnd; ++i) 
	{
	  const DetId& id = (*i).first;
	           
	  const PixelGeomDetUnit* theDet = dynamic_cast<const PixelGeomDetUnit*>(esService->idToDet (id));
	  const RectangularPixelTopology *theTopol = dynamic_cast<const RectangularPixelTopology *>( &(theDet->specificTopology ()));
	     
	  int row = (*i).second.minPixelRow();
	  int column =(*i).second.minPixelCol();
	      
	  GlobalPoint pos =  (esService->idToDet (id))->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,column)));
	      
	  text << "<TR align = right>"
	       << "<TD>" << std::setw (3) << nPClusters++ << "</TD>"
	       << "<TD>" << std::setw (11) << ((*i).first).rawId()<< "</TD>"
	       << "<TD>" << pos.x () << ", " << pos.y () << ", " << pos.z () << "</TD>"
	       << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.minPixelCol() << "</TD>"
	       << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.minPixelRow() << "</TD></TR>";

	  if (nPClusters>100) break;
	}
      }
      catch (cms::Exception& e)
      {
	e.append (" from VisTrackerPiClusterTwig: ");
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
VisTrackerPiClusterTwig::update (Ig3DRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  rep->clear ();
  
  if (! p_cluster.empty ())
  {
    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (0.0, 0.0, 1.0);//blue
    mat->emissiveColor.setValue(0.0, 0.0, 1.0);
      
    SoDrawStyle *drawStyle = new SoDrawStyle;
    drawStyle->pointSize = m_clusterPointSize;
      
    SoSeparator *sep = new SoSeparator; // global
    sep->addChild (mat);
    sep->addChild (drawStyle);
      
    try 
    {
      SoVertexProperty *vertices = new SoVertexProperty;
      int nVrtx = 0;
	  
      for (std::vector<std::pair<DetId, SiPixelCluster> >::const_iterator 
	     i = p_cluster.begin (); i != p_cluster.end (); ++i)
      {
	const DetId& id = (*i).first;	      
	SiPixelCluster c = (*i).second;
	const PixelGeomDetUnit* theDet = dynamic_cast<const PixelGeomDetUnit*>(esService->idToDet (id));
	const RectangularPixelTopology *theTopol = dynamic_cast<const RectangularPixelTopology *>( &(theDet->specificTopology ()));
	int row = (*i).second.minPixelRow();
	int column =(*i).second.minPixelCol();
 
	GlobalPoint pos =  (esService->idToDet (id))->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,column)));
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
      e.append (" from VisTrackerPiClusterTwig: ");
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
VisTrackerPiClusterTwig::update (IgLegoRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  rep->clear ();
  
  if (! p_cluster.empty ()) 
  {
    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);
    SoSeparator *sep = new SoSeparator; // global
  
    try 
    {
      for (std::vector<std::pair<DetId, SiPixelCluster> >::const_iterator i = p_cluster.begin (); i != p_cluster.end (); ++i) 
      {
	const DetId& id = (*i).first;
	const PixelGeomDetUnit* theDet = dynamic_cast<const PixelGeomDetUnit*>(esService->idToDet (id));
	const RectangularPixelTopology *theTopol = dynamic_cast<const RectangularPixelTopology *>( &(theDet->specificTopology ()));
	  
	int row = (*i).second.minPixelRow();
	int column =(*i).second.minPixelCol();
	GlobalPoint pos =  (esService->idToDet (id))->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,column)));
	double eta = pos.eta ();
	double phi = pos.phi ();
	if (phi < 0.0) phi = 2 * M_PI + phi; // correction in absence of one convention
	  
	IgSoTower *tower = new IgSoTower;
	tower->position = SbVec2f (phi, eta); // eta, phi
	tower->scaleFactor = 20.0;
	sep->addChild (tower);
      }
    }
    catch (cms::Exception& e)
    {
      e.append (" from VisTrackerPiClusterTwig: ");
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
VisTrackerPiClusterTwig::update (IgRPhiRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  rep->clear ();
  
  if (! p_cluster.empty ())
  {
    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    SoSeparator *sep = new SoSeparator;    
      
    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (0.0, 0.0, 1.0);
    sep->addChild (mat);
      
    SoDrawStyle *drawStyle = new SoDrawStyle;
    drawStyle->pointSize = m_clusterPointSize;
    sep->addChild (drawStyle);

    try 
    {
      SoVertexProperty *vertices = new SoVertexProperty;
      int nVrtx = 0;
      for (std::vector<std::pair<DetId, SiPixelCluster> >::const_iterator 
	     i = p_cluster.begin (); i != p_cluster.end (); ++i)
      {
	const DetId& id = (*i).first;
	const PixelGeomDetUnit* theDet = dynamic_cast<const PixelGeomDetUnit*>(esService->idToDet (id));
	//              Draw only if in barrel.
	int subdet = theDet->geographicalId().subdetId();
	if(subdet==1 || subdet == 3 || subdet == 5)
	{
	  const RectangularPixelTopology *theTopol = dynamic_cast<const RectangularPixelTopology *>( &(theDet->specificTopology ()));
	  int row = (*i).second.minPixelRow();
	  int column =(*i).second.minPixelCol();
	      
	  GlobalPoint pos =  (esService->idToDet (id))->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,column)));
	  float x = pos.x () / 100.0;  // cm -> m
	  float y = pos.y () / 100.0;  // cm -> m
	  float z = 10.0;  // cm -> m
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
      e.append (" from VisTrackerPiClusterTwig: ");
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
VisTrackerPiClusterTwig::update (IgRZRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  rep->clear ();

  if (! p_cluster.empty ())
  {
    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    SoSeparator *sep = new SoSeparator;    
    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (0.0, 0.0, 1.0);
    sep->addChild (mat);

    SoDrawStyle *drawStyle = new SoDrawStyle;
    drawStyle->pointSize = m_clusterPointSize;
    sep->addChild (drawStyle);
    
    try 
    {
      SoVertexProperty *vertices = new SoVertexProperty;
      int nVrx = 0;

      for (std::vector<std::pair<DetId, SiPixelCluster> >::const_iterator i = p_cluster.begin (); i != p_cluster.end (); ++i) 
      {
	const DetId& id = (*i).first;
	const PixelGeomDetUnit* theDet = dynamic_cast<const PixelGeomDetUnit*>(esService->idToDet (id));
	const RectangularPixelTopology *theTopol = dynamic_cast<const RectangularPixelTopology *>( &(theDet->specificTopology ()));
	int row = (*i).second.minPixelRow();
	int column =(*i).second.minPixelCol();
	GlobalPoint pos =  (esService->idToDet (id))->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,column)));
	float x = 0;
	float y = sqrt(pos.x ()*pos.x()+pos.y()*pos.y()) / 100.0; if(pos.y()< 0. )y = - y;

	float z = pos.z () / 100.0;  // cm -> m
	      
	vertices->vertex.set1Value (nVrx++, SbVec3f (x, y, z));
      }

      vertices->vertex.setNum (nVrx);

      SoPointSet *points = new SoPointSet;
      points->vertexProperty.setValue (vertices);
      points->numPoints.setValue (nVrx);
      sep->addChild (points);
    }    
    catch (cms::Exception& e)
    {
      e.append (" from VisTrackerPiClusterTwig: ");
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

  
