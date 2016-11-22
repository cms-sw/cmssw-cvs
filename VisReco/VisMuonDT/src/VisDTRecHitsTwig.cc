#include "VisReco/VisMuonDT/interface/VisDTRecHitsTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/DTRecHit/interface/DTRecHit1D.h"
#include "DataFormats/DTRecHit/interface/DTRecHit1DPair.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"

#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoLineSet.h>
#include <QString>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace edm;
using namespace std;
using namespace DTEnums;

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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Muon DT Event/DT RecHits");

	return new VisDTRecHitsTwig (state, eventTwig, "[N/A] DT RecHit (" + name + ")", 
				     friendlyName, modLabel, instanceName, processName);
    }

  /** Get rotation of a DetUnit */ 
  void
  createRotation (const GeomDetUnit *det, SbVec3f & axis, float & angle)
  {    
    // Extract the coeff of the rotation matrix
    // with a projection on the basis vectors
    TkRotation< float > rot = det->surface ().rotation ();
    Basic3DVector< float > nx (1., 0., 0.); // x basis vector
    Basic3DVector< float > ny (0., 1., 0.); // y basis vector
    Basic3DVector< float > nz (0., 0., 1.); // z basis vector
    Basic3DVector< float > px = rot * nx; // Rxx Ryx Rzx
    Basic3DVector< float > py = rot * ny; // Rxy Ryy Rzy
    Basic3DVector< float > pz = rot * nz; // Rxz Ryz Rzz
    /* the rotation matrix is
     *	Rxx Rxy Rxz
     *	Ryx Ryy Ryz
     *	Rzx Rzy Rzz
     */
    // SbMatrix in row major order.
    // The 4th dimension is set empiricly set to
    // the identity
    SbMatrix sbm (px.x (), py.x (), pz.x (), 0,
		  px.y (), py.y (), pz.y (), 0,
		  px.z (), py.z (), pz.z (), 0,
		  0, 0, 0, 1);
    SbRotation sbr (sbm);

    float x;
    float y;
    float z;

    sbr.getValue (axis, angle);
    axis.getValue (x, y, z);
  }
}

VisDTRecHitsTwig::VisDTRecHitsTwig (IgState *state, IgTwig *parent,
				    const std::string &name /* = "" */,
				    const std::string &friendlyName /* = "" */,
				    const std::string &moduleLabel /* = "" */,
				    const std::string &instanceName /* = "" */,
				    const std::string &processName /* = "" */)
    : VisQueuedTwig (state, parent, name),
      m_text ("no info"),
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
    edm::TypeID recHitsID (typeid (DTRecHitCollection));
    tfService->registerTwig (recHitsID.friendlyClassName (), &createThisTwig);
}

