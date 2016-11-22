//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisTracker/interface/VisGsfTrackTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoArrow.h"
#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include "Iguana/Inventor/interface/IgSoSplineTrack.h"
#include "Iguana/Inventor/interface/IgSoIdealTrack.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Tracker Event/Gsf Tracks");
	
	if (! eventTwig) 
	    eventTwig = parent;

	return new VisGsfTrackTwig (state, eventTwig, "[N/A] GsfTrack (" + name + ")", 
				    friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisGsfTrackTwig::VisGsfTrackTwig (IgState *state, IgTwig *parent,
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
      m_showDirection (state, lat::CreateCallback (this, &VisGsfTrackTwig::twigChanged)),
      m_shape (state, lat::CreateCallback (this, &VisGsfTrackTwig::twigChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID trackID (typeid (reco::GsfTrackCollection));
    tfService->registerTwig (trackID.friendlyClassName (), &createThisTwig);   
}

void
VisGsfTrackTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }


void
VisGsfTrackTwig::onNewEvent (const edm::Event &event,
			     const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_text = QString ("Run # %1, event # %2")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ()).latin1 ();
    m_tracks.clear ();
    m_trajectoryPositions.clear ();
    m_trajectoryDirections.clear ();
    
    edm::Handle<reco::GsfTrackCollection> collections;
    try 
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.get (visSel, collections);
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
    try 
    {
	if (collections.isValid ())
	{
	    QString sizeStr = (QString ("%1").arg (collections->size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    this->name (nameStr.toStdString ());

	    for (reco::GsfTrackCollection::const_iterator track = collections->begin (), trackEnd = collections->end (); 
		 track != trackEnd; ++track)
	    {
		m_tracks.push_back (*track);
	    
		if ((*track).gsfExtra ().isNonnull ())
		{		
		    std::vector<reco::GsfTrackExtra::LocalParameterVector> islp = (*track).gsfExtra ()->innerStateLocalParameters ();
		    std::vector<reco::GsfTrackExtra::LocalParameterVector> oslp = (*track).gsfExtra ()->outerStateLocalParameters ();

		    LOG (2, trace, LFfwvis, "GsfTrackExtra: Inner State Local Parameters:\n");
	    
		    for (std::vector<reco::GsfTrackExtra::LocalParameterVector>::const_iterator ii = islp.begin (), iiEnd = islp.end ();
			 ii != iiEnd; ++ii)
		    {
			std::cout << (*ii) << std::endl;
		    }

		    std::cout << "GsfTrackExtra: Outer State Local Parameters:" << std::endl;
	    
		    for (std::vector<reco::GsfTrackExtra::LocalParameterVector>::const_iterator io = oslp.begin (), ioEnd = oslp.end ();
			 io != ioEnd; ++io)
		    {
			std::cout << (*io) << std::endl;
		    }
		    std::cout << "GsfTrackExtra: Sign of local P_z at inner state: " << (*track).gsfExtra ()->innerStateLocalPzSign () << std::endl;
		    std::cout << "GsfTrackExtra: Sign of local P_z at outermost state: " << (*track).gsfExtra ()->outerStateLocalPzSign () << std::endl;
		}
	    
		std::vector<SbVec3f> myTraPos;
		std::vector<SbVec3f> myTraDir;
	    
		std::cout << "Innermost state is ";

		(*track).innerOk () ? (std::cout << "OK") : (std::cout << "not OK");
		std::cout << std::endl;

		std::cout << "Outermost state is ";
		(*track).outerOk () ? (std::cout << "OK") : (std::cout << "not OK");
		std::cout << std::endl;

		if ((*track).innerOk () && (*track).outerOk ())
		{
		    float x = (*track).innerPosition ().x () / 100.0;
		    float y = (*track).innerPosition ().y () / 100.0;
		    float z = (*track).innerPosition ().z () / 100.0;

		    float dirx = (*track).innerMomentum ().x ();
		    float diry = (*track).innerMomentum ().y ();
		    float dirz = (*track).innerMomentum ().z ();
		    SbVec3f diri (dirx, diry, dirz);
		    diri.normalize ();
		
		    myTraPos.push_back (SbVec3f (x, y, z));
		    myTraDir.push_back (diri);

		    x = (*track).outerPosition ().x () / 100.0;
		    y = (*track).outerPosition ().y () / 100.0;
		    z = (*track).outerPosition ().z () / 100.0;

		    dirx = (*track).outerMomentum ().x ();
		    diry = (*track).outerMomentum ().y ();
		    dirz = (*track).outerMomentum ().z ();
		    SbVec3f diro (dirx, diry, dirz);
		    diro.normalize ();

		    myTraPos.push_back (SbVec3f (x, y, z));
		    myTraDir.push_back (diro);
		}
		m_trajectoryPositions.push_back (myTraPos);
		m_trajectoryDirections.push_back (myTraDir);
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

    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisGsfTrackTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;

    text << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>P<SUB>T (GeV)</TH>"
	 << "<TH>Eta</TH>"
	 << "<TH>Phi</TH>"
	 << "<TH>Charge</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
    
    int n = 0;
    if (! m_tracks.empty ()) 
    {
	try 
	{
	    for (std::vector<reco::GsfTrack>::const_iterator it = m_tracks.begin (), itEnd = m_tracks.end (); it != itEnd; ++it) 
	    {
		double pt = (*it).pt ();	    
	
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << n++ << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << pt << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*it).eta () << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*it).phi () << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*it).charge () << "</TD>"
		     << "</TR>";
	    }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisGsfTrackTwig: ");
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
       
    text << "</table>";
    
    // Send it over.
    rep->setText (text.str ());
}

void
VisGsfTrackTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
 
    SoSeparator *contents = new SoSeparator;
    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0xCD00CC00, rgbcomponents);
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    contents->addChild (mat);

    //set line width
    SoDrawStyle *sty = new SoDrawStyle;
    sty->style = SoDrawStyle::LINES;
    sty->lineWidth.setValue (4);
    contents->addChild (sty);

    if (!m_trajectoryPositions.empty () && !m_trajectoryDirections.empty ())
    {
	std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_trajectoryPositions.begin ();
	std::vector<std::vector<SbVec3f> >::const_iterator idir = m_trajectoryDirections.begin ();
	for (unsigned int itrack = 0; itrack != m_trajectoryPositions.size (); ++itrack)
	{
	    IgSoSimpleTrajectory *obj = new IgSoSimpleTrajectory;
	    IgSoSplineTrack* myTraj = new IgSoSplineTrack;
	    SoMFVec3f tangents;
	    SoMFVec3f points;
	    int nState = 0;
	    std::vector<SbVec3f>::const_iterator idd = (*idir).begin ();
	    for (std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin (); ipp != (*ipos).end (); ++ipp)
	    {
		obj->controlPoints.set1Value (nState, (*ipp));
		obj->markerPoints.set1Value (nState, (*ipp));

		points.set1Value (nState, (*ipp));
		tangents.set1Value (nState, (*idd));

		if (m_showDirection.value ()) 
		{
		    SoSeparator *directions = new SoSeparator;
		    contents->addChild (directions);
		    
		    SoMaterial *dirMat = new SoMaterial;
		    IgSbColorMap::unpack (0x03C03C00, rgbcomponents);
		    dirMat->diffuseColor.setValue (SbColor (rgbcomponents));
		    directions->addChild (dirMat);
		    
		    IgSoArrow* dir = new IgSoArrow;
		    dir->tail.setValue (SbVec3f ((*ipp)[0], (*ipp)[1], (*ipp)[2]));
		    dir->tip.setValue (SbVec3f ((*ipp)[0] + (*idd)[0] / 2.0, (*ipp)[1] + (*idd)[1] / 2.0, (*ipp)[2] + (*idd)[2] / 2.0));
		    directions->addChild (dir);
		}
		
		nState++;
		idd++;
	    }
	    obj->linePattern = 0xdfff;
	    obj->lineWidth = 4.0;
	    
	    myTraj->points = points;
	    myTraj->tangents = tangents;

	    if (m_shape.value () == "spline") 
	    {
		contents->addChild (myTraj);
	    } 
	    if (m_shape.value () == "line")
	    {
		contents->addChild (obj);
	    }
	    if (m_shape.value () == "both") 
	    {
		contents->addChild (myTraj);
		contents->addChild (obj);
	    }
	    ipos++;
	    idir++;
	}
    }

    rep->node ()->addChild (contents);
}

void
VisGsfTrackTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *contents = new SoSeparator;

    rep->node ()->addChild (contents);
}

void
VisGsfTrackTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *contents = new SoSeparator;
    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0xCD00CC00, rgbcomponents);
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    contents->addChild (mat);

    //set line width
    SoDrawStyle *sty = new SoDrawStyle;
    sty->style = SoDrawStyle::LINES;
    sty->lineWidth.setValue (4);
    contents->addChild (sty);

    if (!m_trajectoryPositions.empty () && !m_trajectoryDirections.empty ())
    {
	std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_trajectoryPositions.begin ();
	std::vector<std::vector<SbVec3f> >::const_iterator idir = m_trajectoryDirections.begin ();
	for (unsigned int itrack = 0; itrack != m_trajectoryPositions.size (); ++itrack)
	{
	    IgSoSimpleTrajectory *obj = new IgSoSimpleTrajectory;
	    IgSoSplineTrack* myTraj = new IgSoSplineTrack;
	    SoMFVec3f tangents;
	    SoMFVec3f points;
	    int nState = 0;
	    float zlayer = 0.1;
	    std::vector<SbVec3f>::const_iterator idd = (*idir).begin ();
	    for (std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin (); ipp != (*ipos).end (); ++ipp)
	    {
		SbVec3f rphipos((*ipp)[0], (*ipp)[1], zlayer);
		SbVec3f rphidir((*idd)[0], (*idd)[1], zlayer);
		rphidir.normalize ();
		
		obj->controlPoints.set1Value (nState, rphipos);
		obj->markerPoints.set1Value (nState, rphipos);

		if (m_showDirection.value ()) 
		{
		    SoSeparator *directions = new SoSeparator;
		    contents->addChild (directions);
		    
		    SoMaterial *dirMat = new SoMaterial;
		    IgSbColorMap::unpack (0x03C03C00, rgbcomponents);
		    dirMat->ambientColor.setValue (SbColor (rgbcomponents));
		    dirMat->diffuseColor.setValue (SbColor (rgbcomponents));
		    dirMat->specularColor.setValue (SbColor (rgbcomponents));
		    dirMat->emissiveColor.setValue (SbColor (rgbcomponents));
		    directions->addChild (dirMat);
		    
		    IgSoArrow* dir = new IgSoArrow;
		    dir->tail.setValue (SbVec3f (rphipos[0], rphipos[1], zlayer));
		    dir->tip.setValue (SbVec3f (rphipos[0] + rphidir[0] / 2.0, rphipos[1] + rphidir[1] / 2.0, zlayer));
		    directions->addChild (dir);
		}

		points.set1Value (nState, (*ipp));
		tangents.set1Value (nState, (*idd));

		nState++;
		++idd;
	    }
	    obj->linePattern = 0xffff;
	    obj->lineWidth = 4.0;

	    myTraj->points = points;
	    myTraj->tangents = tangents;

	    if (m_shape.value ().compare ("spline") == 0) 
	    {
		contents->addChild (myTraj);
	    } 
	    if (m_shape.value ().compare ("line") == 0)
	    {
		contents->addChild (obj);
	    }
	    if (m_shape.value ().compare ("both") == 0) 
	    {
		contents->addChild (myTraj);
		contents->addChild (obj);
	    }
	    ipos++;
	    idir++;
	}
    }

    rep->node ()->addChild (contents);
}

