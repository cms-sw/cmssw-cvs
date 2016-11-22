//<<<<<< INCLUDES        
#include "VisReco/VisTracker/interface/VisTkIdealHelixTracksTwig.h"                                     
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"

#include "Iguana/Inventor/interface/IgSoIdealTrack.h"
#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include "Iguana/Inventor/interface/IgParticleChar.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <classlib/utils/Error.h>
#include <sstream>
#include <iomanip>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

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
	return new VisTkIdealHelixTracksTwig (state, parent, "Tracker reconstructed tracks (" + name + ")", 
					      friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisTkIdealHelixTracksTwig::VisTkIdealHelixTracksTwig  (IgState *state, IgTwig *parent,
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
      m_processName (processName),
      m_animate (state, lat::CreateCallback (this, &VisTkIdealHelixTracksTwig::twigChanged)),
      m_counterMax (state, lat::CreateCallback (this, &VisTkIdealHelixTracksTwig::twigChanged)),
      m_counterMin (state, lat::CreateCallback (this, &VisTkIdealHelixTracksTwig::twigChanged)),
      m_frequency (state, lat::CreateCallback (this, &VisTkIdealHelixTracksTwig::twigChanged)),
      m_BField (state, lat::CreateCallback (this, &VisTkIdealHelixTracksTwig::twigChanged)),
      m_RMax (state, lat::CreateCallback (this, &VisTkIdealHelixTracksTwig::twigChanged)),
      m_ZMax (state, lat::CreateCallback (this, &VisTkIdealHelixTracksTwig::twigChanged)),
      m_t0 (state, lat::CreateCallback (this, &VisTkIdealHelixTracksTwig::twigChanged)),
      m_dt (state, lat::CreateCallback (this, &VisTkIdealHelixTracksTwig::twigChanged)),
      m_annot (state, lat::CreateCallback (this, &VisTkIdealHelixTracksTwig::twigChanged)),
      m_showDets (state, lat::CreateCallback (this, &VisTkIdealHelixTracksTwig::twigChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID trackID (typeid (reco::TrackCollection));
    tfService->registerTwig (trackID.friendlyClassName (), &createThisTwig);
    pset = VisConfigurationService::pSet ();
    if (! pset)
    {
	edm::ParameterSet p;
	VisConfigurationService *visService = new VisConfigurationService (p);
	pset = visService->pSet ();
	delete visService;
    } 
    trackThickness = pset->getUntrackedParameter<double> ("trackLineThickness", 2.0);

    m_flytime = pset->getUntrackedParameter<double> ("flyTime", 0.5);

    std::vector<std::string> notSpecified;
    m_trackCollectionNames = pset->getUntrackedParameter<std::vector<std::string> > ("trackCollectionNames", notSpecified);
}

void
VisTkIdealHelixTracksTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisTkIdealHelixTracksTwig::onNewEvent (const edm::Event &event,
				       const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_tracks.clear ();

    m_trajectoryPositions.clear ();
    m_trajectoryDirections.clear ();

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<reco::TrackCollection> > trackCollections;
  
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, trackCollections);
	}
	else
	{
            if (m_trackCollectionNames.empty ())
	        event.getManyByType (trackCollections);
            else 
	    {
                for(std::vector<std::string>::iterator tkColl = m_trackCollectionNames.begin (), tkCollEnd = m_trackCollectionNames.end ();
		    tkColl != tkCollEnd; ++tkColl) 
		{
		    LOG (2, trace, LFfwvis, "Getting track Collection with label " << (*tkColl) << "\n");

		    edm::Handle<reco::TrackCollection> tcHandle;

		    event.getByLabel ((*tkColl), tcHandle);
		    trackCollections.push_back (tcHandle);
                }
            }
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTkIdealHelixTracksTwig: ");
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
  
    if (! trackCollections.empty ())
    {
	std::vector<edm::Handle<reco::TrackCollection> >::iterator i;
	std::vector<edm::Handle<reco::TrackCollection> >::iterator iEnd;
    
	LOG (2, trace, LFfwvis, "There are " << trackCollections.size () << " track collections.\n");
	int tc = 1;
	
	for (i = trackCollections.begin (), iEnd = trackCollections.end (); i != iEnd; ++i)
	{
	    try 
	    {
		const reco::TrackCollection c = *(*i).product ();
		LOG (3, trace, LFfwvis, "#" << tc++ << ":    track collection has " << c.size () << " tracks\n");
		for (reco::TrackCollection::const_iterator track = c.begin (), cEnd = c.end (); track != cEnd; ++track)
		{
		    m_tracks.push_back (*track);
		    LOG (3, trace, LFfwvis, "\nTrack with rechitsize = " << (*track).recHitsSize () << "\n");
		    
		    LOG (3, trace, LFfwvis, "\nConvert failed, use Inner and Onter States " <<  "\n");

                    std::vector<SbVec3f> myTraPos;
                    std::vector<SbVec3f> myTraDir;

	            if ((*track).innerOk () && (*track).outerOk ())
        	    {

			float x = (*track).innerPosition ().x () / 100.0;
			float y = (*track).innerPosition ().y () / 100.0;
			float z = (*track).innerPosition ().z () / 100.0;

			float dirx = (*track).innerMomentum ().x (); //FIXME
			float diry = (*track).innerMomentum ().y ();
			float dirz = (*track).innerMomentum ().z ();
                       
			myTraPos.push_back (SbVec3f (x, y, z));
			myTraDir.push_back (SbVec3f (dirx, diry, dirz));

			x = (*track).outerPosition ().x () / 100.0;
			y = (*track).outerPosition ().y () / 100.0;
			z = (*track).outerPosition ().z () / 100.0;

			dirx = (*track).outerMomentum ().x ();
			diry = (*track).outerMomentum ().y ();
			dirz = (*track).outerMomentum ().z ();

			myTraPos.push_back (SbVec3f (x, y, z));
			myTraDir.push_back (SbVec3f (dirx, diry, dirz));
              	    }
                    else
                    {
                        LOG (2, trace, LFfwvis, "\nOuter or Inner not OK\n");
                    }

                    m_trajectoryPositions.push_back (myTraPos);
                    m_trajectoryDirections.push_back (myTraDir);
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
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisTkIdealHelixTracksTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    // Prepare the property description.
    std::ostringstream text;
  
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
  
    text << "Reconstructed Tracks = ";
  
    try
    {    
	unsigned int tracks_size = m_tracks.size ();

        std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_trajectoryPositions.begin ();
        std::vector<std::vector<SbVec3f> >::const_iterator idir = m_trajectoryDirections.begin ();

	for (unsigned int nt = 0; nt < tracks_size; nt++)
	{
	    std::ostringstream out_pt;
	    out_pt << m_tracks [nt].pt (); 
	    text << "<table width='100%' border=1>"
		 << "<TR align = center>"
		 << "<TD COLSPAN =4>Track number = " << nt 
		 << " track pt = " << out_pt.str () 
		 << "</TR>";

	    int nState = 0;

            std::vector<SbVec3f>::const_iterator idd = (*idir).begin ();
            for (std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin (); ipp != (*ipos).end (); ++ipp)
	    {  
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nState++<< "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*ipp)[0] << "," << (*ipp)[1] << "," << (*ipp)[2] << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*idd)[0] << "," << (*idd)[1] << "," << (*idd)[2] << "</TD></TR>";
		idd++;
	    }
	    text << "</table>";  
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
  
    rep->setText (text.str ());
}

void
VisTkIdealHelixTracksTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.  
    VisQueuedTwig::update (rep);
    rep->clear ();
  
    if (! m_tracks.empty ())
    {
	SoSeparator *sep = new SoSeparator;    
      
	SoMaterial *recomat = new SoMaterial;
	recomat->diffuseColor.setValue (1.0, 0.0, 0.0);
	sep->addChild (recomat);
        //set line width
        SoDrawStyle *sty = new SoDrawStyle;
        sty->style = SoDrawStyle::LINES;
        sty->lineWidth.setValue (trackThickness);
        sep->addChild (sty);

        if (!m_trajectoryPositions.empty () && !m_trajectoryDirections.empty ())
        {
	    std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_trajectoryPositions.begin ();
	    std::vector<std::vector<SbVec3f> >::const_iterator idir = m_trajectoryDirections.begin ();

	    for (std::vector<reco::Track>::const_iterator itrack = m_tracks.begin (), itrackEnd = m_tracks.end (); itrack != itrackEnd; ++itrack)
	    {
		int nState = 0;
		std::vector<SbVec3f>::const_iterator idd = (*idir).begin ();
		for (std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin (); ipp != (*ipos).end (); ++ipp)
		{
		    if ((*itrack).charge() > 0)
		    {
			IgSoIdealTrack* myTraj = new IgSoIdealTrack;
			myTraj->setBField (m_BField.value ());
			myTraj->setRMax (m_RMax.value ());
			myTraj->setZMax (m_ZMax.value ());
			myTraj->t0.setValue (m_t0.value ());
			myTraj->dt.setValue (m_dt.value ());
			myTraj->initialise ((*ipp)[0], (*ipp)[1], (*ipp)[2],
					    (*idd)[0], (*idd)[1], (*idd)[2],
					    m_flytime,
					    IgParticleChar::getByName ("muon"));
			sep->addChild (myTraj);
		    }
		    else if ((*itrack).charge() < 0)
		    {
			IgSoIdealTrack* myTraj = new IgSoIdealTrack;
			myTraj->setBField (m_BField.value ());
			myTraj->setRMax (m_RMax.value ());
			myTraj->setZMax (m_ZMax.value ());
			myTraj->t0.setValue (m_t0.value ());
			myTraj->dt.setValue (m_dt.value ());
			myTraj->initialise ((*ipp)[0], (*ipp)[1], (*ipp)[2],
					    (*idd)[0], (*idd)[1], (*idd)[2],
					    m_flytime,
					    IgParticleChar::getByName ("electron"));
			sep->addChild (myTraj);
		    } 
		    else
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
                         
			track->controlPoints.set1Value (0, SbVec3f ((*ipp)[0], (*ipp)[1], (*ipp)[2]));
			track->markerPoints.set1Value (0, SbVec3f ((*ipp)[0], (*ipp)[1], (*ipp)[2]));
                          
			track->controlPoints.set1Value (1, SbVec3f ((*ipp)[0], (*ipp)[1], (*ipp)[2]));
			track->markerPoints.set1Value (1, SbVec3f ((*ipp)[0] * 10, (*ipp)[1] * 10, (*ipp)[2] * 10));
			gammas->addChild (track);
                          
			sep->addChild (gammas);
		    }
		    
		    nState++;
		    idd++;
		}
		ipos++;
		idir++;
	    }
        }
	rep->node ()->addChild (sep);
    }
}

void
VisTkIdealHelixTracksTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_tracks.empty ())
    {
        SoSeparator *sep = new SoSeparator;

        SoMaterial *recomat = new SoMaterial;
        recomat->diffuseColor.setValue (1.0, 0.0, 0.0);
        sep->addChild (recomat);
        //set line width
        SoDrawStyle *sty = new SoDrawStyle;
        sty->style = SoDrawStyle::LINES;
        sty->lineWidth.setValue (trackThickness);
        sep->addChild (sty);

        if (! m_trajectoryPositions.empty () && ! m_trajectoryDirections.empty ())        
	{
            std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_trajectoryPositions.begin ();
            std::vector<std::vector<SbVec3f> >::const_iterator idir = m_trajectoryDirections.begin ();

            for (std::vector<reco::Track>::const_iterator itrack = m_tracks.begin (); itrack != m_tracks.end (); ++itrack)
            {
                int nState = 0;
                std::vector<SbVec3f>::const_iterator idd = (*idir).begin ();
                for (std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin (); ipp != (*ipos).end (); ++ipp)
                {
                    IgSoIdealTrack* myTraj = new IgSoIdealTrack;
		    myTraj->setBField (m_BField.value ());
		    myTraj->setRMax (m_RMax.value ());
		    myTraj->setZMax (m_ZMax.value ());
		    myTraj->t0.setValue (m_t0.value ());
		    myTraj->dt.setValue (m_dt.value ());
		    if ((*itrack).charge() > 0)
		    {
			myTraj->initialise ((*ipp)[0], (*ipp)[1], (*ipp)[2],
					    (*idd)[0], (*idd)[1], (*idd)[2],
					    m_flytime,
					    IgParticleChar::getByName ("muon"));
		    }
		    else if ((*itrack).charge() < 0)
		    {
			myTraj->initialise ((*ipp)[0], (*ipp)[1], (*ipp)[2],
					    (*idd)[0], (*idd)[1], (*idd)[2],
					    m_flytime,
					    IgParticleChar::getByName ("electron"));
		    } else
		    {
			myTraj->initialise ((*ipp)[0], (*ipp)[1], (*ipp)[2],
					    (*idd)[0], (*idd)[1], (*idd)[2],
					    m_flytime,
					    IgParticleChar::getByName ("gamma"));
		    }

                    sep->addChild (myTraj);
                    nState++;
                    idd++;
                }
                ipos++;
                idir++;
            }
        }
        rep->node ()->addChild (sep);
    }
}


void
VisTkIdealHelixTracksTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_tracks.empty ())
    {
        SoSeparator *sep = new SoSeparator;

        SoMaterial *recomat = new SoMaterial;
        recomat->diffuseColor.setValue (1.0, 0.0, 0.0);
        sep->addChild (recomat);
        //set line width
        SoDrawStyle *sty = new SoDrawStyle;
        sty->style = SoDrawStyle::LINES;
        sty->lineWidth.setValue (trackThickness);
        sep->addChild (sty);

        if (!m_trajectoryPositions.empty () && !m_trajectoryDirections.empty ())
	{
            std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_trajectoryPositions.begin ();
            std::vector<std::vector<SbVec3f> >::const_iterator idir = m_trajectoryDirections.begin ();
            for (std::vector<reco::Track>::const_iterator itrack = m_tracks.begin (); itrack != m_tracks.end (); ++itrack)
            {
                int nState = 0;
                std::vector<SbVec3f>::const_iterator idd = (*idir).begin ();
                for (std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin (); ipp != (*ipos).end (); ++ipp)
                {
                    IgSoIdealTrack* myTraj = new IgSoIdealTrack;
		    myTraj->setBField (m_BField.value ());
		    myTraj->setRMax (m_RMax.value ());
		    myTraj->setZMax (m_ZMax.value ());
		    myTraj->t0.setValue (m_t0.value ());
		    myTraj->dt.setValue (m_dt.value ());
		    if ((*itrack).charge() > 0)
		    {
			myTraj->initialise ((*ipp)[0], (*ipp)[1], (*ipp)[2],
					    (*idd)[0], (*idd)[1], (*idd)[2],
					    m_flytime,
					    IgParticleChar::getByName ("muon"));
		    }
		    else if ((*itrack).charge() < 0)
		    {
			myTraj->initialise ((*ipp)[0], (*ipp)[1], (*ipp)[2],
					    (*idd)[0], (*idd)[1], (*idd)[2],
					    m_flytime,
					    IgParticleChar::getByName ("electron"));
		    } else
		    {
			myTraj->initialise ((*ipp)[0], (*ipp)[1], (*ipp)[2],
					    (*idd)[0], (*idd)[1], (*idd)[2],
					    m_flytime,
					    IgParticleChar::getByName ("gamma"));
		    }

                    sep->addChild (myTraj);
                    nState++;
                    idd++;
                }
                ipos++;
                idir++;
            }
        }
        rep->node ()->addChild (sep);
    }
}

void
VisTkIdealHelixTracksTwig::update (IgLegoRep *rep)
{

    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    //........ still missing
}