void
VisDTRecHitsTwig::onNewEvent (const edm::Event &event,
			      const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_rechit.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    // Get the rechit collection from the event
    std::vector<edm::Handle<DTRecHitCollection> > rechitCollections;

    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, rechitCollections);
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

    try
    {
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

    if (! rechitCollections.empty ()) 
    {	
	std::vector<Handle<DTRecHitCollection> >::iterator i;
	std::vector<Handle<DTRecHitCollection> >::iterator iEnd;
	for (i = rechitCollections.begin (), iEnd = rechitCollections.end (); i != iEnd; ++i) 
	{
	    const DTRecHitCollection& c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    this->name (nameStr.toStdString ());
	    for (DTRecHitCollection::const_iterator rhi = c.begin (), rhiEnd = c.end ();
		 rhi != rhiEnd; ++rhi) 
	    {
		m_rechit.push_back (*rhi);
	    }
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisDTRecHitsTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    // Prepare property description.
    std::ostringstream  text;

    text << "<b>Muon Barrel: DT Track RecHit pairs (with L/R ambiguity)</b><br>";
    
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Wheel</TH>"
	 << "<TH>Station</TH>"
	 << "<TH>Sector</TH>"
	 << "<TH>SL</TH>"
	 << "<TH>Layer</TH>"
	 << "<TH>Cell</TH>"
	 << "<TH>x [mm]</TH>"
	 << "<TH>Raw digi time [ns]</TH>"
	 << "</TR>";
    text << setiosflags (ios::showpoint | ios::fixed);
    text.setf (ios::right, ios::adjustfield);

    try {
    
	int nRecHits = 0;
	for (std::vector< DTRecHit1DPair >::const_iterator i = m_rechit.begin (); i != m_rechit.end (); ++i) 
	{
	    const DTRecHit1D* lrechit = (*i).componentRecHit(Left);
	    const DTRecHit1D* rrechit = (*i).componentRecHit(Right);

	    int wireIdRh = (*i).wireId().wire();
	    int layerIdRh = (*i).wireId().layerId().layer();
	    int slayerIdRh = (*i).wireId().layerId().superlayerId().superLayer();
	    int sectorIdRh = (*i).wireId().layerId().superlayerId().chamberId().sector();
	    int stationIdRh = (*i).wireId().layerId().superlayerId().chamberId().station();
	    int wheelIdRh = (*i).wireId().layerId().superlayerId().chamberId().wheel();
	
	    float dx = (lrechit->localPosition()-rrechit->localPosition()).mag()/2.;
	    float digitime = (*i).digiTime();
	
	    text << "<TR align = right>"
		 << "<TD>" << std::setw (3) << nRecHits++ << "</TD>"
		 << "<TD>" << wheelIdRh << "</TD>"
		 << "<TD>" << stationIdRh << "</TD>"
		 << "<TD>" << sectorIdRh << "</TD>"
		 << "<TD>" << slayerIdRh << "</TD>"
		 << "<TD>" << layerIdRh << "</TD>"
		 << "<TD>" << wireIdRh << "</TD>"
		 << "<TD>" << setw (6) << setprecision (2) << dx*10. << "</TD>"
		 << "<TD>" << setw (6) << setprecision (2) << digitime << "</TD>"
		 << "</TR>";
	}
    }
  
    catch (...) {
	text << "No information available: an exception is caught.";
    }
    {
	rep->setText (text.str ());
    
    }
}

void
VisDTRecHitsTwig::update (Ig3DRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  
  rep->clear ();
  
  if ((! m_rechit.empty ()) &&  m_pDD.isValid ())
    {
      SoSeparator *sep = new SoSeparator;
      
      SoDrawStyle *drawStyle = new SoDrawStyle;
      drawStyle->pointSize.setValue (1.0);
      sep->addChild(drawStyle);
      try
	{
	  SoVertexProperty *vtx = new SoVertexProperty;
	  vtx->materialBinding = SoVertexProperty::OVERALL;
	  vtx->orderedRGBA = 0x0000FFFF; // yellow;
	  
	  // 	    SoLineSet *line = new SoLineSet;
	  // 	    line->numVertices = 2;
	  
	  SoDrawStyle *wdrawStyle = new SoDrawStyle;
	  wdrawStyle->style = SoDrawStyle::LINES;
	  wdrawStyle->lineWidth.setValue (1.0);
	  //	    wsep->addChild (wdrawStyle);
	  sep->addChild (wdrawStyle);
	  
	  int nVtx = 0;
	  
	  for (std::vector< DTRecHit1DPair >::const_iterator i = m_rechit.begin (); i != m_rechit.end (); ++i) 
	    {
	      //	  const DTRecHit1D* lrechit = (*i).componentRecHit(Left);
	      
	      // 		SoLineSet *line = new SoLineSet;
	      // 		line->numVertices = 2;
	      // 		SoVertexProperty *vtx = new SoVertexProperty;
	      
	      
	      const DTRecHit1D* lrechit = (*i).componentRecHit(Left);
	      const DTRecHit1D* rrechit = (*i).componentRecHit(Right);
	      
	      DTWireId wireId = i->wireId();
	      const DTLayer* layer = m_pDD->layer(wireId);
	      
	      const DTTopology & topo = layer->specificTopology();
	      
	      LocalPoint lLocalPos = lrechit->localPosition();
	      LocalPoint rLocalPos = rrechit->localPosition();
	      
	      LocalPoint yPlusLeft(lLocalPos.x(),
				   topo.cellLenght()/2.,
				   0.);
	      LocalPoint yMinusLeft(lLocalPos.x(),
				    -topo.cellLenght()/2.,
				    0.);
	      LocalPoint yPlusRight(rLocalPos.x(),
				    topo.cellLenght()/2.,
				    0.);
	      LocalPoint yMinusRight(rLocalPos.x(),
				     -topo.cellLenght()/2.,
				     0.);
	      
	      GlobalPoint lPlusGlobalPos = layer->toGlobal(yPlusLeft);
	      GlobalPoint lMinusGlobalPos = layer->toGlobal(yMinusLeft);
	      GlobalPoint rPlusGlobalPos = layer->toGlobal(yPlusRight);
	      GlobalPoint rMinusGlobalPos = layer->toGlobal(yMinusRight);
	      
	      GlobalPoint lGlobalPos = layer->toGlobal(lLocalPos);
	      GlobalPoint rGlobalPos = layer->toGlobal(rLocalPos);
	      
	      SoLineSet *linel = new SoLineSet;
	      linel->numVertices = 2;
	      SoVertexProperty *vtxl = new SoVertexProperty;
	      
	      
	      vtxl->vertex.set1Value(0,SbVec3f(lPlusGlobalPos.x()/100.,
					       lPlusGlobalPos.y()/100.,
					       lPlusGlobalPos.z()/100.)); //cm->m
	      vtxl->vertex.set1Value(1,SbVec3f(lMinusGlobalPos.x()/100.,
					       lMinusGlobalPos.y()/100.,
					       lMinusGlobalPos.z()/100.)); //cm->m
	      linel->vertexProperty = vtxl;
	      
	      SoLineSet *liner = new SoLineSet;
	      liner->numVertices = 2;
	      SoVertexProperty *vtxr = new SoVertexProperty;
	      
	      
	      vtxr->vertex.set1Value(0,SbVec3f(rPlusGlobalPos.x()/100.,
					       rPlusGlobalPos.y()/100.,
					       rPlusGlobalPos.z()/100.)); //cm->m
	      vtxr->vertex.set1Value(1,SbVec3f(rMinusGlobalPos.x()/100.,
					       rMinusGlobalPos.y()/100.,
					       rMinusGlobalPos.z()/100.)); //cm->m
	      liner->vertexProperty = vtxr;
	      
	      sep->addChild (linel);
	      sep->addChild (liner);
	      
	      
	      vtx->vertex.set1Value(nVtx,SbVec3f(lGlobalPos.x()/100.,
						 lGlobalPos.y()/100.,
						 lGlobalPos.z()/100.)); //cm->m
	      nVtx++;
	      vtx->vertex.set1Value(nVtx,SbVec3f(rGlobalPos.x()/100.,
						 rGlobalPos.y()/100.,
						 rGlobalPos.z()/100.)); //cm->m
	      nVtx++;
	      
	      SoSeparator* separator = new SoSeparator;		
	      addWire (separator, *i);
	      sep->addChild (separator);
	      
	    }
	  if (nVtx>0) {
	    vtx->vertex.setNum (nVtx);
	    SoPointSet* pset = new SoPointSet;
	    pset->vertexProperty = vtx;
	    sep->addChild(pset);
	    
	    // 	      SoSeparator* separator = new SoSeparator;		
	    // 	      addWire (separator, *i);
	    // 	      sep->addChild (separator);
	  }
	}
      catch (...) 
	{
	  std::cout << "No DTRecHit RecHits." << std::endl;
	}
      
      rep->node ()->addChild (sep);
    }
}

void
VisDTRecHitsTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    
    //........ still missing
}

void
VisDTRecHitsTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if ((! m_rechit.empty ()) &&  m_pDD.isValid ())
      {
	SoSeparator *sep = new SoSeparator;

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize.setValue (1.0);
	sep->addChild(drawStyle);
	try
	  {
	    SoVertexProperty *vtx = new SoVertexProperty;
	    vtx->materialBinding = SoVertexProperty::OVERALL;
	    vtx->orderedRGBA = 0x0000FFFF; // yellow;

// 	    SoLineSet *line = new SoLineSet;
// 	    line->numVertices = 2;
	  
	    SoDrawStyle *wdrawStyle = new SoDrawStyle;
	    wdrawStyle->style = SoDrawStyle::LINES;
	    wdrawStyle->lineWidth.setValue (1.0);
	    //	    wsep->addChild (wdrawStyle);
	    sep->addChild (wdrawStyle);

	    int nVtx = 0;

	    for (std::vector< DTRecHit1DPair >::const_iterator i = m_rechit.begin (); i != m_rechit.end (); ++i) 
	      {
		//	  const DTRecHit1D* lrechit = (*i).componentRecHit(Left);
		
// 		SoLineSet *line = new SoLineSet;
// 		line->numVertices = 2;
// 		SoVertexProperty *vtx = new SoVertexProperty;


		const DTRecHit1D* lrechit = (*i).componentRecHit(Left);
		const DTRecHit1D* rrechit = (*i).componentRecHit(Right);

		DTWireId wireId = i->wireId();
		const DTLayer* layer = m_pDD->layer(wireId);

 		const DTTopology & topo = layer->specificTopology();

		LocalPoint lLocalPos = lrechit->localPosition();
		LocalPoint rLocalPos = rrechit->localPosition();

		LocalPoint yPlusLeft(lLocalPos.x(),
				     topo.cellLenght()/2.,
				     0.);
		LocalPoint yMinusLeft(lLocalPos.x(),
				     -topo.cellLenght()/2.,
				     0.);
		LocalPoint yPlusRight(rLocalPos.x(),
				     topo.cellLenght()/2.,
				     0.);
		LocalPoint yMinusRight(rLocalPos.x(),
				     -topo.cellLenght()/2.,
				     0.);

		GlobalPoint lPlusGlobalPos = layer->toGlobal(yPlusLeft);
		GlobalPoint lMinusGlobalPos = layer->toGlobal(yMinusLeft);
		GlobalPoint rPlusGlobalPos = layer->toGlobal(yPlusRight);
		GlobalPoint rMinusGlobalPos = layer->toGlobal(yMinusRight);

 		GlobalPoint lGlobalPos = layer->toGlobal(lLocalPos);
 		GlobalPoint rGlobalPos = layer->toGlobal(rLocalPos);

		SoLineSet *linel = new SoLineSet;
		linel->numVertices = 2;
		SoVertexProperty *vtxl = new SoVertexProperty;


		vtxl->vertex.set1Value(0,SbVec3f(lPlusGlobalPos.x()/100.,
						 lPlusGlobalPos.y()/100.,
						 lPlusGlobalPos.z()/100.)); //cm->m
		vtxl->vertex.set1Value(1,SbVec3f(lMinusGlobalPos.x()/100.,
						 lMinusGlobalPos.y()/100.,
						 lMinusGlobalPos.z()/100.)); //cm->m
		linel->vertexProperty = vtxl;

		SoLineSet *liner = new SoLineSet;
		liner->numVertices = 2;
		SoVertexProperty *vtxr = new SoVertexProperty;


		vtxr->vertex.set1Value(0,SbVec3f(rPlusGlobalPos.x()/100.,
						 rPlusGlobalPos.y()/100.,
						 rPlusGlobalPos.z()/100.)); //cm->m
		vtxr->vertex.set1Value(1,SbVec3f(rMinusGlobalPos.x()/100.,
						 rMinusGlobalPos.y()/100.,
						 rMinusGlobalPos.z()/100.)); //cm->m
		liner->vertexProperty = vtxr;

		sep->addChild (linel);
		sep->addChild (liner);


		vtx->vertex.set1Value(nVtx,SbVec3f(lGlobalPos.x()/100.,
						   lGlobalPos.y()/100.,
						   lGlobalPos.z()/100.)); //cm->m
		nVtx++;
		vtx->vertex.set1Value(nVtx,SbVec3f(rGlobalPos.x()/100.,
						   rGlobalPos.y()/100.,
						   rGlobalPos.z()/100.)); //cm->m
		nVtx++;

		SoSeparator* separator = new SoSeparator;		
		addWire (separator, *i);
		sep->addChild (separator);

	      }
	    if (nVtx>0) {
	      vtx->vertex.setNum (nVtx);
	      SoPointSet* pset = new SoPointSet;
	      pset->vertexProperty = vtx;
	      sep->addChild(pset);

// 	      SoSeparator* separator = new SoSeparator;		
// 	      addWire (separator, *i);
// 	      sep->addChild (separator);
	    }
	  }
	catch (...) 
	  {
	    std::cout << "No DTRecHit RecHits." << std::endl;
	  }
    
	rep->node ()->addChild (sep);
      }
}


