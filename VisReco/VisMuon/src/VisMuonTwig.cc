//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuon/interface/VisMuonTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "VisReco/VisMuon/interface/VisMuonEnergyTwig.h"
#include "VisReco/VisMuon/interface/VisRefitTrackTwig.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonChamberMatch.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/Framework/interface/Event.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoIdealTrack.h"
#include "Iguana/Inventor/interface/IgParticleChar.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
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
	IgTwig *rootTwig = IgDocumentData::get (state)->root ();
	IgTwig *eventTwig = 0;
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Muon Event/Muons");
	
	if (! eventTwig)
	    eventTwig = parent;
	else
	{
	    IgRepSet::invalidate (eventTwig->parent (), IgTwig::STRUCTURE_MASK);
        }
	return new VisMuonTwig (state, eventTwig, "[N/A] Muon (" + name + ")", 
				friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisMuonTwig::VisMuonTwig (IgState *state, IgTwig *parent,
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
      m_animate (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_counterMax (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_counterMin (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_frequency (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_BField (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_RMax (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_ZMax (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_t0 (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_dt (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_annot (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_showDets (state, lat::CreateCallback (this, &VisMuonTwig::configChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID muonID (typeid (reco::MuonCollection));
    tfService->registerTwig (muonID.friendlyClassName (), &createThisTwig);    
}

VisMuonTwig::VisMuonTwig (IgState *state, IgTwig *parent,
			  const std::string &name,
			  const std::string &friendlyName,
			  const std::string &moduleLabel,
			  const std::string &instanceName,
			  const std::string &processName,
			  const reco::Muon &muon,
			  std::vector<DetId> &dets)
    : VisQueuedTwig (state, parent, name),
      m_text ("no info"),
      m_friendlyName (friendlyName),
      m_moduleLabel (moduleLabel),
      m_instanceName (instanceName),
      m_processName (processName),
      m_animate (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_counterMax (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_counterMin (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_frequency (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_BField (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_RMax (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_ZMax (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_t0 (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_dt (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_annot (state, lat::CreateCallback (this, &VisMuonTwig::configChanged)),
      m_showDets (state, lat::CreateCallback (this, &VisMuonTwig::configChanged))
{
    m_muons.push_back (muon);
    m_dets = dets;
}

void
VisMuonTwig::configChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisMuonTwig::onNewEvent (const edm::Event &event,
			 const edm::EventSetup &eventSetup)
{    
    if (this->children ())
    {
	for (unsigned int i = 0; i < this->children (); ++i)
	    this->child (i)->destroy ();
    }
    
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    typedef std::vector<edm::Handle<reco::MuonCollection> > Collections;
    Collections muonCollections;

    m_muons.clear ();
    m_dets.clear ();

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, muonCollections);	    
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisMuonTwig: ");
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
    if (! muonCollections.empty ())
    {
        Collections::iterator i (muonCollections.begin ()), end (muonCollections.end ());
	for (; i != end; ++i)
	{
	    const reco::MuonCollection c = *(*i).product ();
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    
	    this->name (nameStr.toStdString ());
	    
	    for (reco::MuonCollection::const_iterator muon = c.begin (), tEnd = c.end (); muon != tEnd; ++muon)
	    {
		std::vector<DetId> muonDetIds;
		muonDetIds.clear ();
		
		QString label;
		if ((*muon).charge () > 0)
		{
		    label = QString ("mu %1 GeV").arg ((*muon).pt(), 0, 'f', 2);
		}
		else if ((*muon).charge () < 0)
		{
		    label = QString ("mu- %1 GeV").arg ((*muon).pt(), 0, 'f', 2);
		}
		else
		    label = QString ("uncharged mu %1 GeV").arg ((*muon).pt(), 0, 'f', 2);
		
		if ((*muon).isMatchesValid ())
		{		    
		    const std::vector<reco::MuonChamberMatch> &dets = (*muon).matches ();
		    for (std::vector<reco::MuonChamberMatch>::const_iterator dit = dets.begin (), ditEnd = dets.end (); dit != ditEnd; ++dit)
		    {		    
			m_dets.push_back ((*dit).id);
			muonDetIds.push_back ((*dit).id);
		    }
		}
		VisMuonTwig *msep = new VisMuonTwig (state (), dynamic_cast<IgTwig *>(this), label.toStdString (), m_friendlyName, m_moduleLabel, 
						     m_instanceName, m_processName, *muon, muonDetIds);
		msep->selfVisible (this->selfVisible ());
		
		m_muons.push_back (*muon);
		if ((*muon).innerTrack ().isNonnull ())
		{
		    double pt = (*muon).innerTrack ()->pt ();
		    label = QString ("Tracker %1 GeV").arg (pt, 0, 'f', 2);		    
		    VisRefitTrackTwig *twig = new VisRefitTrackTwig (state (), msep, label.toStdString (), (*muon).innerTrack ());
		    twig->selfVisible (msep->selfVisible ());    
		}
		
		if ((*muon).outerTrack ().isNonnull ())
		{
		    double pt = (*muon).outerTrack ()->pt ();		    
		    label = QString ("Standalone %1 GeV").arg (pt, 0, 'f', 2);		    
		    VisRefitTrackTwig *twig = new VisRefitTrackTwig (state (), msep, label.toStdString (), (*muon).outerTrack ());
		    twig->selfVisible (msep->selfVisible ());    
		}
		
		if ((*muon).globalTrack ().isNonnull ())
		{
		    double pt = (*muon).globalTrack ()->pt ();		    
		    label = QString ("Global %1 GeV").arg (pt, 0, 'f', 2);		    
		    VisRefitTrackTwig *twig = new VisRefitTrackTwig (state (), msep, label.toStdString (), (*muon).globalTrack ());
		    twig->selfVisible (msep->selfVisible ());    
		}
		
		if ((*muon).isEnergyValid ())
		{
		    float pt = (*muon).calEnergy ().tower;		    
		    label = QString ("CaloTower %1 GeV").arg (pt, 0, 'f', 2);
		    new VisMuonEnergyTwig (state (), msep, label.toStdString (), (*muon).calEnergy ());		    
		}		
	    }
	}
    }

    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisMuonTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    std::ostringstream text;

    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
    {
	text << "Muons = ";

	try
	{    
	    text << std::setw (3) << m_muons.size ();
	    int nt = 0;
	
	    if (! m_muons.empty ())
	    {
		std::vector<reco::Muon>::iterator i (m_muons.begin ()), end (m_muons.end ());
		for (; i != end; ++i) 
		{
		    std::ostringstream out_pt;
		    out_pt << (*i).pt (); 
		    text << "<table width='100%' border=1>"
			 << "<TR align = center>"
			 << "<TD COLSPAN = 5>Muon number = " << nt++ 
			 << " with Pt = " << out_pt.str () 
			 << " and charge = " << (*i).charge ()
			 << ", calo-compatibility = " << muon::caloCompatibility (*i)
			 << " and segment-compatibility = " << muon::segmentCompatibility (*i)
			 << "</TD></TR><TR align = center>"
			 << "<TH>Reference Point (vx, vy, vz)</TH>" 
			 << "<TH>px, py, pz</TH>"
			 << "<TH>Phi</TH>"
			 << "<TH>Eta</TH>"
			 << "<TH># chambers</TH>"
			 << "</TR>";
		
		    text << "<TR><TD>" << std::setw (3) << (*i).vx ()/100. << ", " << (*i).vy()/100. << ", " << (*i).vz()/100. << "</TD>"
			 << "<TD>" << std::setw (3) << (*i).px () << ", " << (*i).py() << ", " << (*i).pz() << "</TD>"
			 << "<TD>" << std::setw (3) << (*i).phi () << "</TD>"
			 << "<TD>" << std::setw (3) << (*i).eta () << "</TD>"
			 << "<TD>" << std::setw (3) << (*i).numberOfChambers () << "</TD></TR>";
	
		    text << "</table>";  
		}		
	    }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisMuonTwig: ");
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
    }
    else
	text << "This is a structure branch. "
	     << "Select its children for information about "
	     << "Muon collections. Double mouse click to "
	     << "expand or collapse it.";
    
    rep->setText (text.str ());
}

void
VisMuonTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_muons.empty ())
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

	std::vector<reco::Muon>::iterator i (m_muons.begin ()), end (m_muons.end ());
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
            myTraj->setBField (m_BField.value ());
            myTraj->setRMax (m_RMax.value ());
            myTraj->setZMax (m_ZMax.value ());
            if ((*i).charge () >= 0)
	    {
		myTraj->initialise ((*i).vx()/100., (*i).vy()/100., (*i).vz()/100.,
				    (*i).px(), (*i).py(), (*i).pz(),
				    1.0,
				    IgParticleChar::getByName ("muon"));
		label = QString ("mu %1 GeV").arg ((*i).pt(), 0, 'f', 2);
		trackLabel->string = label.latin1 (); 
	    }
	    else 
	    {
		myTraj->initialise ((*i).vx()/100., (*i).vy()/100., (*i).vz()/100.,
				    (*i).px(), (*i).py(), (*i).pz(),
				    1.0,
				    IgParticleChar::getByName ("antimuon"));
		label = QString ("mu- %1 GeV").arg ((*i).pt(), 0, 'f', 2);
		trackLabel->string = label.latin1 ();
	    }
	    
	    myTraj->t0.setValue (m_t0.value ());
 	    myTraj->dt.setValue (m_dt.value ());
	    
	    if (m_animate.value ())
	        myTraj->dt.connectFrom (&counter->output);

	    // sep->addChild (myTraj);

	    if (m_annot.value () == true)
	    {
		sep->addChild (labelSep);
	    }
	    if (m_showDets.value () == true)
	    {
		VisEventSetupService *esService = VisEventSetupService::get (state ());
		if (esService)
		{
		    SoSeparator *detSep = esService->buildHitDets (VisEventSetupService::FullDet, m_dets);		    
		    if (detSep != NULL)
			sep->addChild (detSep);		    
		}		
	    }	    
        }
        rep->node ()->addChild (sep);
    }
}

void
VisMuonTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_muons.empty ())
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

	std::vector<reco::Muon>::iterator i (m_muons.begin ()), end (m_muons.end ());
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
            myTraj->setBField (m_BField.value ());
            myTraj->setRMax (m_RMax.value ());
            myTraj->setZMax (m_ZMax.value ());
            if ((*i).charge () >= 0)
	    {
		myTraj->initialise ((*i).vx()/100., (*i).vy()/100., (*i).vz()/100.,
				    (*i).px(), (*i).py(), (*i).pz(),
				    1.0,
				    IgParticleChar::getByName ("muon"));
		label = QString ("mu %1 GeV").arg ((*i).pt(), 0, 'f', 2);
		trackLabel->string = label.latin1 (); 
	    }
	    else 
	    {
		myTraj->initialise ((*i).vx()/100., (*i).vy()/100., (*i).vz()/100.,
				    (*i).px(), (*i).py(), (*i).pz(),
				    1.0,
				    IgParticleChar::getByName ("antimuon"));
		label = QString ("mu- %1 GeV").arg ((*i).pt(), 0, 'f', 2);
		trackLabel->string = label.latin1 ();
	    }
	    
	    myTraj->t0.setValue (m_t0.value ());
 	    myTraj->dt.setValue (m_dt.value ());

	    if (m_animate.value ())
	        myTraj->dt.connectFrom (&counter->output);

	    // sep->addChild (myTraj);		
	    if (m_annot.value () == true)
	    {
		sep->addChild (labelSep);
	    }		
	    if (m_showDets.value () == true)
	    {
		VisEventSetupService *esService = VisEventSetupService::get (state ());
		if (esService)
		{
		    SoSeparator *detSep = esService->buildHitDets (VisEventSetupService::RPhiDet, m_dets);		    
		    if (detSep != NULL)
			sep->addChild (detSep);		    
		}		
	    }	    
        }
        rep->node ()->addChild (sep);
    }
}

void
VisMuonTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_muons.empty ())
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

	std::vector<reco::Muon>::iterator i (m_muons.begin ()), end (m_muons.end ());
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
            myTraj->setBField (m_BField.value ());
            myTraj->setRMax (m_RMax.value ());
            myTraj->setZMax (m_ZMax.value ());
            if ((*i).charge () >= 0)
	    {
		myTraj->initialise ((*i).vx()/100., (*i).vy()/100., (*i).vz()/100.,
				    (*i).px(), (*i).py(), (*i).pz(),
				    1.0,
				    IgParticleChar::getByName ("muon"));
		label = QString ("mu %1 GeV").arg ((*i).pt(), 0, 'f', 2);
		trackLabel->string = label.latin1 (); 
	    }
	    else 
	    {
		myTraj->initialise ((*i).vx()/100., (*i).vy()/100., (*i).vz()/100.,
				    (*i).px(), (*i).py(), (*i).pz(),
				    1.0,
				    IgParticleChar::getByName ("antimuon"));
		label = QString ("mu- %1 GeV").arg ((*i).pt(), 0, 'f', 2);
		trackLabel->string = label.latin1 ();
	    }
	    
	    myTraj->t0.setValue (m_t0.value ());
 	    myTraj->dt.setValue (m_dt.value ());

	    if (m_animate.value ())
	        myTraj->dt.connectFrom (&counter->output);

	    // sep->addChild (myTraj);		
	    if (m_annot.value () == true)
	    {
		sep->addChild (labelSep);
	    }		
	    if (m_showDets.value () == true)
	    {
		VisEventSetupService *esService = VisEventSetupService::get (state ());
		if (esService)
		{
		    SoSeparator *detSep = esService->buildHitDets (VisEventSetupService::RZDet, m_dets);		    
		    if (detSep != NULL)
			sep->addChild (detSep);		    
		}		
	    }	    
        }
        rep->node ()->addChild (sep);
    }
}
