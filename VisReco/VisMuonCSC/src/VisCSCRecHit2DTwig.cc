//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuonCSC/interface/VisCSCRecHit2DTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2D.h"
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
#include "Iguana/Inventor/interface/IgSo3DErrorBar.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <classlib/utils/DebugAids.h>
#include <qstring.h>
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
	return new VisCSCRecHit2DTwig (state, parent, "CSC Rec Hits 2D (" + name + ")", 
				       friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCSCRecHit2DTwig::VisCSCRecHit2DTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID digiCollID (typeid (CSCRecHit2DCollection));
    tfService->registerTwig (digiCollID.friendlyClassName (), &createThisTwig);
}

void
VisCSCRecHit2DTwig::onNewEvent (const edm::Event &event,
			       const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_rechits.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<CSCRecHit2DCollection> > hitCollections;
    
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, hitCollections);
	}
	else
	{
	    event.getManyByType (hitCollections);
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

    if (! hitCollections.empty ())
    {
	std::vector<edm::Handle<CSCRecHit2DCollection> >::iterator i;
	std::vector<edm::Handle<CSCRecHit2DCollection> >::iterator iEnd;
	for (i = hitCollections.begin (), iEnd = hitCollections.end (); i != iEnd; ++i) 
	{
	    const CSCRecHit2DCollection& c = *(*i);
    
	    CSCRecHit2DCollection::const_iterator hitIt;
	    CSCRecHit2DCollection::const_iterator hitItEnd;
	    for (hitIt = c.begin (), hitItEnd = c.end (); hitIt != hitItEnd; ++hitIt)
	    {
		m_rechits.push_back (*hitIt);
	    }
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

/** Update object property description. */
void
VisCSCRecHit2DTwig::update (IgTextRep *rep) 
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare the property description.
    std::ostringstream text;
    text << "Muon Endcap: CSC Rec Hits 2D<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>position</TH>"
	 << "<TH>Error Within Strip</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right,std::ios::adjustfield);

    if (! m_rechits.empty () && m_pDD.isValid ())
    {
	try 
	{
	    int nRecHits = 0;
	
	    for (std::vector<CSCRecHit2D>::const_iterator ihit = m_rechits.begin (), ihitEnd = m_rechits.end (); ihit != ihitEnd; ++ihit) 
	    {
		const GeomDetUnit *det = m_pDD->idToDetUnit ((*ihit).cscDetId ());
		
		GlobalPoint pos = det->surface ().toGlobal ((*ihit).localPosition ());
	    
		text << "<TR align = right>"
		     << "<TD>" << std::setw (2) << nRecHits++ << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (2) << pos << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (2) << (*ihit).errorWithinStrip () << "</TD>"
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
    }
    text << "</table>";
    
    // Send it over.
    rep->setText (text.str ());
}

/** Update object property description. */
void
VisCSCRecHit2DTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    SoSeparator *sep = new SoSeparator;
    SoMaterial *mat = new SoMaterial;
    mat->ambientColor.setValue (0.135, 0.2225, 0.1575);
    mat->diffuseColor.setValue (0.54, 0.89, 0.63); // pale green
    mat->specularColor.setValue (0.316228, 0.316228, 0.316228);
    mat->emissiveColor.setValue(0.0, 0.0, 0.0);
    mat->shininess = 0.1;
    sep->addChild (mat);
    
    if (! m_rechits.empty () && m_pDD.isValid ())
    {
	try 
	{
	    //
	    // Muon Endcap Rechits
	    // 
	
	    for (std::vector<CSCRecHit2D>::const_iterator is = m_rechits.begin (), isEnd = m_rechits.end (); is != isEnd; ++is) 
	    {
		const GeomDetUnit *det = m_pDD->idToDetUnit ((*is).cscDetId ());
		
// 		GlobalPoint pos = det->surface ().toGlobal ((*is).localPosition ());
	    
// 		float x = pos.x () / 100.0;  // cm -> m
// 		float y = pos.y () / 100.0;  // cm -> m
// 		float z = pos.z () / 100.0;  // cm -> m
		
		LocalPoint xyzLocal = is->localPosition ();
		float x = xyzLocal.x ();
                float y = xyzLocal.y ();
		float z = xyzLocal.z ();
		float dx = std::sqrt( is->localPositionError ().xx () );
		float dy = std::sqrt( is->localPositionError ().yy () );
		
		IgSo3DErrorBar *errorBar = new IgSo3DErrorBar;
		GlobalPoint gp;
		gp = det->surface ().toGlobal (LocalPoint (x - dx, y, z));
		errorBar->u1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x + dx, y, z));
		errorBar->u2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, y - dy, z));
		errorBar->v1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, y + dy, z));
		errorBar->v2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (xyzLocal); // no error in z
		errorBar->w1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		errorBar->w2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);

		errorBar->lineWidth.setValue (1.0);
		
		sep->addChild (errorBar);
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
VisCSCRecHit2DTwig::update (IgLegoRep *rep)
{}

