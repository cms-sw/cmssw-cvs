#include "VisReco/VisMuonDT/interface/VisDT4DSegmentTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/DTRecHit/interface/DTRecHit1D.h"
#include "DataFormats/DTRecHit/interface/DTRecHit1DPair.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"

#include "DataFormats/DTRecHit/interface/DTRecSegment4D.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"

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
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
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
		    const std::string &moduleLabel,
		    const std::string &instanceName,
		    const std::string &processName)
    {
	IgTwig *rootTwig = IgDocumentData::get (state)->root ();
	IgTwig *eventTwig = 0;
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Muon DT Event/DT 4DSegments");
	
	if (! eventTwig)
	    eventTwig = parent;

	return new VisDT4DSegmentTwig (state, eventTwig, "[N/A] DT RecSegment4D (" + name + ")",
				       friendlyName, moduleLabel, instanceName, processName);
    }
}

VisDT4DSegmentTwig::VisDT4DSegmentTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID dt4DSegID (typeid (DTRecSegment4DCollection));
    tfService->registerTwig (dt4DSegID.friendlyClassName (), &createThisTwig);
}

void
VisDT4DSegmentTwig::onNewEvent (const edm::Event &event,
				const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_recseg.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    // Get the rec segments collection from the event
    std::vector<edm::Handle<DTRecSegment4DCollection> > recsegCollections;

    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, recsegCollections);
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

    if (! recsegCollections.empty ())
    {
	std::vector<Handle<DTRecSegment4DCollection> >::iterator i;
	std::vector<Handle<DTRecSegment4DCollection> >::iterator iEnd;
	for (i = recsegCollections.begin (), iEnd = recsegCollections.end (); i != iEnd; ++i) 
	{
	    const DTRecSegment4DCollection& c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    this->name (nameStr.toStdString ());

	    for (DTRecSegment4DCollection::const_iterator rsi = c.begin (), rsiEnd = c.end ();
		 rsi != rsiEnd; ++rsi) 
	    {
		m_recseg.push_back (*rsi);
	    }
	}
    }
        
    IgRepSet::invalidate (this->parent (), IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);   
}

void
VisDT4DSegmentTwig::update (IgTextRep *rep)
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
    text << "<b>Muon Barrel: DT Track 4D Segments</b><br>";
    
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>nHits</TH>"
	 << "<TH>Wheel</TH>"
	 << "<TH>Station</TH>"
	 << "<TH>Sector</TH>"
	 << "<TH>position</TH>"
	 << "<TH>direction</TH>"
	 << "<TH>r [cm]</TH>"
	 << "<TH>phi [rad]</TH>"
	 << "<TH>chi2 / hits</TH>"
      //	 << "<TH>chi2 z / hits</TH>"
	 << "</TR>";
    text << setiosflags (ios::showpoint | ios::fixed);
    text.setf (ios::right, ios::adjustfield);

    try 
    {
      int nSegments = 0;
      for (std::vector< DTRecSegment4D >::const_iterator i = m_recseg.begin (), iEnd = m_recseg.end (); i != iEnd; ++i) 
      {
	//int dimRs = (*i).dimension ();
	std::vector<const TrackingRecHit*> hits = (*i).recHits();
	int nHits = 0;
	for (std::vector<const TrackingRecHit*>::const_iterator h=hits.begin() ;
	     h!=hits.end(); ++h) nHits+= (*h)->recHits().size();
	DTChamberId chId ((*i).geographicalId ().rawId ());
	const DTChamber *chamber = m_pDD->chamber (chId);

	LocalVector locDir = (*i).localDirection ();
	LocalPoint locPos = (*i).localPosition ();

	GlobalVector gloDir = chamber->toGlobal (locDir);
	GlobalPoint gloPos = chamber->toGlobal (locPos);

	float phi = gloPos.phi ();
	phi = (phi < 0) ? 2 * M_PI + phi : phi;

	double chi2 = (*i).chi2 ();
	int dof = (*i).degreesOfFreedom ();

	int sectorIdRs = chId.sector ();
	int stationIdRs = chId.station ();
	int wheelIdRs = chId.wheel ();
	
	text << "<TR align = right>"
	     << "<TD>" << setw (2) << nSegments++ << "</TD>"
	     << "<TD>" << setw (1) << nHits << "</TD>"
	     << "<TD>" << wheelIdRs << "</TD>"
	     << "<TD>" << stationIdRs << "</TD>"
	     << "<TD>" << sectorIdRs << "</TD>"
	     << "<TD>" << setw (6) << setprecision (2) << gloPos << "</TD>"
	     << "<TD>" << setw (6) << setprecision (2) << gloDir << "</TD>"
	     << "<TD>" << setw (5) << setprecision (1) << gloPos.perp() << "</TD>"
	     << "<TD>" << setw (6) << setprecision (3) << phi << "</TD>"
	     << "<TD>" << setw (6) << setprecision (2) << chi2/dof << "</TD>"
	     << "</TR>";
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
  
    text << "</table>";
  }
  
  rep->setText (text.str ());
}

