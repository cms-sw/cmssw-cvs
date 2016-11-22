//<<<<<< INCLUDES                                                       >>>>>>

#include "VisSimulation/VisSimHits/interface/VisPSimHitTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/Framework/interface/Event.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgQtLock.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DBaseModel.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <qstring.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>

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
	IgTwig *rootTwig = IgDocumentData::get (state)->root ();
	IgTwig *eventTwig = 0;
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Simulated Data/Sim Hits");
	
	if (! eventTwig) 
	    eventTwig = parent;
		
	return new VisPSimHitTwig (state, eventTwig, "[N/A] PSimHit (" + name + ")", friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisPSimHitTwig::VisPSimHitTwig (IgState *state, IgTwig *parent, const std::string &name /* = ""*/,
				const std::string &friendlyName /* = ""*/,
				const std::string &moduleLabel /* = ""*/,
				const std::string &instanceName /* = ""*/,
				const std::string &processName /* = "" */)
    : VisQueuedTwig (state, parent, name),
      m_text (name),
      m_friendlyName (friendlyName),
      m_moduleLabel (moduleLabel),
      m_instanceName (instanceName),
      m_processName (processName),
      m_connect (state, lat::CreateCallback (this, &VisPSimHitTwig::twigChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID caloJetsID (typeid (edm::PSimHitContainer));
    tfService->registerTwig (caloJetsID.friendlyClassName (), &createThisTwig);
}

void
VisPSimHitTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisPSimHitTwig::onNewEvent (const edm::Event &event,
			    const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_hits.clear ();
    m_text = (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ())
	      .arg (event.luminosityBlock ())
	      .arg (event.orbitNumber ())
	      .arg (event.bunchCrossing ())
	      .latin1 ());

    std::vector<edm::Handle<edm::PSimHitContainer> > hitCollections;
    
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, hitCollections);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisPSimHitTwig::onNewEvent ");
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

    if (! hitCollections.empty ())
    {
	for (std::vector<edm::Handle<edm::PSimHitContainer> >::iterator i = hitCollections.begin (), iEnd = hitCollections.end (); i != iEnd; ++i) 
	{
	    const edm::PSimHitContainer& c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    
	    this->name (nameStr.toStdString ());

	    for (std::vector<PSimHit>::const_iterator isim = c.begin (), isimEnd = c.end ();
		 isim != isimEnd; ++isim)
	    {
		m_hits.push_back ((*isim));
	    }    
	}
    }

    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisPSimHitTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "Total " << m_hits.size () << " hits from ";
    text << m_text << "<br>";

    text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Position</TH>"
	 << "<TH>Energy loss</TH>"
	 << "<TH>DetID</TH>"
	 << "<TH>Track ID</TH>"
	 << "<TH>Time of flight</TH>"
	 << "<TH>Particle type</TH>"
	 << "<TH>Process type</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if ((! m_hits.empty ())) 
    {
	int nHits = 0;
	try 
	{
	    for (std::vector<PSimHit>::const_iterator i = m_hits.begin (); i != m_hits.end (); ++i) 
	    {
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nHits++ << "</TD>"
		     << "<TD>" << (*i).localPosition ().x () << ", " 
		               << (*i).localPosition ().y () << ", " 
		     << (*i).localPosition ().z () << "; Phi "<< (*i).localPosition ().phi ()  << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).energyLoss () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).detUnitId () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).trackId () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).timeOfFlight () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).particleType () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).processType () << "</TD></TR>";
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

    // Send it over.
    IgQtLock ();
    rep->setText (text.str ());
}

