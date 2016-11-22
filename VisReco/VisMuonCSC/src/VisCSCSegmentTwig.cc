//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuonCSC/interface/VisCSCSegmentTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCChamberDrawer.h"
#include "VisReco/VisMuonCSC/interface/VisCSCConfigurationService.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegment.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CSCGeometry/interface/CSCLayer.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/GLBrowsers/interface/IgTwigConfigElement.h"
#include "Iguana/Inventor/interface/IgSoArrow.h"
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoLineSet.h>
#include <classlib/utils/DebugAids.h>
#include <qstring.h>
#include <iostream>
#include <iomanip>

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
const edm::ParameterSet*      VisCSCSegmentTwig::m_pset = 0;

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
	return new VisCSCSegmentTwig (state, parent, "CSC Segments (" + name + ")", 
				      friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCSCSegmentTwig::VisCSCSegmentTwig (IgState *state, IgTwig *parent,
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
      m_lineThickness(0.F),
      m_lineThicknessEnvelope(2.0F),
      m_lineColor(0xFF000000),
      m_lineColorEnvelope(0xFF000000),
      m_envelopeFlag(true)
{    
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID digiCollID (typeid (CSCSegmentCollection));
    tfService->registerTwig (digiCollID.friendlyClassName (), &createThisTwig);

    m_pset = VisCSCConfigurationService::pSet ();
    if (! m_pset)
    {
        edm::ParameterSet p;
        VisCSCConfigurationService visService(p);
        m_pset = visService.pSet ();
    }
    m_envelopeFlag = m_pset->getUntrackedParameter<bool>("VisCSCSegmentEnvelopeFlag",false);
}

void
VisCSCSegmentTwig::onNewEvent (const edm::Event &event,
			       const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_segs.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<CSCSegmentCollection> > segCollections;
    
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, segCollections);
	}
	else
	{
	    event.getManyByType (segCollections);
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

    if (! segCollections.empty ())
    {
	std::vector<edm::Handle<CSCSegmentCollection> >::iterator i;
	std::vector<edm::Handle<CSCSegmentCollection> >::iterator iEnd;
	for (i = segCollections.begin (), iEnd = segCollections.end (); i != iEnd; ++i) 
	{
	    const CSCSegmentCollection& c = *(*i);
    
	    CSCSegmentCollection::const_iterator segIt;
	    CSCSegmentCollection::const_iterator segItEnd;
	    for (segIt = c.begin (), segItEnd = c.end (); segIt != segItEnd; ++segIt)
	    {
		m_segs.push_back (*segIt);
	    }
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

/** Update object property description. */
void
VisCSCSegmentTwig::update (IgTextRep *rep) 
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare the property description.
    std::ostringstream text;
    text << "Muon Endcap: CSC Track Segments<br>";

    if (! m_segs.empty () && m_pDD.isValid ())
    {
	try 
	{
	    text << "<table width='100%' border=1>"
		 << "<TR align = center>"
		 << "<TH>Number</TH>"
		 << "<TH>dim</TH>"
		 << "<TH>position</TH>"
		 << "<TH>direction</TH>"
		 << "<TH>z [cm]</TH>"
		 << "<TH>phi [rad]</TH>"
		 << "<TH>chi2 / hits</TH>"
		 << "</TR>";
	    text << setiosflags (std::ios::showpoint | std::ios::fixed);
	    text.setf (std::ios::right, std::ios::adjustfield);
	
	    int nSegments = 0;
	
	    for (std::vector<CSCSegment>::const_iterator iseg = m_segs.begin (), isegEnd = m_segs.end (); iseg != isegEnd; ++iseg) 
	    {
		const GeomDet *det = m_pDD->idToDet ((*iseg).cscDetId ());
		
		GlobalPoint pos = det->surface ().toGlobal ((*iseg).localPosition ());
		GlobalVector dir = det->surface ().toGlobal ((*iseg).localDirection ());
	    
		text << "<TR align = right>"
		     << "<TD>" << std::setw (2) << ++nSegments << "</TD>"
		     << "<TD>" << std::setw (1) << "(*iseg).dimension ()" << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (2) << pos << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (2) << dir << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (1) << pos.z () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << pos.phi () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (2) << (*iseg).chi2 () 
		     << " / " << (*iseg).nRecHits () << "</TD>"
		     << "</TR>";    
	    }
	    text << "</table>";
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
    
    // Send it over.
    rep->setText (text.str ());
}

/** Update object property description. */
void
VisCSCSegmentTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    m_dets.clear ();

    
    if (! m_segs.empty () && m_pDD.isValid ())
    {

	IgTwigConfigElement *element = IgTwigConfigElement::get(state());
	SoMaterial *mat = element->material("VisCSCSegmentTwig/3D");
	SoDrawStyle *ds = element->drawStyle("VisCSCSegmentTwig/3D");

        SoSeparator *sep = new SoSeparator;    
	VisCSCChamberDrawer d(mat,ds);

	mat = element->material("VisCSCSegmentTwig/3D/Segment");
	ds = element->drawStyle("VisCSCSegmentTwig/3D/Segment");

	SoMaterial *matenv = element->material("VisCSCSegmentTwig/3D/SegmentEnvelope");
	SoDrawStyle *dsenv = element->drawStyle("VisCSCSegmentTwig/3D/SegmentEnvelope");
	try 
	{
	    //
	    // Muon Endcap Segment
	    // 
	    int nVtx = 0;
	    SoCoordinate3 *pts = new SoCoordinate3;
	    SoLineSet *lines = new SoLineSet;

	    for (std::vector<CSCSegment>::const_iterator iseg = m_segs.begin (), isegEnd = m_segs.end (); iseg != isegEnd; ++iseg) 
	    {
		const CSCDetId& id = (*iseg).cscDetId();
		const GeomDet *det = m_pDD->idToDet (id);
		
		// This is a frame around the (active) layer
		if (m_dets.find(id) == m_dets.end() )
        	{
                    m_dets.insert(id);	
		    // This is a frame around the (active) layer
		    d.getOutline(sep, m_pDD, id);
        	}
				    
		// Local pos & dir
                LocalPoint  pos = (*iseg).localPosition();
                LocalVector dir = (*iseg).localDirection();

		// Chamber thickness (in z)
                float halfThickness = det->surface ().bounds ().thickness () / 2.;
		
		
                float z1 = halfThickness;
		float x1 = pos.x() + dir.x()*z1/dir.z();
		float y1 = pos.y() + dir.y()*z1/dir.z();
                GlobalPoint g1 = det->surface().toGlobal( LocalPoint(x1,y1,z1) );

                float z2 = -halfThickness;
		float x2 = pos.x() + dir.x()*z2/dir.z();
		float y2 = pos.y() + dir.y()*z2/dir.z();
                GlobalPoint g2 = det->surface().toGlobal( LocalPoint(x2,y2,z2) );
		
		SbVec3f ptA( g1.x(), g1.y(), g1.z() );
		ptA *= 0.01F; // cm -> m
		
		
		lines->numVertices.set1Value(nVtx/2, 2);
		pts->point.set1Value ( nVtx++, ptA );
		SbVec3f ptB( g2.x(), g2.y(), g2.z() );
		ptB *= 0.01F; // cm->m
		
		pts->point.set1Value ( nVtx++, ptB );
		
	    }
	    sep->addChild(pts);
	    sep->addChild(mat);
	    sep->addChild(ds);
	    sep->addChild(lines);
	    if (m_envelopeFlag) // envelope
	    {
	        sep->addChild(matenv);
	        sep->addChild(dsenv);
	        SoLineSet *envelope = new SoLineSet;
		sep->addChild(envelope);
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

/** Update object property description. */
void
VisCSCSegmentTwig::update (IgLegoRep *rep)
{}

/** Update object property description. */
void
VisCSCSegmentTwig::update (IgRPhiRep *rep)
{
   // Get debugging dump.
    VisQueuedTwig::update (rep);

    SoSeparator *sep = new SoSeparator;
    SoMaterial *mat = new SoMaterial;
    mat->ambientColor.setValue (0.135, 0.2225, 0.1575);

    mat->diffuseColor.setValue (1.00, 0.28, 0.49); // light red

    mat->specularColor.setValue (0.316228, 0.316228, 0.316228);
    mat->emissiveColor.setValue(0.0, 0.0, 0.0);
    mat->shininess = 0.1;
    sep->addChild (mat);

    SoDrawStyle *sty = new SoDrawStyle;
    sty->style = SoDrawStyle::LINES;
    sty->lineWidth.setValue (1.0);
    sep->addChild (sty);
    
    if (! m_segs.empty () && m_pDD.isValid ())
    {
	try 
	{
	    //
	    // Muon Endcap Segment
	    // 
	
	    for (std::vector<CSCSegment>::const_iterator iseg = m_segs.begin (), isegEnd = m_segs.end (); iseg != isegEnd; ++iseg) 
	    {
		const GeomDet *det = m_pDD->idToDet ((*iseg).cscDetId ());
		
		// Local pos & dir
                LocalPoint  pos = (*iseg).localPosition ();
                LocalVector dir = (*iseg).localDirection ();

		// Chamber thickness (in z)
                float halfThickness = det->surface ().bounds ().thickness ()/2.;
		
		SoVertexProperty *vtx = new SoVertexProperty;
		
		SoLineSet *line = new SoLineSet;
		line->numVertices = 2;
		
                float z1 = halfThickness;
		float x1 = pos.x() + dir.x()*z1/dir.z();
		float y1 = pos.y() + dir.y()*z1/dir.z();
                GlobalPoint g1 = det->surface().toGlobal( LocalPoint(x1,y1,z1) );

                float z2 = -halfThickness;
		float x2 = pos.x() + dir.x()*z2/dir.z();
		float y2 = pos.y() + dir.y()*z2/dir.z();
                GlobalPoint g2 = det->surface().toGlobal( LocalPoint(x2,y2,z2) );
		
		float x = g1.x () / 100.0;  // cm -> m
		float y = g1.y () / 100.0;  // cm -> m
		float z = g1.z () / 100.0;  // cm -> m
		
		vtx->vertex.set1Value(0,SbVec3f(x,y,z));
		
		x = g2.x () / 100.0;  // cm -> m
		y = g2.y () / 100.0;  // cm -> m
		z = g2.z () / 100.0;  // cm -> m
		
		vtx->vertex.set1Value(1,SbVec3f(x,y,z));
		
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
    }
    
    rep->clear ();
    rep->node ()->addChild (sep);
}

/** Update object property description. */
void
VisCSCSegmentTwig::update (IgRZRep *rep)
{
   // Get debugging dump.
    VisQueuedTwig::update (rep);

    SoSeparator *sep = new SoSeparator;
    SoMaterial *mat = new SoMaterial;
    mat->ambientColor.setValue (0.135, 0.2225, 0.1575);

    mat->diffuseColor.setValue (1.00, 0.28, 0.49); //light red

    mat->specularColor.setValue (0.316228, 0.316228, 0.316228);
    mat->emissiveColor.setValue(0.0, 0.0, 0.0);
    mat->shininess = 0.1;
    sep->addChild (mat);

    SoDrawStyle *sty = new SoDrawStyle;
    sty->style = SoDrawStyle::LINES;
    sty->lineWidth.setValue (6.0);
    sep->addChild (sty);
    
    if (! m_segs.empty () && m_pDD.isValid ())
    {
	try 
	{
	    //
	    // Muon Endcap Segment
	    // 
	
	    for (std::vector<CSCSegment>::const_iterator iseg = m_segs.begin (), isegEnd = m_segs.end (); iseg != isegEnd; ++iseg) 
	    {
		const GeomDet *det = m_pDD->idToDet ((*iseg).cscDetId ());
		
		// Local pos & dir
                LocalPoint  pos = (*iseg).localPosition();
                LocalVector dir = (*iseg).localDirection();

		// Chamber thickness (in z)
                float halfThickness = det->surface ().bounds ().thickness () / 2.;
		
		SoVertexProperty *vtx = new SoVertexProperty;
		
		SoLineSet *line = new SoLineSet;
		line->numVertices = 2;
		
                float z1 = halfThickness;
		float x1 = pos.x() + dir.x()*z1/dir.z();
		float y1 = pos.y() + dir.y()*z1/dir.z();
                GlobalPoint g1 = det->surface().toGlobal( LocalPoint(x1,y1,z1) );

                float z2 = -halfThickness;
		float x2 = pos.x() + dir.x()*z2/dir.z();
		float y2 = pos.y() + dir.y()*z2/dir.z();
                GlobalPoint g2 = det->surface().toGlobal( LocalPoint(x2,y2,z2) );
		
		float x = g1.x () / 100.0;  // cm -> m
		float y = g1.y () / 100.0;  // cm -> m
		float z = g1.z () / 100.0;  // cm -> m
		
		vtx->vertex.set1Value(0,SbVec3f(x,y,z));
		
		x = g2.x () / 100.0;  // cm -> m
		y = g2.y () / 100.0;  // cm -> m
		z = g2.z () / 100.0;  // cm -> m
		
		vtx->vertex.set1Value(1,SbVec3f(x,y,z));
		
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
    }
    
    rep->clear ();
    rep->node ()->addChild (sep);
}