void
VisDT4DSegmentTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if ((! m_recseg.empty ()) &&  m_pDD.isValid ())
    {
	
	SoSeparator *sep = new SoSeparator;

	try
	{
	    for (std::vector< DTRecSegment4D >::const_iterator i = m_recseg.begin (), iEnd = m_recseg.end (); i != iEnd; ++i) 
	    {
		DTChamberId chId ((*i).geographicalId ().rawId ());
		const DTChamber *chamber = m_pDD->chamber (chId);	   
		
		float halfHeight = chamber->surface ().bounds ().thickness () / 2.0;
		float halfWidth = chamber->surface ().bounds ().width () / 2.0;
		LocalVector locDir = (*i).localDirection ();
		LocalPoint locPos = (*i).localPosition ();
		LocalPoint locPosInner = locPos + locDir / locDir.mag () * halfHeight / cos (locDir.theta ());
		if (fabs(locPosInner.y ()) >= halfWidth)
		    locPosInner = locPos + locDir / locDir.mag () * halfWidth / sin (locDir.theta ());
		
		LocalPoint locPosOuter = locPos + locDir / (-locDir.mag ()) * halfHeight / cos (locDir.theta ());		  
		if (fabs(locPosOuter.y ()) >= halfWidth)
		    locPosOuter = locPos + locDir / (-locDir.mag ()) * halfWidth / sin (locDir.theta ());
		
		SoVertexProperty *vtx = new SoVertexProperty;
		
		SoLineSet *line = new SoLineSet;
		line->numVertices = 2;
		
		GlobalPoint posInner = chamber->toGlobal (locPosInner);		  
		float x = posInner.x () / 100.0;  // cm -> m
		float y = posInner.y () / 100.0;  // cm -> m
		float z = posInner.z () / 100.0;  // cm -> m
		
		vtx->vertex.set1Value (0, SbVec3f (x, y, z));
		
		GlobalPoint posOuter = chamber->toGlobal (locPosOuter);		  
		x = posOuter.x () / 100.0;  // cm -> m
		y = posOuter.y () / 100.0;  // cm -> m
		z = posOuter.z () / 100.0;  // cm -> m
		
		vtx->vertex.set1Value (1, SbVec3f (x, y, z));
		
		line->vertexProperty = vtx;

		SoMaterial *wmat = new SoMaterial;
		float rgbcomponents [4];
		IgSbColorMap::unpack (0xff000000, rgbcomponents);
		wmat->diffuseColor.setValue (SbColor (rgbcomponents));
		sep->addChild (wmat);	
	       
		SoDrawStyle *sty = new SoDrawStyle;
		sty->style = SoDrawStyle::LINES;
		sty->lineWidth.setValue (4.0);
		sep->addChild (sty);

		sep->addChild (line);		
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

	rep->node ()->addChild (sep);	
    }
}

void
VisDT4DSegmentTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    
    //........ still missing
}

