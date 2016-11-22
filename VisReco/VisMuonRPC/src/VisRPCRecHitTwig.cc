//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuonRPC/interface/VisRPCRecHitTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHit.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/RPCGeometry/interface/RPCRoll.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSo3DErrorBar.h"
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <classlib/utils/DebugAids.h>
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
	return new VisRPCRecHitTwig (state, parent, "RPC Rec Hits (" + name + ")", 
				     friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisRPCRecHitTwig::VisRPCRecHitTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID rechitCollID (typeid (RPCRecHitCollection));
    tfService->registerTwig (rechitCollID.friendlyClassName (), &createThisTwig);
}

void
VisRPCRecHitTwig::onNewEvent (const edm::Event &event,
			      const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_rechits.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<RPCRecHitCollection> > collections;
    
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, collections);
	}
	else
	{
	    event.getManyByType (collections);
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

    if (! collections.empty ())
    {
	std::vector<edm::Handle<RPCRecHitCollection> >::iterator i;
	std::vector<edm::Handle<RPCRecHitCollection> >::iterator iEnd;
	for (i = collections.begin (), iEnd = collections.end (); i != iEnd; ++i) 
	{
	    const RPCRecHitCollection& c = *(*i);
    
	    RPCRecHitCollection::const_iterator segIt;
	    RPCRecHitCollection::const_iterator segItEnd;
	    for (segIt = c.begin (), segItEnd = c.end (); segIt != segItEnd; ++segIt)
	    {
		m_rechits.push_back (*segIt);
	    }
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

/** Update object property description. */
void
VisRPCRecHitTwig::update (IgTextRep *rep) 
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare the property description.
    std::ostringstream text;
    text << "Muon Endcap: RPC Rec Hits <br>";

    if (! m_rechits.empty () && m_pDD.isValid ())
    {
	try 
	{
	    text << "<table width='100%' border=1>"
		 << "<TR align = center>"
		 << "<TH>Number</TH>"
		 << "<TH>position</TH>"
		 << "<TH>chi2</TH>"
		 << "</TR>";
	    text << setiosflags (std::ios::showpoint | std::ios::fixed);
	    text.setf (std::ios::right,std::ios::adjustfield);
	
	    int nRecHits = 0;
	
	    for (std::vector<RPCRecHit>::const_iterator iseg = m_rechits.begin (), isegEnd = m_rechits.end (); iseg != isegEnd; ++iseg) 
	    {
		const GeomDetUnit *det = m_pDD->idToDetUnit ((*iseg).rpcId ());
		
		GlobalPoint pos = det->surface ().toGlobal ((*iseg).localPosition ());
	    
		text << "<TR align = right>"
		     << "<TD>" << std::setw (2) << nRecHits++ << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (2) << pos << "</TD>"
		  //		     << "<TD>" << std::setw (6) << std::setprecision (2) << (*iseg).chi2 () << "</TD>"
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
VisRPCRecHitTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    SoSeparator *sep = new SoSeparator;

    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0xffff0000, rgbcomponents);
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);

//     SoMaterial *mat = new SoMaterial;
//     mat->ambientColor.setValue (0.135, 0.2225, 0.1575);
//     mat->diffuseColor.setValue (0.54, 0.89, 0.63);
//     mat->specularColor.setValue (0.316228, 0.316228, 0.316228);
//     mat->emissiveColor.setValue(0.0, 0.0, 0.0);
//     mat->shininess = 0.1;
//     sep->addChild (mat);
    
    if (! m_rechits.empty () && m_pDD.isValid ())
    {
	try 
	{
	    //
	    // Muon Endcap Segment
	    // 
	
	    for (std::vector<RPCRecHit>::const_iterator is = m_rechits.begin (), isEnd = m_rechits.end (); is != isEnd; ++is) 
	    {
		const GeomDetUnit *det = m_pDD->idToDetUnit ((*is).rpcId ());

		// warning:  error bars aren't right for tilted wires
		LocalPoint xyzLocal = is->localPosition ();
 		float x = xyzLocal.x ();
		float y = xyzLocal.y ();
 		float z = xyzLocal.z ();
 		float dx = is->localPositionError ().xx ();
		// 		float dy = lenght/2;
  		float dy = sqrt(is->localPositionError ().yy ());
		
		IgSo3DErrorBar *errorBar = new IgSo3DErrorBar;
		GlobalPoint gp;
		/*		gp = det->surface ().toGlobal (LocalPoint (-x, -y, 0));
		errorBar->u1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, -y, 0));
		errorBar->u2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, y, 0));
		errorBar->v1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (-x, y, 0));
		errorBar->v2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		gp = det->surface ().toGlobal (xyzLocal); // no error in z
 		errorBar->w1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		errorBar->w2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		*/

 		gp = det->surface ().toGlobal (LocalPoint ((x - dx), y, z));
 		errorBar->u1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		gp = det->surface ().toGlobal (LocalPoint ((x + dx), y, z));
		errorBar->u2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		gp = det->surface ().toGlobal (LocalPoint (x, (y - dy), z));
 		errorBar->v1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		gp = det->surface ().toGlobal (LocalPoint (x, (y + dy), z));
 		errorBar->v2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		gp = det->surface ().toGlobal (xyzLocal); // no error in z
 		errorBar->w1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		errorBar->w2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);

		errorBar->lineWidth.setValue (2.0);
		
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
VisRPCRecHitTwig::update (IgLegoRep *rep)
{}

/** Update object property description. */
void
VisRPCRecHitTwig::update (IgRPhiRep *rep)
{
   // Get debugging dump.
    VisQueuedTwig::update (rep);

    SoSeparator *sep = new SoSeparator;

    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0xffff0000, rgbcomponents);
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);

//     SoAnnotation *ann = new SoAnnotation;
//     SoMaterial *mat = new SoMaterial;
//     mat->ambientColor.setValue (0.135, 0.2225, 0.1575);
//     mat->diffuseColor.setValue (0.54, 0.89, 0.63);
//     mat->specularColor.setValue (0.316228, 0.316228, 0.316228);
//     mat->emissiveColor.setValue(0.0, 0.0, 0.0);
//     mat->shininess = 0.1;
//     ann->addChild (mat);

//     SoDrawStyle *drawStyle = new SoDrawStyle;
//     drawStyle->lineWidth.setValue (8.0);
//     sep->addChild (drawStyle);
//     SoMaterial *dmat = new SoMaterial;
//     dmat->diffuseColor.setValue (0., 0., 0.);
//     sep->addChild (dmat);
//     sep->addChild (ann);

    if (! m_rechits.empty () && m_pDD.isValid ())
    {
	try 
	{
	    //
	    // Muon Endcap Segment
	    // 
	
	    for (std::vector<RPCRecHit>::const_iterator is = m_rechits.begin (), isEnd = m_rechits.end (); is != isEnd; ++is) 
	    {
		const GeomDetUnit *det = m_pDD->idToDetUnit ((*is).rpcId ());

		// warning:  error bars aren't right for tilted wires
		LocalPoint xyzLocal = is->localPosition ();
 		float x = xyzLocal.x ();
		float y = xyzLocal.y ();
 		float z = xyzLocal.z ();
 		float dx = is->localPositionError ().xx ();
		// 		float dy = lenght/2;
  		float dy = sqrt(is->localPositionError ().yy ());
		
		//IgSo3DErrorBar *annErrorBar = new IgSo3DErrorBar;
		IgSo3DErrorBar *errorBar = new IgSo3DErrorBar;
		GlobalPoint gp;
		/*		gp = det->surface ().toGlobal (LocalPoint (-x, -y, 0));
		errorBar->u1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, -y, 0));
		errorBar->u2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, y, 0));
		errorBar->v1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (-x, y, 0));
		errorBar->v2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		gp = det->surface ().toGlobal (xyzLocal); // no error in z
 		errorBar->w1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		errorBar->w2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		*/

 		gp = det->surface ().toGlobal (LocalPoint ((x - dx), y, z));
 		//annErrorBar->u1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		errorBar->u1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		gp = det->surface ().toGlobal (LocalPoint ((x + dx), y, z));
		//annErrorBar->u2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		errorBar->u2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		gp = det->surface ().toGlobal (LocalPoint (x, (y - dy), z));
 		//annErrorBar->v1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		errorBar->v1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		gp = det->surface ().toGlobal (LocalPoint (x, (y + dy), z));
 		//annErrorBar->v2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		errorBar->v2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		gp = det->surface ().toGlobal (xyzLocal); // no error in z
 		//annErrorBar->w1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		errorBar->w1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		//annErrorBar->w2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		errorBar->w2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);

		//annErrorBar->lineWidth.setValue (2.0);
		errorBar->lineWidth.setValue (2.0);
		
		//ann->addChild (annErrorBar);
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
VisRPCRecHitTwig::update (IgRZRep *rep)
{
   // Get debugging dump.
    VisQueuedTwig::update (rep);

    SoSeparator *sep = new SoSeparator;

    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0xffff0000, rgbcomponents);
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);
//     SoAnnotation *ann = new SoAnnotation;
//     SoMaterial *mat = new SoMaterial;
//     mat->ambientColor.setValue (0.135, 0.2225, 0.1575);
//     mat->diffuseColor.setValue (0.54, 0.89, 0.63);
//     mat->specularColor.setValue (0.316228, 0.316228, 0.316228);
//     mat->emissiveColor.setValue(0.0, 0.0, 0.0);
//     mat->shininess = 0.1;
//     ann->addChild (mat);

//     SoDrawStyle *drawStyle = new SoDrawStyle;
//     drawStyle->lineWidth.setValue (6.0);
//     sep->addChild (drawStyle);

//     SoMaterial *dmat = new SoMaterial;
//     dmat->diffuseColor.setValue (0., 0., 0.);
//     sep->addChild (dmat);
//     sep->addChild (ann);
    
    if (! m_rechits.empty () && m_pDD.isValid ())
    {
	try 
	{
	    //
	    // Muon Endcap Segment
	    // 
	
	    for (std::vector<RPCRecHit>::const_iterator is = m_rechits.begin (), isEnd = m_rechits.end (); is != isEnd; ++is) 
	    {
		const GeomDetUnit *det = m_pDD->idToDetUnit ((*is).rpcId ());

		// warning:  error bars aren't right for tilted wires
		LocalPoint xyzLocal = is->localPosition ();
 		float x = xyzLocal.x ();
		float y = xyzLocal.y ();
 		float z = xyzLocal.z ();
 		float dx = is->localPositionError ().xx ();
		// 		float dy = lenght/2;
  		float dy = sqrt(is->localPositionError ().yy ());
		
		//IgSo3DErrorBar *annErrorBar = new IgSo3DErrorBar;
		IgSo3DErrorBar *errorBar = new IgSo3DErrorBar;
		GlobalPoint gp;
		/*		gp = det->surface ().toGlobal (LocalPoint (-x, -y, 0));
		errorBar->u1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, -y, 0));
		errorBar->u2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (x, y, 0));
		errorBar->v1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		gp = det->surface ().toGlobal (LocalPoint (-x, y, 0));
		errorBar->v2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		gp = det->surface ().toGlobal (xyzLocal); // no error in z
 		errorBar->w1.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
 		errorBar->w2.setValue (gp.x () / 100.0, gp.y () / 100.0, gp.z () / 100.0);
		*/
		float posx = -4.0;

 		gp = det->surface ().toGlobal (LocalPoint ((x - dx), y, z));
		float posy = sqrt (gp.x () * gp.x () + gp.y () * gp.y ()) / 100.0; 
		if (gp.y () < 0.) posy = -posy;
		float posz = gp.z () / 100.0;  // cm -> m
 		errorBar->u1.setValue (posx, posy, posz); 

 		gp = det->surface ().toGlobal (LocalPoint ((x + dx), y, z));
		posy = sqrt (gp.x () * gp.x () + gp.y () * gp.y ()) / 100.0; 
		if (gp.y () < 0.) posy = -posy;
		posz = gp.z () / 100.0;  // cm -> m
		errorBar->u2.setValue (posx, posy, posz);

 		gp = det->surface ().toGlobal (LocalPoint (x, (y - dy), z));
		posy = sqrt (gp.x () * gp.x () + gp.y () * gp.y ()) / 100.0; 
		if (gp.y () < 0.) posy = -posy;
		posz = gp.z () / 100.0;  // cm -> m
 		errorBar->v1.setValue (posx, posy, posz);

 		gp = det->surface ().toGlobal (LocalPoint (x, (y + dy), z));
		posy = sqrt (gp.x () * gp.x () + gp.y () * gp.y ()) / 100.0; 
		if (gp.y () < 0.) posy = -posy;
		posz = gp.z () / 100.0;  // cm -> m
 		errorBar->v2.setValue (posx, posy, posz);

 		gp = det->surface ().toGlobal (xyzLocal); // no error in z
		posy = sqrt (gp.x () * gp.x () + gp.y () * gp.y ()) / 100.0; 
		if (gp.y () < 0.) posy = -posy;
		posz = gp.z () / 100.0;  // cm -> m
 		errorBar->w1.setValue (posx, posy, posz);
		errorBar->w2.setValue (posx, posy, posz);

		errorBar->lineWidth.setValue (2.0);
		
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

