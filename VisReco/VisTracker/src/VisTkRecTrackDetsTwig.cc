//<<<<<< INCLUDES        
#include "VisReco/VisTracker/interface/VisTkRecTrackDetsTwig.h"      
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"

#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"

#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"

#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
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
	return new VisTkRecTrackDetsTwig (state, parent, "Dets for reconstructed tracks (" + name + ")", 
					  friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisTkRecTrackDetsTwig::VisTkRecTrackDetsTwig  (IgState *state, IgTwig *parent,
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
      m_processName (processName)
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID trackID (typeid (reco::TrackCollection));
    tfService->registerTwig (trackID.friendlyClassName (), &createThisTwig);
    m_pset = VisConfigurationService::pSet ();
    if (! m_pset)
    {
	edm::ParameterSet p;
	VisConfigurationService *visService = new VisConfigurationService (p);
	m_pset = visService->pSet ();
	delete visService;
    } 

    std::vector<std::string> notSpecified;
    m_trackCollectionNames = m_pset->getUntrackedParameter<std::vector<std::string> > ("trackCollectionNames", notSpecified);
}


/** Clean the scene graph. */
VisTkRecTrackDetsTwig::~VisTkRecTrackDetsTwig (void) 
{
}

void
VisTkRecTrackDetsTwig::onNewEvent (const edm::Event &event,
				   const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_geomDetUnits.clear ();
    std::vector<DetId> trackIds;

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
                for (std::vector<std::string>::iterator tkColl = m_trackCollectionNames.begin (), tkCollEnd = m_trackCollectionNames.end ();
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
	e.append (" from VisTkRecTrackDetsTwig: ");
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
	e.append (" from VisTkRecTrackDetsTwig: ");
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
  
    if ((! trackCollections.empty ()) && m_trackerGeometry.isValid ())
    {
	LOG (2, trace, LFfwvis, "OK: trackCollections is not empty and TrackingGeometry is valid===>>>\n");

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
		for (reco::TrackCollection::const_iterator track = c.begin (), trackEnd = c.end (); track != trackEnd; ++track)
		{
		    if ((*track).extra ().isNonnull ())
		    {			
			LOG (3, trace, LFfwvis, "\nTrack with rechitsize = " << (*track).recHitsSize ()
			     << "\nCheck the validity of the RecHits:" << "\n");

			bool valid = true;		    
			for (trackingRecHit_iterator it = track->recHitsBegin (), itEnd = track->recHitsEnd ();  
			     it != itEnd; ++it)
			{
			    if ((*it)->geographicalId ().null ()) 
			    {
				LOG (3, trace, LFfwvis, "\nWARNING: RecHit is not valid.\n");
				valid = false;
			    }
			    else
			    {
				LOG (3, trace, LFfwvis,  "\nRecHit is OK."
				     << "\nPut id into trackIds.\n");

				trackIds.push_back ((*it)->geographicalId ());
			    }
			}
		    }
		    else
		    {
			LOG (3, trace, LFfwvis, "\nTrack has no Extras stored.\n");
		    }
		}
		
	    }
	    catch (cms::Exception& e)
	    {
		e.append (" from VisTkRecTrackDetsTwig: ");
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
    else
    {
	LOG (2, trace, LFfwvis, "Hmmm.. trackCollections is empty or GlobalTrackingGeometry is not valid.\n");
    }
    
    int nselected = 0;
    
    std::vector<DetId> cleanedDetIds;
    std::vector<DetId>::const_iterator iter;
    std::vector<DetId>::const_iterator jter;
    std::vector<DetId>::const_iterator iterEnd = trackIds.end ();

    try 
    {
	for (iter = trackIds.begin (); iter != iterEnd; ++iter) 
	{
	    bool hasDup = false;
	    for (jter = iter + 1; jter != iterEnd; ++jter) 
	    {
		if (*iter == *jter) hasDup = true;
	    }
	    if (! hasDup) cleanedDetIds.push_back (*iter);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisTkRecTrackDetsTwig: ");
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

    trackIds.clear ();
    trackIds.swap (cleanedDetIds);

    for (std::vector<DetId>::const_iterator detIdObject = trackIds.begin (); detIdObject != trackIds.end (); ++detIdObject) 
    {
	if (! (*detIdObject).null ()) 
	{		
	    const GeomDet *gd = 0;
	
	    unsigned int id = detIdObject->rawId ();
	    try 
	    {
                gd = dynamic_cast<const GeomDet *>(m_trackerGeometry->idToDet (*detIdObject));
	    }
	    catch (cms::Exception& e)
	    {
		e.append (" from VisTkRecTrackDetsTwig: ");
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
	    
	    if (gd != 0)
	    {
		std::vector<const GeomDet*> comp = gd->components ();

		if (comp.empty ())
		{
		    //case 1 - only 1 real module associated
		    const GeomDetUnit *  geoUnit = 0;
                    try
                    {
                        geoUnit = dynamic_cast<const GeomDetUnit *>(m_trackerGeometry->idToDetUnit (*detIdObject));
                    }
                    catch (cms::Exception& e)
                    {
                        if (this->m_onCmsException)
                        this->m_onCmsException (&e);
                    }

		    if (geoUnit != 0)
		    {
			// push back geoUnit
			m_geomDetUnits.push_back(geoUnit);
			nselected++;
		    }
		}
		if (comp.size () == 2)
		{
		    // case 2 - stereo pair associated
		    const GeomDetUnit *  geoUnit1 =
			dynamic_cast<const GeomDetUnit*> (comp [0]);
		    if (geoUnit1 != 0)
		    {
			// push back geoUnit1
			m_geomDetUnits.push_back (geoUnit1);
			nselected++;
		    }

		    const GeomDetUnit *  geoUnit2 =
			dynamic_cast<const GeomDetUnit*>(comp [1]);
		    if (geoUnit2 != 0)
		    {
			//push back geoUnit2
			m_geomDetUnits.push_back (geoUnit2);
			nselected++;
		    }
		}

		if (comp.size () == 1 || comp.size () > 2) 
		{
		    std::cout<<"module " << id << " intersected by rechit not found!"<<std::endl;
		}
	    } 
	    else 
	    {		
		std::cout<<"module " << id << " intersected by rechit not found!"<<std::endl;
	    }
	}
    }
    
    //    std::cout<<"size detid after unique "<<m_geomDetUnits.size()<<std::endl;

    VisQueuedTwig::onBaseInvalidate ();
}

void
VisTkRecTrackDetsTwig::update (IgTextRep *rep)
{
    // Get debugging dump.

}

void
VisTkRecTrackDetsTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.  
    VisQueuedTwig::update (rep);
    rep->clear ();

    if ((! m_geomDetUnits.empty ()) &&  m_trackerGeometry.isValid () )
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	SoSeparator* sep = esService->buildDet (VisEventSetupService::FullDet, m_geomDetUnits);
	if (sep != NULL)
	    rep->node ()->addChild (sep);
    }
}

void
VisTkRecTrackDetsTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
 
    if ((! m_geomDetUnits.empty ()) &&  m_trackerGeometry.isValid () )
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	SoSeparator* sep = esService->buildDet (VisEventSetupService::RPhiDet, m_geomDetUnits);

	sep->ref ();
	rep->node ()->addChild (sep);
    }
}

void
VisTkRecTrackDetsTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    if ((! m_geomDetUnits.empty ()) &&  m_trackerGeometry.isValid () )
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	SoSeparator* sep = esService->buildDet (VisEventSetupService::RZDet, m_geomDetUnits);

        sep->ref ();
	rep->node ()->addChild (sep);
    }

}


void
VisTkRecTrackDetsTwig::update (IgLegoRep *rep)
{

    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    //........ still missing
}

