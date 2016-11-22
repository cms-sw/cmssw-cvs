//<<<<<< INCLUDES                                                       >>>>>>

#include "VisSimulation/VisSimHits/interface/VisSimTrackTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "Iguana/Inventor/interface/IgSoIdealTrack.h"
#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include "Iguana/Inventor/interface/IgParticleChar.h"
#include "Iguana/Studio/interface/IgQtLock.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SoOutput.h>
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
	return new VisSimTrackTwig (state, parent, name, friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisSimTrackTwig::VisSimTrackTwig (IgState *state, IgTwig *parent, const std::string &name /* = ""*/,
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
      m_cut (state, lat::CreateCallback (this, &VisSimTrackTwig::cutChanged)), 
      m_annot (state, lat::CreateCallback (this, &VisSimTrackTwig::cutChanged)),
      m_time (state, lat::CreateCallback (this, &VisSimTrackTwig::cutChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID simTracksID (typeid (edm::SimTrackContainer));
    tfService->registerTwig (simTracksID.friendlyClassName (), &createThisTwig);
}

void
VisSimTrackTwig::cutChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisSimTrackTwig::onNewEvent (const edm::Event &event,
			     const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_tracks.clear ();
    m_vertices.clear ();

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<edm::SimTrackContainer> > trackCollections;
    std::vector<edm::Handle<edm::SimVertexContainer> > vertexCollections;
    
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, trackCollections);
	}
	else
	{
	    event.getManyByType (trackCollections);
	}	    
	event.getManyByType (vertexCollections);
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

    if (! trackCollections.empty ())
    {
	for (std::vector<edm::Handle<edm::SimTrackContainer> >::iterator i = trackCollections.begin (), iEnd = trackCollections.end (); 
	     i != iEnd; ++i) 
	{
	    const edm::SimTrackContainer& c = *(*i);

	    for (std::vector<SimTrack>::const_iterator isimt = c.begin (), isimtEnd = c.end ();
		 isimt != isimtEnd; ++isimt)
	    {
		m_tracks.push_back ((*isimt));
	    }    
	}
    }

    if (! vertexCollections.empty ())
    {
	for (std::vector<edm::Handle<edm::SimVertexContainer> >::iterator i = vertexCollections.begin (), iEnd = vertexCollections.end ();
	     i != iEnd; ++i) 
	{
	    const edm::SimVertexContainer& c = *(*i);

	    for (std::vector<SimVertex>::const_iterator isimv = c.begin (), isimvEnd = c.end ();
		 isimv != isimvEnd; ++isimv)
	    {
		m_vertices.push_back ((*isimv));
	    }    
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisSimTrackTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "Total " << m_tracks.size () << " sim tracks.<br>";
    text << "Above " << m_cut.value () << " GeV: <br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Track ID</TH>"
	 << "<TH>Momentum</TH>"
	 << "<TH>Hep PDT code</TH>"
	 << "<TH>Vertex ID</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if ((! m_tracks.empty ())) 
    {
	int nTracks = 0;
	try 
	{
	    for (std::vector<SimTrack>::const_iterator i = m_tracks.begin (), iEnd = m_tracks.end (); i != iEnd; ++i) 
	    {
		float pt = (*i).momentum ().Pt();
		if (pt > m_cut.value ())
		{
		    text << "<TR align = right>"
			 << "<TD>" << std::setw (3) << nTracks++ << "</TD>"
			 << "<TD>" << std::setw (3) << double ((*i).trackId ()) << "</TD>"
			 << "<TD>" << std::setw (3) << pt << "</TD>"
			 << "<TD>" << std::setw (3) << (*i).type () << "</TD>"
			 << "<TD>" << std::setw (3) << (*i).vertIndex () << "</TD></TR>";
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
    }
    text << "</table>";

    // Send it over.
    IgQtLock ();
    rep->setText (text.str ());
}

void
VisSimTrackTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);      
    IgQtLock ();
    rep->clear ();

    if ((! m_tracks.empty ())) 
    {
	SoSeparator *sep = new SoSeparator; // global
	
	try 
	{
	    for (std::vector<SimTrack>::const_iterator i = m_tracks.begin (), iEnd = m_tracks.end (); i != iEnd; ++i) 
	    {
		float pt = (*i).momentum ().Pt();
		if (pt > m_cut.value ())
		{
		    double x0 = 0.0;
		    double y0 = 0.0;
		    double z0 = 0.0;
		
		    if (! (*i).noVertex ())
		    {
			int vInd = (*i).vertIndex ();
			x0 = (m_vertices.at (vInd)).position ().x () / 100.0;
			y0 = (m_vertices.at (vInd)).position ().y () / 100.0;
			z0 = (m_vertices.at (vInd)).position ().z () / 100.0;
		    }
		
		    IgSoIdealTrack *kit = new IgSoIdealTrack;

		    SoSeparator *labelSep = new SoSeparator;
		    QString label;
		    SoText2  *trackLabel = new SoText2;
		    SoTranslation *trackLabelTranslation = new SoTranslation;

		    SbVec3f pos = SbVec3f ((*i).trackerSurfacePosition ().x () / 100.0,
					   (*i).trackerSurfacePosition ().y () / 100.0,
					   (*i).trackerSurfacePosition ().z () / 100.0);
		    trackLabelTranslation->translation = pos;
		    
		    labelSep->addChild (trackLabelTranslation);
		    labelSep->addChild (trackLabel);
		    
		    switch ((*i).type ())
		    {
		    case 13:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("muon"));
			kit->radius = 25.0;
			label = QString ("mu %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -13:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("antimuon"));
			kit->radius = 25.0;

			label = QString ("mu- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 11:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("electron"));

			label = QString ("e- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -11:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("positron"));

			label = QString ("e+ %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 211:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("antipion"));

			label = QString ("pi+ %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -211:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("pion"));

			label = QString ("pi- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 2212:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("proton"));

			label = QString ("p %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -2212:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("antiproton"));

			label = QString ("pbar %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 130:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kplus"));

			label = QString ("Kl %1 GeV").arg (pt, 0, 'g', 2);
			break;			
		    case 310:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kplus"));

			label = QString ("Ks %1 GeV").arg (pt, 0, 'g', 2);
			break;			
		    case 321:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kplus"));

			label = QString ("K+ %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -321:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kminus"));

			label = QString ("K- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 3312:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("ximinus"));

			label = QString ("Xi- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -3122:
			break;
		    case 2112: //"neutron"
		    case -2112: //"antineutron"
		    case 22: //"gamma"
                        {
                          SoSeparator *gammas = new SoSeparator;
                          SoMaterial *gammaMaterial = new SoMaterial;
                          gammaMaterial->ambientColor.setValue (0.000, 0.000, 0.000); 
                          gammaMaterial->diffuseColor.setValue (1.000, 1.000, 0.000);  
                          gammaMaterial->emissiveColor.setValue (1.000, 1.000, 0.000); 
                          gammaMaterial->specularColor.setValue (0.000, 0.000, 0.000); 
                          gammaMaterial->shininess = 0.0;
                          gammaMaterial->transparency = 0.0;
                          gammas->addChild (gammaMaterial);
                         
                          IgSoSimpleTrajectory *track = new IgSoSimpleTrajectory;
                         
                          track->controlPoints.set1Value (0, SbVec3f (x0, y0, z0));
                          track->markerPoints.set1Value (0, SbVec3f (x0, y0, z0));
                          
                          track->controlPoints.set1Value (1, pos);
                          track->markerPoints.set1Value (1, pos);
                          gammas->addChild (track);
                          
                          sep->addChild (gammas);
                          
                          break;
                        }
		    default:
			break;
		    }		    
		    if (m_annot.value () == true)
		    {
			sep->addChild (labelSep);
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
VisSimTrackTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisSimTrackTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);      
    IgQtLock ();
    rep->clear ();

    if ((! m_tracks.empty ())) 
    {
	SoSeparator *sep = new SoSeparator; // global
	
	try 
	{
	    for (std::vector<SimTrack>::const_iterator i = m_tracks.begin (), iEnd = m_tracks.end (); i != iEnd; ++i) 
	    {
		float pt = (*i).momentum ().Pt();
		if (pt > m_cut.value ())
		{
		    double x0 = 0.0;
		    double y0 = 0.0;
		    double z0 = 0.0;
		
		    if (! (*i).noVertex ())
		    {
			int vInd = (*i).vertIndex ();
			x0 = (m_vertices.at (vInd)).position ().x () / 100.0;
			y0 = (m_vertices.at (vInd)).position ().y () / 100.0;
			z0 = (m_vertices.at (vInd)).position ().z () / 100.0;
		    }
		
		    IgSoIdealTrack *kit = new IgSoIdealTrack;


		    SoSeparator *labelSep = new SoSeparator;
		    QString label;
		    SoText2  *trackLabel = new SoText2;
		    SoTranslation *trackLabelTranslation = new SoTranslation;

		    SbVec3f pos = SbVec3f ((*i).trackerSurfacePosition ().x () / 100.0,
					   (*i).trackerSurfacePosition ().y () / 100.0,
					   (*i).trackerSurfacePosition ().z () / 100.0);
		    trackLabelTranslation->translation = pos;
		    
		    labelSep->addChild (trackLabelTranslation);
		    labelSep->addChild (trackLabel);

		    
		    switch ((*i).type ())
		    {
		    case 13:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("muon"));
			kit->radius = 25.0;
			label = QString ("mu %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -13:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("antimuon"));
			kit->radius = 25.0;

			label = QString ("mu- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 11:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("electron"));

			label = QString ("e- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -11:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("positron"));

			label = QString ("e+ %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 211:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("antipion"));

			label = QString ("pi+ %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -211:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("pion"));

			label = QString ("pi- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 2212:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("proton"));

			label = QString ("p %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -2212:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("antiproton"));

			label = QString ("pbar %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 130:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kplus"));

			label = QString ("Kl %1 GeV").arg (pt, 0, 'g', 2);
			break;			
		    case 310:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kplus"));

			label = QString ("Ks %1 GeV").arg (pt, 0, 'g', 2);
			break;			
		    case 321:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kplus"));

			label = QString ("K+ %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -321:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kminus"));

			label = QString ("K- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 3312:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("ximinus"));

			label = QString ("Xi- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -3122:
			break;
		    case 2112: //"neutron"
		    case -2112: //"antineutron"
		    case 22: //"gamma"
                        {
                          SoSeparator *gammas = new SoSeparator;
                          SoMaterial *gammaMaterial = new SoMaterial;
                          gammaMaterial->ambientColor.setValue (0.000, 0.000, 0.000); 
                          gammaMaterial->diffuseColor.setValue (1.000, 1.000, 0.000);  
                          gammaMaterial->emissiveColor.setValue (1.000, 1.000, 0.000); 
                          gammaMaterial->specularColor.setValue (0.000, 0.000, 0.000); 
                          gammaMaterial->shininess = 0.0;
                          gammaMaterial->transparency = 0.0;
                          gammas->addChild (gammaMaterial);
                         
                          IgSoSimpleTrajectory *track = new IgSoSimpleTrajectory;
                         
                          track->controlPoints.set1Value (0, SbVec3f (x0, y0, z0));
                          track->markerPoints.set1Value (0, SbVec3f (x0, y0, z0));
                          
                          track->controlPoints.set1Value (1, pos);
                          track->markerPoints.set1Value (1, pos);
                          gammas->addChild (track);
                          
                          sep->addChild (gammas);
                          
                          break;
                        }
		    default:
			break;
		    }		    
		    if (m_annot.value () == true)
		    {
			sep->addChild (labelSep);
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
VisSimTrackTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);      
    IgQtLock ();
    rep->clear ();

    if ((! m_tracks.empty ())) 
    {
	SoSeparator *sep = new SoSeparator; // global
	
	try 
	{
	    for (std::vector<SimTrack>::const_iterator i = m_tracks.begin (), iEnd = m_tracks.end (); i != iEnd; ++i) 
	    {
		float pt = (*i).momentum ().Pt();
		if (pt > m_cut.value ())
		{
		    double x0 = 0.0;
		    double y0 = 0.0;
		    double z0 = 0.0;
		
		    if (! (*i).noVertex ())
		    {
			int vInd = (*i).vertIndex ();
			x0 = (m_vertices.at (vInd)).position ().x () / 100.0;
			y0 = (m_vertices.at (vInd)).position ().y () / 100.0;
			z0 = (m_vertices.at (vInd)).position ().z () / 100.0;
		    }
		
		    IgSoIdealTrack *kit = new IgSoIdealTrack;

		    SoSeparator *labelSep = new SoSeparator;
		    QString label;
		    SoText2  *trackLabel = new SoText2;
		    SoTranslation *trackLabelTranslation = new SoTranslation;

		    SbVec3f pos = SbVec3f ((*i).trackerSurfacePosition ().x () / 100.0,
					   (*i).trackerSurfacePosition ().y () / 100.0,
					   (*i).trackerSurfacePosition ().z () / 100.0);
		    trackLabelTranslation->translation = pos;
		    
		    labelSep->addChild (trackLabelTranslation);
		    labelSep->addChild (trackLabel);
		    
		    switch ((*i).type ())
		    {
		    case 13:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("muon"));
			kit->radius = 25.0;
			label = QString ("mu %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -13:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("antimuon"));
			kit->radius = 25.0;

			label = QString ("mu- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 11:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("electron"));

			label = QString ("e- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -11:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("positron"));

			label = QString ("e+ %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 211:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("antipion"));

			label = QString ("pi+ %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -211:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("pion"));

			label = QString ("pi- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 2212:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("proton"));

			label = QString ("p %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -2212:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("antiproton"));

			label = QString ("pbar %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 130:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kplus"));

			label = QString ("Kl %1 GeV").arg (pt, 0, 'g', 2);
			break;			
		    case 310:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kplus"));

			label = QString ("Ks %1 GeV").arg (pt, 0, 'g', 2);
			break;			
		    case 321:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kplus"));

			label = QString ("K+ %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -321:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("kminus"));

			label = QString ("K- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case 3312:
			kit->initialise (x0, y0, z0, 
					 (*i).momentum ().x (), (*i).momentum ().y (), (*i).momentum ().z (), 
					 m_time.value (), IgParticleChar::getByName ("ximinus"));

			label = QString ("Xi- %1 GeV").arg (pt, 0, 'g', 2);
			trackLabel->string = label.latin1 ();

			sep->addChild (kit);
			break;
		    case -3122:
			break;
		    case 2112: //"neutron"
		    case -2112: //"antineutron"
		    case 22: //"gamma"
                        {
                          SoSeparator *gammas = new SoSeparator;
                          SoMaterial *gammaMaterial = new SoMaterial;
                          gammaMaterial->ambientColor.setValue (0.000, 0.000, 0.000); 
                          gammaMaterial->diffuseColor.setValue (1.000, 1.000, 0.000);  
                          gammaMaterial->emissiveColor.setValue (1.000, 1.000, 0.000); 
                          gammaMaterial->specularColor.setValue (0.000, 0.000, 0.000); 
                          gammaMaterial->shininess = 0.0;
                          gammaMaterial->transparency = 0.0;
                          gammas->addChild (gammaMaterial);
                         
                          IgSoSimpleTrajectory *track = new IgSoSimpleTrajectory;
                         
                          track->controlPoints.set1Value (0, SbVec3f (x0, y0, z0));
                          track->markerPoints.set1Value (0, SbVec3f (x0, y0, z0));
                          
                          track->controlPoints.set1Value (1, pos);
                          track->markerPoints.set1Value (1, pos);
                          gammas->addChild (track);
                          
                          sep->addChild (gammas);
                          
                          break;
                        }
		    default:
			break;
		    }		    
		    if (m_annot.value () == true)
		    {
			sep->addChild (labelSep);
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