void
VisDT4DSegmentTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    rep->clear ();
  
    if ((! m_recseg.empty ()) &&  m_pDD.isValid ())
    {
      
	SoSeparator *sep = new SoSeparator;

	try
	{
	    for (std::vector< DTRecSegment4D >::const_iterator i = m_recseg.begin (), iEnd = m_recseg.end (); i != iEnd; ++i) 
	    {
		DTChamberId chId ((*i).geographicalId ().rawId ());
		const DTChamber* chamber = m_pDD->chamber (chId);	   

		float halfHeight = chamber->surface ().bounds ().thickness () / 2.0;
		LocalVector locDir = (*i).localDirection ();
		LocalPoint locPos = (*i).localPosition ();
	      
		SoVertexProperty *vtx = new SoVertexProperty;
	      
		SoLineSet *line = new SoLineSet;
		line->numVertices = 2;

		GlobalPoint posInner = chamber->toGlobal (locPos + locDir / locDir.mag () * halfHeight / cos (locDir.theta ()));		  
		float x = posInner.x () / 100.0;  // cm -> m
		float y = posInner.y () / 100.0;  // cm -> m
		float z = posInner.z () / 100.0;  // cm -> m
	      
		vtx->vertex.set1Value (0, SbVec3f (x, y, z));

		GlobalPoint posOuter = chamber->toGlobal (locPos + locDir / (-locDir.mag ()) * halfHeight / cos (locDir.theta ()));		  
		x = posOuter.x () / 100.0;  // cm -> m
		y = posOuter.y () / 100.0;  // cm -> m
		z = posOuter.z () / 100.0;  // cm -> m
	      
		vtx->vertex.set1Value (1, SbVec3f (x, y, z));
	      
		line->vertexProperty = vtx;

		SoMaterial *wmat = new SoMaterial;
		float rgbcomponents [4];
		IgSbColorMap::unpack (0xff000000, rgbcomponents);
		wmat->diffuseColor.setValue (SbColor (rgbcomponents));
		sep->addChild (wmat);	
	      
		SoDrawStyle *sty = new SoDrawStyle;
		sty->style = SoDrawStyle::LINES;
		sty->lineWidth.setValue (4.0);
		sep->addChild (sty);
	      
		sep->addChild (line);
	      
		SoDrawStyle *drawStyle = new SoDrawStyle;
		drawStyle->pointSize.setValue (2.0);
		sep->addChild (drawStyle);
	      
		SoVertexProperty *pti = new SoVertexProperty;
		pti->materialBinding = SoVertexProperty::OVERALL;
		pti->orderedRGBA = 0x9900cc00;

		if ((*i).phiSegment () && (*i).zSegment ()) 
		{	    
		    std::vector<DTRecHit1D> segRecHitPhi = (*i).phiSegment ()->specificRecHits ();
		    std::vector<DTRecHit1D> segRecHitZ = (*i).zSegment ()->specificRecHits ();

		    int npti = 0;

		    for (std::vector<DTRecHit1D>::const_iterator rechit = segRecHitPhi.begin (), rechitEnd = segRecHitPhi.end (); 
			 rechit != rechitEnd; ++rechit)
		    {
			DTWireId wireId = rechit->wireId ();
			const DTLayer* layer = m_pDD->layer (wireId);
		  
			LocalPoint LocalPos = rechit->localPosition ();
			GlobalPoint GlobalPos = layer->toGlobal (LocalPos);
		  
			pti->vertex.set1Value (npti, SbVec3f (GlobalPos.x () / 100.0,
							      GlobalPos.y () / 100.0,
							      GlobalPos.z () / 100.0)); // cm->m
			++npti;
		    } 

		    for (std::vector<DTRecHit1D>::const_iterator rechit = segRecHitZ.begin (), rechitEnd = segRecHitZ.end (); 
			 rechit != rechitEnd; ++rechit)
		    {
			DTWireId wireId = rechit->wireId ();
			const DTLayer *layer = m_pDD->layer (wireId);
		  
			LocalPoint LocalPos = rechit->localPosition ();
			GlobalPoint GlobalPos = layer->toGlobal (LocalPos);
		  
			pti->vertex.set1Value (npti, SbVec3f (GlobalPos.x () / 100.0,
							      GlobalPos.y () / 100.0,
							      GlobalPos.z () / 100.0)); // cm->m
			++npti;
		    } 

		    if (npti > 0) 
		    {
			pti->vertex.setNum (npti);
			SoPointSet *pset = new SoPointSet;
			pset->vertexProperty = pti;
			sep->addChild (pset);
		    }
		}
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
      
	rep->node ()->addChild (sep);      
    }
}

void
VisDT4DSegmentTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
  
    if ((! m_recseg.empty ()) &&  m_pDD.isValid ())
    {
	SoSeparator *sep = new SoSeparator;

	try
	{
	    for (std::vector< DTRecSegment4D >::const_iterator i = m_recseg.begin (), iEnd = m_recseg.end (); i != iEnd; ++i) 
	    {
		DTChamberId chId ((*i).geographicalId ().rawId ());
		const DTChamber *chamber = m_pDD->chamber (chId);	   
	      
		float halfHeight = chamber->surface ().bounds ().thickness () / 2.0;
		LocalVector locDir = (*i).localDirection ();
		LocalPoint locPos = (*i).localPosition ();
	      
		SoVertexProperty *vtx = new SoVertexProperty;
	      
		SoLineSet *line = new SoLineSet;
		line->numVertices = 2;
	      
		GlobalPoint posInner = chamber->toGlobal (locPos + locDir / locDir.mag () * halfHeight / cos (locDir.theta ()));		  
		float x = posInner.x () / 100.0;  // cm -> m
		float y = posInner.y () / 100.0;  // cm -> m
		float z = posInner.z () / 100.0;  // cm -> m
	      
		vtx->vertex.set1Value (0, SbVec3f (x, y, z));
	      
		GlobalPoint posOuter = chamber->toGlobal (locPos + locDir / (-locDir.mag ()) * halfHeight / cos (locDir.theta ()));		  
		x = posOuter.x () / 100.0;  // cm -> m
		y = posOuter.y () / 100.0;  // cm -> m
		z = posOuter.z () / 100.0;  // cm -> m
	      
		vtx->vertex.set1Value (1, SbVec3f (x, y, z));
	      
		line->vertexProperty = vtx;
	      
		SoMaterial *wmat = new SoMaterial;
		float rgbcomponents [4];
		IgSbColorMap::unpack (0xff000000, rgbcomponents);
		wmat->diffuseColor.setValue (SbColor (rgbcomponents));
		sep->addChild (wmat);	
	      
		SoDrawStyle *sty = new SoDrawStyle;
		sty->style = SoDrawStyle::LINES;
		sty->lineWidth.setValue (4.0);
		sep->addChild (sty);

		sep->addChild (line);
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
      
	rep->node ()->addChild (sep);      
    }
}


/** Define material */
void
VisDT4DSegmentTwig::addMaterial (SoSeparator *sep) 
{
    SoMaterial *segment = new SoMaterial;  // green
    segment->diffuseColor.setValue (0., 1., 0.);
  
    sep->addChild (segment);
}
