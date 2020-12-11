//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuon/interface/VisTrackTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "FWCore/Framework/interface/Event.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoIdealTrack.h"
#include "Iguana/Inventor/interface/IgParticleChar.h"
#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <QString>
#include <sstream>
#include <iomanip>
#include <classlib/utils/Error.h>

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
	return new VisTrackTwig (state, parent, "Tracks (" + name + ")", 
				 friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisTrackTwig::VisTrackTwig (IgState *state, IgTwig *parent,
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
      m_animate (state, lat::CreateCallback (this, &VisTrackTwig::configChanged)),
      m_counterMax (state, lat::CreateCallback (this, &VisTrackTwig::configChanged)),
      m_counterMin (state, lat::CreateCallback (this, &VisTrackTwig::configChanged)),
      m_frequency (state, lat::CreateCallback (this, &VisTrackTwig::configChanged)),
      m_BField (state, lat::CreateCallback (this, &VisTrackTwig::configChanged)),
      m_RMax (state, lat::CreateCallback (this, &VisTrackTwig::configChanged)),
      m_ZMax (state, lat::CreateCallback (this, &VisTrackTwig::configChanged)),
      m_t0 (state, lat::CreateCallback (this, &VisTrackTwig::configChanged)),
      m_dt (state, lat::CreateCallback (this, &VisTrackTwig::configChanged)),
      m_annot (state, lat::CreateCallback (this, &VisTrackTwig::configChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID trackID (typeid (reco::TrackCollection));
    tfService->registerTwig (trackID.friendlyClassName (), &createThisTwig);
}

void
VisTrackTwig::configChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisTrackTwig::onNewEvent (const edm::Event &event,
                          const edm::EventSetup &eventSetup)
{    
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    typedef std::vector<edm::Handle<reco::TrackCollection> > Collections;
    Collections trackCollections;

    m_tracks.clear ();

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

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
	Collections::iterator i (trackCollections.begin ()), end (trackCollections.end ());
	for (; i != end; ++i)
	{
	    const reco::TrackCollection c = *(*i).product ();
	    for (reco::TrackCollection::const_iterator track = c.begin (), tEnd = c.end (); track != tEnd; ++track)
	    {
		m_tracks.push_back (*track);
	    }
	}
    }

    VisQueuedTwig::onBaseInvalidate ();
}

void
VisTrackTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    std::ostringstream text;

    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    text << "Reconstructed Tracks = ";

    try
    {    
	text << std::setw (3) << m_tracks.size ();
	int nt = 0;

	if (! m_tracks.empty ())
	{
	    std::vector<reco::Track>::iterator i (m_tracks.begin ()), end (m_tracks.end ());
	    for (; i != end; ++i) 
	    {
		std::ostringstream out_pt;
		out_pt << (*i).pt (); 
		text << "<table width='100%' border=1>"
		     << "<TR align = center>"
		     << "<TD COLSPAN =4>Track number = " << nt++ 
		     << " with Pt = " << out_pt.str () 
		     << " and charge = " << (*i).charge ()
		     << "</TD></TR><TR align = center>"
		     << "<TH>Reference Point (vx, vy, vz)</TH>" 
		     << "<TH>px, py, pz</TH>"
		     << "<TH>Phi</TH>"
		     << "<TH>Eta</TH>"
		     << "</TR>";

		text << "<TR><TD>" << std::setw (3) << (*i).vx ()/100. << ", " << (*i).vy()/100. << ", " << (*i).vz()/100. << "</TD>"
		     << "<TD>" << std::setw (3) << (*i).px () << ", " << (*i).py() << ", " << (*i).pz() << "</TD>"
		     << "<TD>" << std::setw (3) << (*i).phi () << "</TD>"
		     << "<TD>" << std::setw (3) << (*i).eta () << "</TD></TR>";

		text << "</table>";  
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

    rep->setText (text.str ());
}

void
VisTrackTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisTrackTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if (! m_tracks.empty ())
    {
	SoSeparator *sep = new SoSeparator;    

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);
 
	SoTimeCounter *counter = new SoTimeCounter;
	counter->max = (short int) m_counterMax.value ();
	counter->min = (short int) m_counterMin.value ();
	counter->frequency = m_frequency.value ();

	std::vector<reco::Track>::iterator i (m_tracks.begin ()), end (m_tracks.end ());
	for (; i != end; ++i) 
	{
	    SoSeparator *labelSep = new SoSeparator;
	    QString label;
	    SoText2  *trackLabel = new SoText2;
	    SoTranslation *trackLabelTranslation = new SoTranslation;
	    
	    SbVec3f pos = SbVec3f ((*i).px (),
				   (*i).py (),
				   (*i).pz ());
	    pos.normalize ();
	    
	    trackLabelTranslation->translation = pos * m_ZMax.value ();
	    labelSep->addChild (trackLabelTranslation);
	    labelSep->addChild (trackLabel);

	    IgSoIdealTrack* myTraj = new IgSoIdealTrack;
	    myTraj->setBField(m_BField.value ());
	    myTraj->setRMax(m_RMax.value ());
	    myTraj->setZMax(m_ZMax.value ());

	    IgParticleChar *pid;
	    (*i).charge () >= 0 ? pid = IgParticleChar::getByName ("muon") : pid = IgParticleChar::getByName ("antimuon");

	    // SL: IgSoIdealTrack wants vertex coordinates in meter, not centimeter
	    myTraj->initialise ((*i).vx()/100., (*i).vy()/100., (*i).vz()/100.,
				(*i).px(), (*i).py(), (*i).pz(),
				0.0,
				pid);
	    label = QString ("%1 GeV").arg ((*i).pt(), 0, 'f', 2);
	    trackLabel->string = label.latin1 (); 
	    
	    myTraj->t0.setValue (m_t0.value ());
 	    myTraj->dt.setValue (m_dt.value ());
	    
	    if (m_animate.value ())
	        myTraj->dt.connectFrom (&counter->output);

	    sep->addChild (myTraj);         

	    if (m_annot.value () == true)
	    {
		sep->addChild (labelSep);
	    }		
	}
	rep->node ()->addChild (sep);
    }
}

void
VisTrackTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if (! m_tracks.empty ())
    {
	SoSeparator *sep = new SoSeparator;    

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);
 
	SoTimeCounter *counter = new SoTimeCounter;
	counter->max = (short int) m_counterMax.value ();
	counter->min = (short int) m_counterMin.value ();
	counter->frequency = m_frequency.value ();

	std::vector<reco::Track>::iterator i (m_tracks.begin ()), end (m_tracks.end ());
	for (; i != end; ++i) 
	{
	    SoSeparator *labelSep = new SoSeparator;
	    QString label;
	    SoText2  *trackLabel = new SoText2;
	    SoTranslation *trackLabelTranslation = new SoTranslation;
	    
	    SbVec3f pos = SbVec3f ((*i).px (),
				   (*i).py (),
				   (*i).pz ());
	    pos.normalize ();
	    
	    trackLabelTranslation->translation = pos * m_ZMax.value ();
	    labelSep->addChild (trackLabelTranslation);
	    labelSep->addChild (trackLabel);

	    IgSoIdealTrack* myTraj = new IgSoIdealTrack;
	    myTraj->setBField(m_BField.value ());
	    myTraj->setRMax(m_RMax.value ());
	    myTraj->setZMax(m_ZMax.value ());

	    IgParticleChar *pid;
	    (*i).charge () >= 0 ? pid = IgParticleChar::getByName ("muon") : pid = IgParticleChar::getByName ("antimuon");

	    // SL: IgSoIdealTrack wants vertex coordinates in meter, not centimeter
	    myTraj->initialise ((*i).vx()/100., (*i).vy()/100., (*i).vz()/100.,
				(*i).px(), (*i).py(), (*i).pz(),
				0.0,
				pid);
	    label = QString ("%1 GeV").arg ((*i).pt(), 0, 'f', 2);
	    trackLabel->string = label.latin1 (); 
	    
	    myTraj->t0.setValue (m_t0.value ());
 	    myTraj->dt.setValue (m_dt.value ());
	    
	    if (m_animate.value ())
	        myTraj->dt.connectFrom (&counter->output);

	    sep->addChild (myTraj);         

	    if (m_annot.value () == true)
	    {
		sep->addChild (labelSep);
	    }		
	}
	rep->node ()->addChild (sep);
    }
}

