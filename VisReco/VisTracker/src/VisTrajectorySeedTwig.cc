//<<<<<< INCLUDES        
#include "VisReco/VisTracker/interface/VisTrajectorySeedTwig.h"                                     
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
 
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"

#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "Iguana/Inventor/interface/IgSoArrow.h"

#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <qstring.h>
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
	IgTwig *rootTwig = IgDocumentData::get (state)->root ();
	IgTwig *eventTwig = 0;
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Tracker Event/Trajectory Seeds");
	
	if (! eventTwig) 
	    eventTwig = parent;

	return new VisTrajectorySeedTwig (state, eventTwig, "[N/A] Seed (" + name + ")", 
					  friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisTrajectorySeedTwig::VisTrajectorySeedTwig  (IgState *state, IgTwig *parent,
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
      m_seedRescaleFactor (state, lat::CreateCallback (this, &VisTrajectorySeedTwig::twigChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID seedID (typeid (TrajectorySeedCollection));
    tfService->registerTwig (seedID.friendlyClassName (), &createThisTwig);
}

void
VisTrajectorySeedTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisTrajectorySeedTwig::onNewEvent (const edm::Event &event,
				   const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_hitPoints.clear ();
    m_seedPositions.clear ();
    m_seedDirections.clear ();

    m_text = (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ())
	      .arg (event.luminosityBlock ())
	      .arg (event.orbitNumber ())
	      .arg (event.bunchCrossing ())
	      .latin1 ());

    std::vector<edm::Handle<TrajectorySeedCollection> > seedCollections;
  
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, seedCollections);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTrajectorySeedTwig: ");
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

    try
    { 
        eventSetup.get<GlobalTrackingGeometryRecord> ().get (m_trackerGeometry);
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTrajectorySeedTwig: ");
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


    try
    { 
        eventSetup.get<IdealMagneticFieldRecord> ().get (m_mgField);
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTrajectorySeedTwig: ");
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
  
    if (! seedCollections.empty ())
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	if (m_trackerGeometry.isValid ()) 
	{
	    if (m_mgField.isValid ())
	    {
		LOG (2, trace, LFfwvis, "OK: seedCollections is not empty and GlobalTrackingGeometry is valid===>>>\n");

		std::vector<edm::Handle<TrajectorySeedCollection> >::iterator i;
		std::vector<edm::Handle<TrajectorySeedCollection> >::iterator end;
    
		LOG (2, trace, LFfwvis,  "There are " << seedCollections.size () << " seed collections.\n");

		for (i = seedCollections.begin (), end = seedCollections.end (); i != end; ++i)
		{
		    try 
		    {
			const TrajectorySeedCollection c = *(*i).product ();
			QString sizeStr = (QString ("%1").arg (c.size ()));
			QString nameStr = QString::fromStdString (this->name ());
			int ib = nameStr.find ("[");
			int ie = nameStr.find ("]");
			nameStr.replace (ib + 1, ie - 1, sizeStr);
			this->name (nameStr.toStdString ());
			
			LOG (2, trace, LFfwvis,  ":    seed collection has " << c.size () << " seeds\n");

			for (TrajectorySeedCollection::const_iterator seed = c.begin (), seedEnd = c.end (); seed != seedEnd; ++seed)
			{
			    LOG (2, trace, LFfwvis,  "\nSeed with rechitsize = " << (*seed).nHits () << "\n");
			    std::vector<SbVec3f> mySeed;
		    
			    int nRecHit = 1;
			    for (edm::OwnVector<TrackingRecHit>::const_iterator it = seed->recHits ().first, itEnd = seed->recHits ().second;  it != itEnd; ++it) 
			    {
				if ((*it).isValid () && !(*it).geographicalId ().null ()) 
				{
				    GlobalPoint gp = esService->globalPosition (&(*it));
				    float x = gp.x ();
				    float y = gp.y ();
				    float z = gp.z ();
				    
				    LOG (2, trace, LFfwvis,  "#" << nRecHit++ << ": " << "(" << x << ", " << y << ", " << z << ")" 
					 << " on module =" << (it)->geographicalId ().rawId () << "\n");
				    mySeed.push_back (SbVec3f (x, y, z));
				}
			    }
			    m_hitPoints.push_back (mySeed);

			    LOG (2, trace, LFfwvis,  "\nConvert TrajectoryState \n");

			    TrajectoryStateTransform tsTransform;

			    PTrajectoryStateOnDet pTSOD = seed->startingState ();
			    DetId seedDetId (pTSOD.detId ());

			    const GeomDet* gdet = esService->idToDet (seedDetId);

			    TrajectoryStateOnSurface tsos = tsTransform.transientState (pTSOD, &(gdet->surface ()), 
											&*m_mgField);

			    if (tsos.isValid ()) 
			    {
				float x = tsos.globalPosition ().x () / 100.0;
				float y = tsos.globalPosition ().y () / 100.0;
				float z = tsos.globalPosition ().z () / 100.0;

				float dirx = tsos.globalDirection ().x () / 100.0;
				float diry = tsos.globalDirection ().y () / 100.0;
				float dirz = tsos.globalDirection ().z () / 100.0;

				LOG (2, trace, LFfwvis,  "seed"<< "Pos: (" << x << ", " << y << ", " << z 
				     << "),\n Dir: (" << dirx << ", " << diry << ", " << dirz << ") ");
		
				m_seedPositions.push_back (SbVec3f (x, y, z));
				m_seedDirections.push_back (SbVec3f (dirx, diry, dirz));
			    }
			}
		    }
		    catch (cms::Exception& e)
		    {
			e.append (" from VisTrajectorySeedTwig: ");
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
	    }
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();

    if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (state ()))
        sbar->setMessage (QString::fromStdString (m_text));
}

void
VisTrajectorySeedTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
  
    // Prepare the property description.
    std::ostringstream text;
  
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
  
    text << "Trajectory Seed = ";
  
    try
    {    
        unsigned int seed_size = m_seedPositions.size ();
        text << std::setw (3) << seed_size;

	for (unsigned int nt = 0; nt < seed_size; ++nt)
	{
            double seed_pt = sqrt (m_seedDirections [nt][0] * m_seedDirections [nt][0] + m_seedDirections [nt][1] * m_seedDirections [nt][1]);

	    std::ostringstream out_pt;
	    out_pt << seed_pt; 
	    text << "<table width='100%' border=1>"
		 << "<TR align = center>"
		 << "<TD COLSPAN =4>Seed number = " << nt 
		 << " track pt = " << out_pt.str () 
		 << "</TD></TR><TR align = center>"
		 << "<TH>NumberRecHit</TH>"
		 << "<TH>GlobalPos</TH>"
		 << "</TR>";
      
	    int n_rechit = 0;
            if (nt < m_hitPoints.size ()) //FIXME
		for (std::vector<SbVec3f>::const_iterator ir = m_hitPoints[nt].begin (); ir != m_hitPoints[nt].end (); ++ir) 
		{
		    SbVec3f xyzPoint = (*ir);
		    float x = xyzPoint [0];
		    float y = xyzPoint [1];
		    float z = xyzPoint [2];
	  
		    text << "<TR align = right>"
			 << "<TD>" << std::setw (3) << n_rechit++<< "</TD>"
			 << "<TD>" << std::setw (6) << std::setprecision (3) << x << "," << y << "," << z << "</TD></TR>";
		}
	    text << "</table>";  
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTrajectorySeedTwig: ");
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
VisTrajectorySeedTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.  
    VisQueuedTwig::update (rep);
    rep->clear ();
  
    if (! m_seedPositions.empty ())
    {
	SoSeparator *sep = new SoSeparator;    
      
	SoMaterial *recomat = new SoMaterial;
	recomat->diffuseColor.setValue (0.6, 1.0, 0.0);
	sep->addChild (recomat);

        std::vector<SbVec3f>::const_iterator idir = m_seedDirections.begin ();

	for (std::vector<SbVec3f>::const_iterator i = m_seedPositions.begin (); i != m_seedPositions.end (); ++i) 
	{
	    IgSoArrow* seedArrow = new IgSoArrow;
	    seedArrow->tail.setValue (*i);

            float x1 ((*i)[0] + m_seedRescaleFactor.value () *(*idir)[0]);
            float y1 ((*i)[1] + m_seedRescaleFactor.value () *(*idir)[1]);
            float z1 ((*i)[2] + m_seedRescaleFactor.value () *(*idir)[2]);

            SbVec3f tipPos( SbVec3f(x1, y1, z1) );
	    seedArrow->tip.setValue (tipPos);
	    sep->addChild (seedArrow);
            idir++;
	}	   
	rep->node ()->addChild (sep);
    }
}

void
VisTrajectorySeedTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_seedPositions.empty ())
    {
        SoSeparator *sep = new SoSeparator;

        SoMaterial *recomat = new SoMaterial;
        recomat->diffuseColor.setValue (0.6, 1.0, 0.0);
        sep->addChild (recomat);

        std::vector<SbVec3f>::const_iterator idir = m_seedDirections.begin ();

        for (std::vector<SbVec3f>::const_iterator i = m_seedPositions.begin (); i != m_seedPositions.end (); ++i)
        {
            IgSoArrow* seedArrow = new IgSoArrow;
            seedArrow->tail.setValue (*i);

            float x1 ((*i)[0] + m_seedRescaleFactor.value () *(*idir)[0]);
            float y1 ((*i)[1] + m_seedRescaleFactor.value () *(*idir)[1]);
            float z1 ((*i)[2] + m_seedRescaleFactor.value () *(*idir)[2]);

            SbVec3f tipPos (SbVec3f (x1, y1, z1));
            seedArrow->tip.setValue (tipPos);
            sep->addChild (seedArrow);
            idir++;
        }
        rep->node ()->addChild (sep);
    }
}

void
VisTrajectorySeedTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_seedPositions.empty ())
    {
        SoSeparator *sep = new SoSeparator;

        SoMaterial *recomat = new SoMaterial;
        recomat->diffuseColor.setValue (0.6, 1.0, 0.0);
        sep->addChild (recomat);

        std::vector<SbVec3f>::const_iterator idir = m_seedDirections.begin ();

        for (std::vector<SbVec3f>::const_iterator i = m_seedPositions.begin (); i != m_seedPositions.end (); ++i)
        {
            IgSoArrow* seedArrow = new IgSoArrow;
	    float iyy = sqrt ((*i)[0]*(*i)[0] + (*i)[1]*(*i)[1]);
	    if ((*i)[1] < 0) iyy = -iyy;

            SbVec3f rzpos(0, iyy, (*i)[2]);

            seedArrow->tail.setValue (rzpos);

            float x1 ((*i)[0] + m_seedRescaleFactor.value () *(*idir)[0] );
            float y1 ((*i)[1] + m_seedRescaleFactor.value () *(*idir)[1] );
            float z1 ((*i)[2] + m_seedRescaleFactor.value () *(*idir)[2] );

            float irr = sqrt (x1*x1 + y1*y1);
	    if (y1 < 0) irr = -irr;
            SbVec3f tipPos (SbVec3f (0, irr, z1));
            seedArrow->tip.setValue (tipPos);
            sep->addChild (seedArrow);
            idir++; 
        }
        rep->node ()->addChild (sep);
    }
}

void
VisTrajectorySeedTwig::update (IgLegoRep *rep)
{

    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    //........ still missing
}