void
VisPSimHitTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);      
    IgQtLock ();
    rep->clear ();

    if (! m_hits.empty ())
    {
	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (0.0, 0.0, 1.0);
	mat->emissiveColor.setValue(0.0, 0.0, 1.0);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 6.0;

	SoSeparator *sep = new SoSeparator; // global
	sep->addChild (mat);
	sep->addChild (drawStyle);

	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	try 
	{
	    typedef std::map<int, std::vector <SbVec3f> > VisParticleMap;
	    VisParticleMap pmap;

	    for (std::vector<PSimHit>::const_iterator i = m_hits.begin (); i != m_hits.end (); ++i) 
	    {
		int ptype = (*i).trackId ();
		
		VisParticleMap::iterator mit = pmap.find (ptype);
		
		if (mit == pmap.end ())
		{
		    std::vector<SbVec3f> vec;
		    bool done = pmap.insert (VisParticleMap::value_type (ptype, vec)).second;
		    if (done)
		    {
			mit = pmap.find (ptype);
		    }
		}

		if (mit == pmap.end ())
		{
		    throw std::runtime_error ("Cannot find particle type in the map!");
		}		
		DetId id = (*i).detUnitId ();
		if (! id.null () && (id.det () == DetId::Tracker || id.det () == DetId::Muon))
		{
		    GlobalPoint pos = esService->toGlobal ((*i).localPosition (), id);
		    (mit->second).push_back (SbVec3f (pos.x () / 100.0, pos.y () / 100.0, pos.z () / 100.0));
		}
	    }	    
	    if (! pmap.empty ())
	    {
		for (VisParticleMap::const_iterator mit = pmap.begin (); mit != pmap.end (); ++mit)
		{
		    SoSeparator *trackSep = new SoSeparator; 
		    
		    QString label = QString ("ID = %1")
				    .arg (mit->first);             

		    trackSep->setName (Ig3DBaseModel::encode (label.prepend ("Track with ").toStdString ()));

		    SoVertexProperty *vertices = new SoVertexProperty;
		    int nVrtx = 0;

		    for (std::vector<SbVec3f>::const_iterator vit = (mit->second).begin (); vit != (mit->second).end (); ++vit) 
		    {	
			vertices->vertex.set1Value (nVrtx++, (*vit));
		    }
		    
		    vertices->vertex.setNum (nVrtx);
		    SoPointSet *points = new SoPointSet;
		    points->vertexProperty.setValue (vertices);
		    points->numPoints.setValue (nVrtx);
		    trackSep->addChild (points);

		    if (m_connect.value ())
		    {			
			SoLineSet *track = new SoLineSet;
			track->vertexProperty.setValue (vertices);
			track->numVertices.setValue (nVrtx);
			trackSep->addChild (track);
		    }
		    
		    sep->addChild (trackSep);		    
		}
	    }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisPSimHitTwig::update(Ig3DRep*) ");
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
VisPSimHitTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisPSimHitTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    if (! m_hits.empty ())
    {
	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (0.0, 0.0, 1.0);
	mat->emissiveColor.setValue(0.0, 0.0, 1.0);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 6.0;

	SoSeparator *sep = new SoSeparator; // global
	sep->addChild (mat);
	sep->addChild (drawStyle);

	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	try 
	{
	    typedef std::map<int, std::vector <SbVec3f> > VisParticleMap;
	    VisParticleMap pmap;

	    for (std::vector<PSimHit>::const_iterator i = m_hits.begin (); i != m_hits.end (); ++i) 
	    {
		int ptype = (*i).trackId ();
		
		VisParticleMap::iterator mit = pmap.find (ptype);
		
		if (mit == pmap.end ())
		{
		    std::vector<SbVec3f> vec;
		    bool done = pmap.insert (VisParticleMap::value_type (ptype, vec)).second;
		    if (done)
		    {
			mit = pmap.find (ptype);
		    }
		}

		if (mit == pmap.end ())
		{
		    throw std::runtime_error ("Cannot find particle type in the map!");
		}		
		DetId id = (*i).detUnitId ();
		if (! id.null () && (id.det () == DetId::Tracker || id.det () == DetId::Muon))
		{
		    GlobalPoint pos = esService->toGlobal ((*i).localPosition (), id);
		    (mit->second).push_back (SbVec3f (pos.x () / 100.0, pos.y () / 100.0, pos.z () / 100.0));
		}
	    }	    
	    if (! pmap.empty ())
	    {
		for (VisParticleMap::const_iterator mit = pmap.begin (); mit != pmap.end (); ++mit)
		{
		    SoSeparator *trackSep = new SoSeparator; 
		    
		    QString label = QString ("ID = %1")
				    .arg (mit->first);             

		    trackSep->setName (Ig3DBaseModel::encode (label.prepend ("Track with ").toStdString ()));

		    SoVertexProperty *vertices = new SoVertexProperty;
		    int nVrtx = 0;

		    for (std::vector<SbVec3f>::const_iterator vit = (mit->second).begin (); vit != (mit->second).end (); ++vit) 
		    {	
			vertices->vertex.set1Value (nVrtx++, (*vit));
		    }
		    
		    vertices->vertex.setNum (nVrtx);
		    SoPointSet *points = new SoPointSet;
		    points->vertexProperty.setValue (vertices);
		    points->numPoints.setValue (nVrtx);
		    trackSep->addChild (points);

		    if (m_connect.value ()) 
		    {
			SoLineSet *track = new SoLineSet;
			track->vertexProperty.setValue (vertices);
			track->numVertices.setValue (nVrtx);
			trackSep->addChild (track);
		    }
		    
		    sep->addChild (trackSep);		    
		}
	    }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisPSimHitTwig::update(IgRPhiRep*) ");
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
VisPSimHitTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    if (! m_hits.empty ())
    {
	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (0.0, 0.0, 1.0);
	mat->emissiveColor.setValue(0.0, 0.0, 1.0);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 6.0;

	SoSeparator *sep = new SoSeparator; // global
	sep->addChild (mat);
	sep->addChild (drawStyle);

	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	try 
	{
	    typedef std::map<int, std::vector <SbVec3f> > VisParticleMap;
	    VisParticleMap pmap;

	    for (std::vector<PSimHit>::const_iterator i = m_hits.begin (); i != m_hits.end (); ++i) 
	    {
		int ptype = (*i).trackId ();
		
		VisParticleMap::iterator mit = pmap.find (ptype);
		
		if (mit == pmap.end ())
		{
		    std::vector<SbVec3f> vec;
		    bool done = pmap.insert (VisParticleMap::value_type (ptype, vec)).second;
		    if (done)
		    {
			mit = pmap.find (ptype);
		    }
		}

		if (mit == pmap.end ())
		{
		    throw std::runtime_error ("Cannot find particle type in the map!");
		}		
		DetId id = (*i).detUnitId ();
		if (! id.null () && (id.det () == DetId::Tracker || id.det () == DetId::Muon))
		{
		    GlobalPoint pos = esService->toGlobal ((*i).localPosition (), id);
		    (mit->second).push_back (SbVec3f (pos.x () / 100.0, pos.y () / 100.0, pos.z () / 100.0));
		}
	    }	    
	    if (! pmap.empty ())
	    {
		for (VisParticleMap::const_iterator mit = pmap.begin (); mit != pmap.end (); ++mit)
		{
		    SoSeparator *trackSep = new SoSeparator; 
		    
		    QString label = QString ("ID = %1")
				    .arg (mit->first);             

		    trackSep->setName (Ig3DBaseModel::encode (label.prepend ("Track with ").toStdString ()));

		    SoVertexProperty *vertices = new SoVertexProperty;
		    int nVrtx = 0;

		    for (std::vector<SbVec3f>::const_iterator vit = (mit->second).begin (); vit != (mit->second).end (); ++vit) 
		    {	
			vertices->vertex.set1Value (nVrtx++, (*vit));
		    }
		    
		    vertices->vertex.setNum (nVrtx);
		    SoPointSet *points = new SoPointSet;
		    points->vertexProperty.setValue (vertices);
		    points->numPoints.setValue (nVrtx);
		    trackSep->addChild (points);

		    if (m_connect.value ()) 
		    {
			SoLineSet *track = new SoLineSet;
			track->vertexProperty.setValue (vertices);
			track->numVertices.setValue (nVrtx);
			trackSep->addChild (track);
		    }
		    
		    sep->addChild (trackSep);		    
		}
	    }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisPSimHitTwig::update(IgRZRep*) ");
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