void
VisTrackTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if (! m_tracks.empty ())
    {
	SoSeparator *sep = new SoSeparator;    

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);
 
	SoTimeCounter *counter = new SoTimeCounter;
	counter->max = (short int) m_counterMax.value ();
	counter->min = (short int) m_counterMin.value ();
	counter->frequency = m_frequency.value ();

	std::vector<reco::Track>::iterator i (m_tracks.begin ()), end (m_tracks.end ());
	for (; i != end; ++i) 
	{
	    SoSeparator *labelSep = new SoSeparator;
	    QString label;
	    SoText2  *trackLabel = new SoText2;
	    SoTranslation *trackLabelTranslation = new SoTranslation;
	    
	    SbVec3f pos = SbVec3f ((*i).px (),
				   (*i).py (),
				   (*i).pz ());
	    pos.normalize ();
	    
	    trackLabelTranslation->translation = pos * m_ZMax.value ();
	    labelSep->addChild (trackLabelTranslation);
	    labelSep->addChild (trackLabel);

	    IgSoIdealTrack* myTraj = new IgSoIdealTrack;
	    myTraj->setBField(m_BField.value ());
	    myTraj->setRMax(m_RMax.value ());
	    myTraj->setZMax(m_ZMax.value ());

	    IgParticleChar *pid;
	    (*i).charge () >= 0 ? pid = IgParticleChar::getByName ("muon") : pid = IgParticleChar::getByName ("antimuon");

	    // SL: IgSoIdealTrack wants vertex coordinates in meter, not centimeter
	    myTraj->initialise ((*i).vx()/100., (*i).vy()/100., (*i).vz()/100.,
				(*i).px(), (*i).py(), (*i).pz(),
				0.0,
				pid);
	    label = QString ("%1 GeV").arg ((*i).pt(), 0, 'f', 2);
	    trackLabel->string = label.latin1 (); 
	    
	    myTraj->t0.setValue (m_t0.value ());
 	    myTraj->dt.setValue (m_dt.value ());
	    
	    if (m_animate.value ())
	        myTraj->dt.connectFrom (&counter->output);

	    sep->addChild (myTraj);         

	    if (m_annot.value () == true)
	    {
		sep->addChild (labelSep);
	    }		
	}
	rep->node ()->addChild (sep);
    }
}