void
VisDTRecHitsTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if ((! m_rechit.empty ()) &&  m_pDD.isValid ())
      {
	SoSeparator *sep = new SoSeparator;
	
	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize.setValue (1.0);
	sep->addChild(drawStyle);
	try
	  {
	    SoVertexProperty *vtx = new SoVertexProperty;
	    vtx->materialBinding = SoVertexProperty::OVERALL;
	    vtx->orderedRGBA = 0x0000FFFF; // yellow;

// 	    SoLineSet *line = new SoLineSet;
// 	    line->numVertices = 2;
	  
	    SoDrawStyle *wdrawStyle = new SoDrawStyle;
	    wdrawStyle->style = SoDrawStyle::LINES;
	    wdrawStyle->lineWidth.setValue (1.0);
	    //	    wsep->addChild (wdrawStyle);
	    sep->addChild (wdrawStyle);

	    int nVtx = 0;

	    for (std::vector< DTRecHit1DPair >::const_iterator i = m_rechit.begin (); i != m_rechit.end (); ++i) 
	      {
		//	  const DTRecHit1D* lrechit = (*i).componentRecHit(Left);
		
// 		SoLineSet *line = new SoLineSet;
// 		line->numVertices = 2;
// 		SoVertexProperty *vtx = new SoVertexProperty;


		const DTRecHit1D* lrechit = (*i).componentRecHit(Left);
		const DTRecHit1D* rrechit = (*i).componentRecHit(Right);

		DTWireId wireId = i->wireId();
		const DTLayer* layer = m_pDD->layer(wireId);

 		const DTTopology & topo = layer->specificTopology();

		LocalPoint lLocalPos = lrechit->localPosition();
		LocalPoint rLocalPos = rrechit->localPosition();

		LocalPoint yPlusLeft(lLocalPos.x(),
				     topo.cellLenght()/2.,
				     0.);
		LocalPoint yMinusLeft(lLocalPos.x(),
				     -topo.cellLenght()/2.,
				     0.);
		LocalPoint yPlusRight(rLocalPos.x(),
				     topo.cellLenght()/2.,
				     0.);
		LocalPoint yMinusRight(rLocalPos.x(),
				     -topo.cellLenght()/2.,
				     0.);

		GlobalPoint lPlusGlobalPos = layer->toGlobal(yPlusLeft);
		GlobalPoint lMinusGlobalPos = layer->toGlobal(yMinusLeft);
		GlobalPoint rPlusGlobalPos = layer->toGlobal(yPlusRight);
		GlobalPoint rMinusGlobalPos = layer->toGlobal(yMinusRight);

 		GlobalPoint lGlobalPos = layer->toGlobal(lLocalPos);
 		GlobalPoint rGlobalPos = layer->toGlobal(rLocalPos);

		SoLineSet *linel = new SoLineSet;
		linel->numVertices = 2;
		SoVertexProperty *vtxl = new SoVertexProperty;


		vtxl->vertex.set1Value(0,SbVec3f(lPlusGlobalPos.x()/100.,
						 lPlusGlobalPos.y()/100.,
						 lPlusGlobalPos.z()/100.)); //cm->m
		vtxl->vertex.set1Value(1,SbVec3f(lMinusGlobalPos.x()/100.,
						 lMinusGlobalPos.y()/100.,
						 lMinusGlobalPos.z()/100.)); //cm->m
		linel->vertexProperty = vtxl;

		SoLineSet *liner = new SoLineSet;
		liner->numVertices = 2;
		SoVertexProperty *vtxr = new SoVertexProperty;


		vtxr->vertex.set1Value(0,SbVec3f(rPlusGlobalPos.x()/100.,
						 rPlusGlobalPos.y()/100.,
						 rPlusGlobalPos.z()/100.)); //cm->m
		vtxr->vertex.set1Value(1,SbVec3f(rMinusGlobalPos.x()/100.,
						 rMinusGlobalPos.y()/100.,
						 rMinusGlobalPos.z()/100.)); //cm->m
		liner->vertexProperty = vtxr;

		sep->addChild (linel);
		sep->addChild (liner);


		vtx->vertex.set1Value(nVtx,SbVec3f(lGlobalPos.x()/100.,
						   lGlobalPos.y()/100.,
						   lGlobalPos.z()/100.)); //cm->m
		nVtx++;
		vtx->vertex.set1Value(nVtx,SbVec3f(rGlobalPos.x()/100.,
						   rGlobalPos.y()/100.,
						   rGlobalPos.z()/100.)); //cm->m
		nVtx++;

		SoSeparator* separator = new SoSeparator;		
		addWire (separator, *i);
		sep->addChild (separator);

	      }
	    if (nVtx>0) {
	      vtx->vertex.setNum (nVtx);
	      SoPointSet* pset = new SoPointSet;
	      pset->vertexProperty = vtx;
	      sep->addChild(pset);

// 	      SoSeparator* separator = new SoSeparator;		
// 	      addWire (separator, *i);
// 	      sep->addChild (separator);
	    }
	  }
	catch (...) 
	  {
	    std::cout << "No DTRecHit RecHits." << std::endl;
	  }
    
	rep->node ()->addChild (sep);
      }
}

