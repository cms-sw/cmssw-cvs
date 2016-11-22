//<<<<<< INCLUDES        
#include "VisReco/VisTracker/interface/VisTkRecTracksTwig.h"                                     
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h" 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoSplineTrack.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <classlib/utils/Error.h>
#include <sstream>
#include <iomanip>
#include <iterator>

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
	IgTwig *rootTwig = IgDocumentData::get (state)->root ();
	IgTwig *eventTwig = 0;
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Tracker Event/Reconstructed Tracks");
	
	if (! eventTwig) 
	    eventTwig = parent;
		
	return new VisTkRecTracksTwig (state, eventTwig, "[N/A] Track (" + name + ")", 
				       friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisTkRecTracksTwig::VisTkRecTracksTwig  (IgState *state, IgTwig *parent,
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
      m_trackCollectionNames (std::vector<std::string> ()),
      m_haloFlag (false),
      m_trackThickness (2.0),
      m_selectedTrack(state, lat::CreateCallback (this, &VisTkRecTracksTwig::twigChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID trackID (typeid (reco::TrackCollection));
    tfService->registerTwig (trackID.friendlyClassName (), &createThisTwig);

    const edm::ParameterSet *pset = VisConfigurationService::pSet ();
    if (pset)
    {
	m_haloFlag = pset->getUntrackedParameter<bool>("VisHaloTracks", false);
	m_trackThickness = pset->getUntrackedParameter<double> ("trackLineThickness", 2.0);
	std::vector<std::string> notSpecified;
	m_trackCollectionNames = pset->getUntrackedParameter<std::vector<std::string> > ("trackCollectionNames", notSpecified);
    } 
}
void
VisTkRecTracksTwig::onNewEvent (const edm::Event &event,
				const edm::EventSetup &eventSetup)
{
    LOG (1, trace, LFfwvis, "VisTkRecTracksTwig[" << name ()
	 << "]::onNewEvent()\n");
    
    VisQueuedTwig::onBaseInvalidate ();
    m_tracks.clear ();
    m_trackPoints.clear ();

    m_trajectoryPositions.clear ();
    m_trajectoryDirections.clear ();

    m_text = (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ())
	      .arg (event.luminosityBlock ())
	      .arg (event.orbitNumber ())
	      .arg (event.bunchCrossing ())
	      .latin1 ());

    std::vector<edm::Handle<reco::TrackCollection> > trackCollections;
  
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, trackCollections);
	}
    }
    catch (lat::Error& e)
    {
	if (this->m_onError)
	    this->m_onError (&e);
    }
    catch (cms::Exception& e)
    {
	if (this->m_onCmsException)
	    this->m_onCmsException (&e);
    }
    catch (std::exception& e)
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
	
	for (i = trackCollections.begin (), iEnd = trackCollections.end (); i != iEnd; ++i)
	{
	    try 
	    {
		const reco::TrackCollection c = *(*i).product ();
		QString sizeStr = (QString ("%1").arg (c.size ()));
		QString nameStr = QString::fromStdString (this->name ());
		int ib = nameStr.find ("[");
		int ie = nameStr.find ("]");
		nameStr.replace (ib + 1, ie - 1, sizeStr);
		this->name (nameStr.toStdString ());

		for (reco::TrackCollection::const_iterator track = c.begin (), trackEnd = c.end (); track != trackEnd; ++track)
		{
		    m_tracks.push_back (*track);

		    std::vector<SbVec3f> myTraPos;
		    std::vector<SbVec3f> myTraDir;

		    if ((*track).innerOk () && (*track).outerOk ())
		    {
			float x = (*track).innerPosition ().x () / 100.0;
			float y = (*track).innerPosition ().y () / 100.0;
			float z = (*track).innerPosition ().z () / 100.0;
			SbVec3f posi (x, y, z);

			float dirx = (*track).innerMomentum ().x ();
			float diry = (*track).innerMomentum ().y ();
			float dirz = (*track).innerMomentum ().z ();
			SbVec3f diri (dirx, diry, dirz);
			diri.normalize ();
                       
			// 				myTraPos.push_back (SbVec3f (x, y, z));
			// 				myTraDir.push_back (diri);

			x = (*track).outerPosition ().x () / 100.0;
			y = (*track).outerPosition ().y () / 100.0;
			z = (*track).outerPosition ().z () / 100.0;
			SbVec3f poso (x, y, z);
				
			dirx = (*track).outerMomentum ().x ();
			diry = (*track).outerMomentum ().y ();
			dirz = (*track).outerMomentum ().z ();
			SbVec3f diro (dirx, diry, dirz);
			diro.normalize ();
			if (m_haloFlag) // only for use w/ halo tracks
			{
			    if ((poso-posi).dot (diri) < 0.F) diri = -1.F * diri;
			    if ((poso-posi).dot (diro) < 0.F) diro = -1.F * diro;
			}
			myTraPos.push_back (posi);
			myTraDir.push_back (diri);
			myTraPos.push_back (poso);
			// 				myTraPos.push_back (SbVec3f (x, y, z));
			myTraDir.push_back (diro);
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
    
    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisTkRecTracksTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisTkRecTracksTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    // Prepare the property description.
    std::ostringstream text;
  
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
  
    text << m_text << "<br>";

    try
    {    
	unsigned int tracks_size = m_tracks.size ();
	text << tracks_size << " Reconstructed Tracks in collection defined by " << m_friendlyName << ":" << m_moduleLabel << ":" << m_instanceName << ":" << m_processName;
	
	if (tracks_size == 0) 
	{
	    text << "" << std::endl;
	}
	else 
	{    
	    for (unsigned int nt = 0; nt < tracks_size; nt++)
	    {
                if (m_selectedTrack.value () == -1 || m_selectedTrack.value () == (int) nt)
		{
		    std::ostringstream out_pt;
		    out_pt << m_tracks [nt].pt (); 
		    text << "<table width='100%' border=1>"
			 << "<TR align = center>"
			 << "<TD COLSPAN = 5>Track number = " << nt 
			 << " track pt = " << out_pt.str () 
			 << " and charge = " << m_tracks [nt].charge ()
			 << "</TD></TR><TR align = center>"
			 << "<TH>Reference Point (vx, vy, vz)</TH>" 
			 << "<TH>px, py, pz</TH>"
			 << "<TH>Phi</TH>"
			 << "<TH>Eta</TH>"
			 << "<TH>RecHits (x, y, z)</TH>"
			 << "</TR>";
		  
		    text << "<TR><TD>" << std::setw (3) << std::setprecision (3)
			 << m_tracks [nt].vx () / 100. << ", " 
			 << m_tracks [nt].vy () / 100. << ", " 
			 << m_tracks [nt].vz () / 100. << "</TD>"
			 << "<TD>" << std::setw (3) << std::setprecision (3)
			 << m_tracks [nt].px () << ", " 
			 << m_tracks [nt].py () << ", " 
			 << m_tracks [nt].pz () << "</TD>"
			 << "<TD>" << std::setw (3) << std::setprecision (3) << m_tracks [nt].phi () << "</TD>"
			 << "<TD>" << std::setw (3) << std::setprecision (3) << m_tracks [nt].eta () << "</TD>";

		    int n_rechit = 0;
		    text << "<TD>";
		    
		    if (nt < m_trackPoints.size ()) //FIXME
			for (std::vector<SbVec3f>::const_iterator ir = m_trackPoints[nt].begin (); ir != m_trackPoints[nt].end (); ++ir) 
			{
			    SbVec3f xyzPoint = (*ir);
			    float x = xyzPoint [0];
			    float y = xyzPoint [1];
			    float z = xyzPoint [2];
			
			    text << std::setw (3) << n_rechit++ << ":("
				 << std::setprecision (3) << x << "," << y << "," << z << "), ";
			    
			}
		    text << "</TD></TR>";
		    text << "</table>";  
		}
	    }
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTkRecTracksTwig::update(IgTextRep*) ");
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
    
    rep->setText (text.str ());
}

void
VisTkRecTracksTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.  
    VisQueuedTwig::update (rep);
  
    rep->clear ();
  
    SoSeparator *sep = new SoSeparator;    
      
    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x99ccff, rgbcomponents);
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);

    //set line width
    SoDrawStyle *sty = new SoDrawStyle;
    sty->style = SoDrawStyle::LINES;
    sty->lineWidth.setValue (m_trackThickness);
    sep->addChild (sty);

    if (!m_trajectoryPositions.empty () && !m_trajectoryDirections.empty ())
    {
	std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_trajectoryPositions.begin ();
	std::vector<std::vector<SbVec3f> >::const_iterator idir = m_trajectoryDirections.begin ();
	for (unsigned int itrack = 0; itrack != m_trajectoryPositions.size (); ++itrack)
	{
	    if(m_selectedTrack.value()==-1 || m_selectedTrack.value()==(int)itrack){
		IgSoSplineTrack* myTraj = new IgSoSplineTrack;
		SoMFVec3f tangents;
		SoMFVec3f points;
		int nState = 0;
		std::vector<SbVec3f>::const_iterator idd = (*idir).begin ();
		for (std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin (); ipp != (*ipos).end (); ++ipp)
		{
		    points.set1Value (nState, (*ipp));
		    tangents.set1Value (nState, (*idd));
		      
		    nState++;
		    idd++;
		}
		myTraj->points = points;
		myTraj->tangents = tangents;
		  
		sep->addChild (myTraj);
	    }
	    ipos++;
	    idir++;
	}
    }
    rep->node ()->addChild (sep);
}

void
VisTkRecTracksTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if (! m_tracks.empty ())
    {
        SoSeparator *sep = new SoSeparator;

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x99ccff, rgbcomponents);
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

	//set line width
        SoDrawStyle *sty = new SoDrawStyle;
        sty->style = SoDrawStyle::LINES;
        sty->lineWidth.setValue (m_trackThickness);
	sep->addChild (sty);

        if (!m_trajectoryPositions.empty () && !m_trajectoryDirections.empty ())
	{
	    std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_trajectoryPositions.begin ();
	    std::vector<std::vector<SbVec3f> >::const_iterator idir = m_trajectoryDirections.begin ();
	    for (unsigned int itrack = 0; itrack != m_trajectoryPositions.size (); ++itrack)
	    {
                if(m_selectedTrack.value()==-1 || m_selectedTrack.value()==(int)itrack){
		    IgSoSplineTrack* myTraj = new IgSoSplineTrack;
		    SoMFVec3f tangents;
		    SoMFVec3f points;
		    int nState = 0;
		    std::vector<SbVec3f>::const_iterator idd = (*idir).begin ();
		    for (std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin (); ipp != (*ipos).end (); ++ipp)
		    {
			points.set1Value (nState, (*ipp));
			tangents.set1Value (nState, (*idd));
			nState++;
			idd++;
		    }
		    myTraj->points = points;
		    myTraj->tangents = tangents;
		  
		    sep->addChild (myTraj);
		}
		ipos++;
		idir++;
	    }
        }
        rep->node ()->addChild (sep);
    }
}