/** Update object property description. */
void
VisCSCRecHit2DTwig::update (IgRPhiRep *rep)
{

    // Get debugging dump.
    VisQueuedTwig::update (rep);

    SoSeparator *sep = new SoSeparator;
    SoMaterial *mat = new SoMaterial;
    mat->ambientColor.setValue (0.135, 0.2225, 0.1575);
    mat->diffuseColor.setValue (0.54, 0.89, 0.63);
    mat->specularColor.setValue (0.316228, 0.316228, 0.316228);
    mat->emissiveColor.setValue(0.0, 0.0, 0.0);
    mat->shininess = 0.1;
    sep->addChild (mat);
    
    if (! m_rechits.empty () && m_pDD.isValid ())
    {
	try 
	{
	    //
	    // Muon Endcap Rechits
	    // 
	
	    for (std::vector<CSCRecHit2D>::const_iterator is = m_rechits.begin (), isEnd = m_rechits.end (); is != isEnd; ++is) 
	    {
		const GeomDetUnit *det = m_pDD->idToDetUnit ((*is).cscDetId ());
		
// 		GlobalPoint pos = det->surface ().toGlobal ((*is).localPosition ());
	    
// 		float x = pos.x () / 100.0;  // cm -> m
// 		float y = pos.y () / 100.0;  // cm -> m
// 		float z = pos.z () / 100.0;  // cm -> m
		
		LocalPoint xyzLocal = is->localPosition ();
		float x = xyzLocal.x ();
                float y = xyzLocal.y ();
		float z = xyzLocal.z ();
		float dx = std::sqrt( is->localPositionError ().xx () );
		float dy = std::sqrt( is->localPositionError ().yy () );
		
		IgSo3DErrorBar *errorBar = new IgSo3DErrorBar;
		GlobalPoint gp;
		gp = det->surface ().toGlobal (LocalPoint (x - dx, y, z));
		errorBar->u1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x + dx, y, z));
		errorBar->u2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, y - dy, z));
		errorBar->v1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, y + dy, z));
		errorBar->v2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (xyzLocal); // no error in z
		errorBar->w1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		errorBar->w2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);

		errorBar->lineWidth.setValue (1.0);
		
		sep->addChild (errorBar);
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
VisCSCRecHit2DTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    SoSeparator *sep = new SoSeparator;
    SoMaterial *mat = new SoMaterial;
    mat->ambientColor.setValue (0.135, 0.2225, 0.1575);
    mat->diffuseColor.setValue (0.54, 0.89, 0.63);
    mat->specularColor.setValue (0.316228, 0.316228, 0.316228);
    mat->emissiveColor.setValue(0.0, 0.0, 0.0);
    mat->shininess = 0.1;
    sep->addChild (mat);
    
    if (! m_rechits.empty () && m_pDD.isValid ())
    {
	try 
	{
	    //
	    // Muon Endcap Rechits
	    // 
	
	    for (std::vector<CSCRecHit2D>::const_iterator is = m_rechits.begin (), isEnd = m_rechits.end (); is != isEnd; ++is) 
	    {
		const GeomDetUnit *det = m_pDD->idToDetUnit ((*is).cscDetId ());
		
// 		GlobalPoint pos = det->surface ().toGlobal ((*is).localPosition ());
	    
// 		float x = pos.x () / 100.0;  // cm -> m
// 		float y = pos.y () / 100.0;  // cm -> m
// 		float z = pos.z () / 100.0;  // cm -> m
		
		LocalPoint xyzLocal = is->localPosition ();
		float x = xyzLocal.x ();
                float y = xyzLocal.y ();
		float z = xyzLocal.z ();
		float dx = std::sqrt( is->localPositionError ().xx () );
		float dy = std::sqrt( is->localPositionError ().yy () );
		
		IgSo3DErrorBar *errorBar = new IgSo3DErrorBar;
		GlobalPoint gp;
		gp = det->surface ().toGlobal (LocalPoint (x - dx, y, z));
		errorBar->u1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x + dx, y, z));
		errorBar->u2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, y - dy, z));
		errorBar->v1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, y + dy, z));
		errorBar->v2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (xyzLocal); // no error in z
		errorBar->w1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		errorBar->w2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);

		errorBar->lineWidth.setValue (1.0);
		
		sep->addChild (errorBar);
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

