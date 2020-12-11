//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuonDT/interface/VisDT2DSegmentTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/DTRecHit/interface/DTRecHit1D.h"
#include "DataFormats/DTRecHit/interface/DTRecHit1DPair.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"

#include "DataFormats/DTRecHit/interface/DTSLRecSegment2D.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment2DCollection.h"

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
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
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


//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

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
	return new VisDT2DSegmentTwig (state, parent, "DT RecSegment2D (" + name + ")",
				       friendlyName, moduleLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisDT2DSegmentTwig::VisDT2DSegmentTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID dt2DSegID (typeid (DTRecSegment2DCollection));
    tfService->registerTwig (dt2DSegID.friendlyClassName (), &createThisTwig);
}

void
VisDT2DSegmentTwig::onNewEvent (const edm::Event &event,
				const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_recseg.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    // Get the rec segments collection from the event
    std::vector<edm::Handle<DTRecSegment2DCollection> > recsegCollections;

    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, recsegCollections);
	}
	else
	{
	    event.getManyByType (recsegCollections);
	}
	// event.getByLabel("recseg2dbuilder", recsegCollection);
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
	std::vector<Handle<DTRecSegment2DCollection> >::iterator i;
	std::vector<Handle<DTRecSegment2DCollection> >::iterator iEnd;
	for (i = recsegCollections.begin (), iEnd = recsegCollections.end (); i != iEnd; ++i) 
	{
	    const DTRecSegment2DCollection& c = *(*i);
	    for (DTRecSegment2DCollection::const_iterator rsi = c.begin (), rsiEnd = c.end ();
		 rsi != rsiEnd; ++rsi) 
	    {
		m_recseg.push_back (*rsi);
	    }
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisDT2DSegmentTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    // Prepare property description.
    std::ostringstream  text;

    text << "<b>Muon Barrel: DT Track 2D Segments</b><br>";
    
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>nHits</TH>"
	 << "<TH>Wheel</TH>"
	 << "<TH>Station</TH>"
	 << "<TH>Sector</TH>"
	 << "<TH>SL</TH>"
	 << "<TH>position</TH>"
	 << "<TH>direction</TH>"
	 << "<TH>r [cm]</TH>"
	 << "<TH>phi [rad]</TH>"
	 << "<TH>chi2 / hits</TH>"
	 << "</TR>";
    text << setiosflags (ios::showpoint | ios::fixed);
    text.setf (ios::right, ios::adjustfield);

    try 
    {
	int nSegments = 0;
	for (std::vector< DTSLRecSegment2D >::const_iterator i = m_recseg.begin (), iEnd = m_recseg.end (); i != iEnd; ++i) 
	{
	    //int dimRs = (*i).dimension ();
	    int nHits = (*i).recHits().size();
	    DTSuperLayerId slId ((*i).geographicalId ().rawId ());
	    int slayerIdRs = slId.superLayer ();
	    const DTSuperLayer* slayer = m_pDD->superLayer (slId);

	    LocalVector locDir = (*i).localDirection ();
	    LocalPoint locPos = (*i).localPosition ();

	    GlobalVector gloDir = slayer->toGlobal (locDir);
	    GlobalPoint gloPos = slayer->toGlobal (locPos);

	    float phi = gloPos.phi ();
	    phi = (phi < 0) ? 2 * M_PI + phi : phi;

	    double chi2 = (*i).chi2 ();
	    int dof = (*i).degreesOfFreedom ();

	    int sectorIdRs = (*i).chamberId ().sector ();
	    int stationIdRs = (*i).chamberId().station ();
	    int wheelIdRs = (*i).chamberId ().wheel ();
	
	    text << "<TR align = right>"
		 << "<TD>" << setw (2) << nSegments++ << "</TD>"
		 << "<TD>" << setw (1) << nHits << "</TD>"
		 << "<TD>" << wheelIdRs << "</TD>"
		 << "<TD>" << stationIdRs << "</TD>"
		 << "<TD>" << sectorIdRs << "</TD>"
		 << "<TD>" << slayerIdRs << "</TD>"
		 << "<TD>" << setw (6) << setprecision (2) << gloPos << "</TD>"
		 << "<TD>" << setw (6) << setprecision (2) << gloDir << "</TD>"
		 << "<TD>" << setw (5) << setprecision (1) << gloPos.perp () << "</TD>"
		 << "<TD>" << setw (6) << setprecision (3) << phi << "</TD>"
		 << "<TD>" << setw (6) << setprecision (2) << chi2 / dof << "</TD>"
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
  
    rep->setText (text.str ());
}

void
VisDT2DSegmentTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if ((! m_recseg.empty ()) &&  m_pDD.isValid ())
    {
	SoSeparator *sep = new SoSeparator;
	addMaterial (sep);
	try
	{
	    for (std::vector< DTSLRecSegment2D >::const_iterator i = m_recseg.begin (), iEnd = m_recseg.end (); i != iEnd; ++i) 
	    {
		float halfHeight = 1.3 * 2.0; //cm
		LocalVector locDir = (*i).localDirection ();
		LocalPoint locPos = (*i).localPosition ();
	      
		DTSuperLayerId slId ((*i).geographicalId ().rawId ());
		const DTSuperLayer *slayer = m_pDD->superLayer (slId);
	      
		SoVertexProperty *vtx = new SoVertexProperty;
	      
		SoLineSet *line = new SoLineSet;
		line->numVertices = 2;
	      
		GlobalPoint posInner = slayer->toGlobal (locPos + locDir / locDir.mag () * halfHeight / cos (locDir.theta ()));		  
		float x = posInner.x () / 100.0;  // cm -> m
		float y = posInner.y () / 100.0;  // cm -> m
		float z = posInner.z () / 100.0;  // cm -> m
	      
		vtx->vertex.set1Value (0, SbVec3f (x, y, z));
	      
		GlobalPoint posOuter = slayer->toGlobal (locPos + locDir / (-locDir.mag ()) * halfHeight / cos (locDir.theta ()));		  
		x = posOuter.x () / 100.0;  // cm -> m
		y = posOuter.y () / 100.0;  // cm -> m
		z = posOuter.z () / 100.0;  // cm -> m
	      
		vtx->vertex.set1Value (1, SbVec3f (x, y, z));
	      
		line->vertexProperty = vtx;
	      
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
VisDT2DSegmentTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    
    //........ still missing
}

void
VisDT2DSegmentTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    rep->clear ();
  
    if ((! m_recseg.empty ()) &&  m_pDD.isValid ())
    {
	SoSeparator *sep = new SoSeparator;
	addMaterial (sep);
	
	try
	{
	    for (std::vector< DTSLRecSegment2D >::const_iterator i = m_recseg.begin (), iEnd = m_recseg.end (); i != iEnd; ++i) 
	    {
		float halfHeight = 1.3 * 2.0; //cm
		LocalVector locDir = (*i).localDirection ();
		LocalPoint locPos = (*i).localPosition ();
	      
		DTSuperLayerId slId ((*i).geographicalId ().rawId ());
		const DTSuperLayer* slayer = m_pDD->superLayer (slId);
	      
		SoVertexProperty *vtx = new SoVertexProperty;
	      
		SoLineSet *line = new SoLineSet;
		line->numVertices = 2;
	      
		GlobalPoint posInner = slayer->toGlobal (locPos + locDir / locDir.mag () * halfHeight / cos (locDir.theta ()));		  
		float x = posInner.x () / 100.0;  // cm -> m
		float y = posInner.y () / 100.0;  // cm -> m
		float z = posInner.z () / 100.0;  // cm -> m
	      
		vtx->vertex.set1Value (0, SbVec3f (x, y, z));
	      
		GlobalPoint posOuter = slayer->toGlobal (locPos + locDir / (-locDir.mag ()) * halfHeight / cos (locDir.theta ()));		  
		x = posOuter.x () / 100.0;  // cm -> m
		y = posOuter.y () / 100.0;  // cm -> m
		z = posOuter.z () / 100.0;  // cm -> m
	      
		vtx->vertex.set1Value (1, SbVec3f (x, y, z));
	      
		line->vertexProperty = vtx;
	      
		sep->addChild (line);
	      
		SoDrawStyle *drawStyle = new SoDrawStyle;
		drawStyle->pointSize.setValue (2.0);
		sep->addChild(drawStyle);

		SoVertexProperty *pti = new SoVertexProperty;
		pti->materialBinding = SoVertexProperty::OVERALL;
		pti->orderedRGBA = 0x9900cc00; // yellow;

		std::vector<DTRecHit1D> segRecHit = (*i).specificRecHits ();

		int npti = 0;

		for (std::vector< DTRecHit1D >::const_iterator rechit = segRecHit.begin (), rechitEnd = segRecHit.end (); 
		     rechit != rechitEnd; ++rechit)
		{
		    DTWireId wireId = rechit->wireId ();
		    const DTLayer* layer = m_pDD->layer (wireId);

		    LocalPoint LocalPos = rechit->localPosition ();
		    GlobalPoint GlobalPos = layer->toGlobal (LocalPos);

		    pti->vertex.set1Value (npti, SbVec3f (GlobalPos.x () / 100.0,
							  GlobalPos.y () / 100.0,
							  GlobalPos.z () / 100.0)); //cm->m
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
VisDT2DSegmentTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if ((! m_recseg.empty ()) &&  m_pDD.isValid ())
    {      
	SoSeparator *sep = new SoSeparator;
	addMaterial (sep);
	try
	{
	    for (std::vector< DTSLRecSegment2D >::const_iterator i = m_recseg.begin (), iEnd = m_recseg.end (); i != iEnd; ++i) 
	    {	      
		float halfHeight = 1.3 * 2.0; // cm
		LocalVector locDir = (*i).localDirection ();
		LocalPoint locPos = (*i).localPosition ();
	      
		DTSuperLayerId slId ((*i).geographicalId ().rawId ());
		const DTSuperLayer *slayer = m_pDD->superLayer (slId);
	      
		SoVertexProperty *vtx = new SoVertexProperty;
	      
		SoLineSet *line = new SoLineSet;
		line->numVertices = 2;
	      
		GlobalPoint posInner = slayer->toGlobal (locPos + locDir / locDir.mag () * halfHeight / cos (locDir.theta ()));		  
		float x = posInner.x () / 100.0;  // cm -> m
		float y = posInner.y () / 100.0;  // cm -> m
		float z = posInner.z () / 100.0;  // cm -> m
	      
		vtx->vertex.set1Value (0, SbVec3f (x, y, z));
	      
		GlobalPoint posOuter = slayer->toGlobal (locPos + locDir / (-locDir.mag ()) * halfHeight / cos (locDir.theta ()));		  
		x = posOuter.x () / 100.0;  // cm -> m
		y = posOuter.y () / 100.0;  // cm -> m
		z = posOuter.z () / 100.0;  // cm -> m
	      
		vtx->vertex.set1Value (1, SbVec3f (x, y, z));
	      
		line->vertexProperty = vtx;
	      
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
VisDT2DSegmentTwig::addMaterial (SoSeparator *sep) 
{
    SoMaterial *segment = new SoMaterial;  // green
    segment->diffuseColor.setValue (0., 1., 0.);
  
    sep->addChild (segment);
}