void
VisTkRecTracksTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if (! m_tracks.empty ())
    {
        SoSeparator *sep = new SoSeparator;

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0x99ccff, rgbcomponents);
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

        //set line width
        SoDrawStyle *sty = new SoDrawStyle;
        sty->style = SoDrawStyle::LINES;
        sty->lineWidth.setValue (m_trackThickness);
	sep->addChild (sty);

        if (!m_trajectoryPositions.empty () && !m_trajectoryDirections.empty ())        
	{
	    std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_trajectoryPositions.begin ();
	    std::vector<std::vector<SbVec3f> >::const_iterator idir = m_trajectoryDirections.begin ();
	    for (unsigned int itrack = 0; itrack != m_trajectoryPositions.size(); ++itrack)
	    {
		if (m_selectedTrack.value()==-1 || m_selectedTrack.value()==(int)itrack)
		{
		    IgSoSplineTrack* myTraj = new IgSoSplineTrack;
		    SoMFVec3f tangents;
		    SoMFVec3f points;
		    int nState = 0;
		    std::vector<SbVec3f>::const_iterator idd = (*idir).begin ();
		    bool projectInSameHalf = (((*idir).front()[1] * (*idir).back()[1] < 0 ) || ( fabs((*idir).front()[1]) < 0.3 ));
		
		    bool projectLower = ((*ipos).back()[1] < 0);

		    for (std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin (); ipp != (*ipos).end (); ++ipp)
		    {
			float posy = sqrt((*ipp)[0]*(*ipp)[0]+(*ipp)[1]*(*ipp)[1]);
			float diry = sqrt((*idd)[0]*(*idd)[0]+(*idd)[1]*(*idd)[1]);
		    
			if (projectInSameHalf) 
			{
			    if (projectLower) 
			    { 
				posy = -posy;
				diry = -diry;
			    }
			} 
			else 
			{
			    if ((*ipp)[1] < 0) posy = -posy;
			    if ((*idd)[1] < 0) diry = -diry;
			}

			SbVec3f rzpos (0, posy, (*ipp)[2]);
			SbVec3f rzdir (0, diry, (*idd)[2]);
		    
			points.set1Value (nState, rzpos);
			tangents.set1Value (nState, rzdir);
			nState++;
			idd++;
		    }
		    myTraj->points = points;
		    myTraj->tangents = tangents;
		    sep->addChild (myTraj);
		}
		ipos++;
		idir++;
	    }
        }
        rep->node ()->addChild (sep);
    }
}

void
VisTkRecTracksTwig::update (IgLegoRep *rep)
{

    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    //........ still missing
}