/** Transform and add the wire to the scene graph */
void
VisDTRecHitsTwig::addWire (SoSeparator *separator, const DTRecHit1DPair &rechit)
{    
  Surface::GlobalPoint cpos (0.0, 0.0, 0.0);
    
    DTWireId wireId = rechit.wireId();
    const DTLayer* layer = m_pDD->layer(wireId);
    DTLayerId layerId = wireId.layerId();
    const GeomDetUnit *det = m_pDD->idToDetUnit (layerId);

    //    const DTLayer *layer = m_pDD->layer (digi.first);
    //    const GeomDetUnit *det = m_pDD->idToDetUnit (digi.first);
    int wireNumber = wireId.wire();
    
    // Position of the wire in the Layer's reference frame
    float localXPos = layer->specificTopology ().wirePosition (wireNumber);
    LocalPoint localPos (localXPos, 0, 0);
    // Position of the wire in the CMS's ref.frame
    //    cpos = layer->surface ().toGlobal (localPos);
    cpos = layer->toGlobal (localPos);

    float posx = cpos.x () / 100.0;  // cm -> m
    float posy = cpos.y () / 100.0;  // cm -> m
    float posz = cpos.z () / 100.0;  // cm -> m

    float angle;
    SbVec3f axis;
    createRotation (det, axis, angle);

    SoTransform *transform = new SoTransform;
    transform->translation.setValue (posx, posy, posz);    
    transform->rotation.setValue (axis, angle);

    SoCube *cube = new SoCube;
    cube->width = layer->specificTopology ().cellWidth () / 100.0;   // cm -> m
    cube->height = layer->specificTopology ().cellLenght () / 100.0; // cm -> m
    cube->depth =  layer->specificTopology ().cellHeight () / 100.0; // cm -> m
    
    separator->addChild (transform);
    separator->addChild (cube);
}