void
VisGsfTrackTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *contents = new SoSeparator;
    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0xCD00CC00, rgbcomponents);
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    contents->addChild (mat);

    //set line width
    SoDrawStyle *sty = new SoDrawStyle;
    sty->style = SoDrawStyle::LINES;
    sty->lineWidth.setValue (4);
    contents->addChild (sty);
      
    if (!m_trajectoryPositions.empty () && !m_trajectoryDirections.empty ())        
    {
	std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_trajectoryPositions.begin ();
	std::vector<std::vector<SbVec3f> >::const_iterator idir = m_trajectoryDirections.begin ();
	for (unsigned int itrack = 0; itrack != m_trajectoryPositions.size(); ++itrack)
	{
	    IgSoSimpleTrajectory *obj = new IgSoSimpleTrajectory;
	    IgSoSplineTrack* myTraj = new IgSoSplineTrack;
	    SoMFVec3f tangents;
	    SoMFVec3f points;
	    int nState = 0;
	    float xlayer = -0.1;

	    std::vector<SbVec3f>::const_iterator idd = (*idir).begin ();
	    for (std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin (); ipp != (*ipos).end (); ++ipp)
	    {
		float posy = sqrt((*ipp)[0]*(*ipp)[0]+(*ipp)[1]*(*ipp)[1]);
		if ( (*ipp)[1] < 0 ) posy = -posy;

		float diry = sqrt((*idd)[0]*(*idd)[0]+(*idd)[1]*(*idd)[1]);
		if ( (*idd)[1] < 0 ) diry = -diry;

		SbVec3f rzpos(xlayer, posy, (*ipp)[2]);
		SbVec3f rzdir(xlayer, diry, (*idd)[2]);

		obj->controlPoints.set1Value (nState, rzpos);
		obj->markerPoints.set1Value (nState, rzpos);

		if (m_showDirection.value ()) 
		{
		    SoSeparator *directions = new SoSeparator;
		    contents->addChild (directions);
		    
		    SoMaterial *dirMat = new SoMaterial;
		    IgSbColorMap::unpack (0x03C03C00, rgbcomponents);
		    dirMat->ambientColor.setValue (SbColor (rgbcomponents));
		    dirMat->diffuseColor.setValue (SbColor (rgbcomponents));
		    dirMat->specularColor.setValue (SbColor (rgbcomponents));
		    dirMat->emissiveColor.setValue (SbColor (rgbcomponents));
		    directions->addChild (dirMat);
		    
		    IgSoArrow* dir = new IgSoArrow;
		    dir->tail.setValue (SbVec3f (xlayer, rzpos[1], rzpos[2]));
		    dir->tip.setValue (SbVec3f (xlayer, rzpos[1] + rzdir[1] / 2.0, rzpos[2] + rzdir[2] / 2.0));
		    directions->addChild (dir);
		}

		points.set1Value (nState, (*ipp));
		tangents.set1Value (nState, (*idd));

		nState++;
		idd++;
	    }
	    obj->linePattern = 0xffff;
	    obj->lineWidth = 4.0;
	    
	    myTraj->points = points;
	    myTraj->tangents = tangents;

	    if (m_shape.value ().compare ("spline") == 0) 
	    {
		contents->addChild (myTraj);
	    } 
	    if (m_shape.value ().compare ("line") == 0)
	    {
		contents->addChild (obj);
	    }
	    if (m_shape.value ().compare ("both") == 0) 
	    {
		contents->addChild (myTraj);
		contents->addChild (obj);
	    }
	    ipos++;
	    idir++;
	}
    }

    rep->node ()->addChild (contents);
}
