//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/GeometrySurface/interface/PlaneBuilder.h"
#include "DataFormats/GeometrySurface/interface/RectangularPlaneBounds.h"
#include "DataFormats/GeometrySurface/interface/TrapezoidalPlaneBounds.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoCrystalHit.h"
#include "Iguana/Inventor/interface/IgSoG4Trap.h"
#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <QApplication>
#include <QString>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <boost/lexical_cast.hpp>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (VisEventSetupService, "Services/Framework/Visualisation Event Setup");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisEventSetupService::VisEventSetupService (IgState *state)
    : VisEventObserver (state),
      m_state (state),
      m_phiDelta (state, lat::CreateCallback (this, &VisEventSetupService::invalidate)),
      m_phiStart (state, lat::CreateCallback (this, &VisEventSetupService::invalidate)),
      m_globalTrackingGeomCanged (true),
      m_trackerGeomCanged (true),
      m_caloGeomCanged (true),
      m_muonGeomCanged (true),
      m_fieldGeomCanged (true)
{    
    ASSERT (state);
    state->put (s_key, this);
}

VisEventSetupService::~VisEventSetupService (void)
{
    ASSERT (m_state);
    m_state->detach (s_key);
}

void
VisEventSetupService::onNewEvent (const edm::Event &event,
				  const edm::EventSetup &eventSetup)
{
    m_run = event.id ().run ();
    m_event = event.id ().event ();
    m_ls = event.luminosityBlock ();
    m_orbit = event.orbitNumber ();
    m_bx = event.bunchCrossing ();

    QApplication::setOverrideCursor (Qt::waitCursor);
    
    std::vector<std::string> ess (6);
    ess.push_back ("GlobalTrackingGeometry");
    ess.push_back ("CaloGeometry");
    ess.push_back ("CSCGeometry");
    ess.push_back ("DTGeometry");
    ess.push_back ("RPCGeometry");
    ess.push_back ("TrackerGeometry");
    ess.push_back ("MagneticField");

    const edm::ParameterSet *pset = VisConfigurationService::pSet ();
    if (pset)
    {
	ess = pset->getUntrackedParameter<std::vector<std::string> > ("VisEventSetup", ess);
    }

    if (std::find (ess.begin (), ess.end (), "GlobalTrackingGeometry") != ess.end ()) 
    {
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("CMSSW is retrieving GlobalTrackingGeometryRecord...");
	eventSetup.get<GlobalTrackingGeometryRecord> ().get (m_globalTrackingGeom);
	m_globalTrackingGeomCanged = m_watch_globalTrackingGeom.check (eventSetup);
    }
    if (std::find (ess.begin (), ess.end (), "TrackerGeometry") != ess.end ()) 
    {    
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("CMSSW is retrieving TrackerDigiGeometryRecord...");
	eventSetup.get<TrackerDigiGeometryRecord> ().get (m_trackerGeom);
	m_trackerGeomCanged = m_watch_trackerGeom.check (eventSetup);
    }
    if (std::find (ess.begin (), ess.end (), "CaloGeometry") != ess.end ()) 
    {    
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("CMSSW is retrieving CaloGeometryRecord...");
	eventSetup.get<CaloGeometryRecord> ().get (m_caloGeom);
	m_caloGeomCanged = m_watch_caloGeom.check (eventSetup);
    }
    if (std::find (ess.begin (), ess.end (), "CSCGeometry") != ess.end ()) 
    {    
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("CMSSW is retrieving MuonGeometryRecord...");
	eventSetup.get<MuonGeometryRecord> ().get (m_cscGeom);
	m_muonGeomCanged = m_watch_muonGeom.check (eventSetup);
    }
    if (std::find (ess.begin (), ess.end (), "DTGeometry") != ess.end ()) 
    {    
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("CMSSW is retrieving MuonGeometryRecord...");
	eventSetup.get<MuonGeometryRecord> ().get (m_dtGeom);
	m_muonGeomCanged = m_watch_muonGeom.check (eventSetup);
    }
    if (std::find (ess.begin (), ess.end (), "RPCGeometry") != ess.end ()) 
    {
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("CMSSW is retrieving MuonGeometryRecord...");
	eventSetup.get<MuonGeometryRecord> ().get (m_rpcGeom);
	m_muonGeomCanged = m_watch_muonGeom.check (eventSetup);
    }
    if (std::find (ess.begin (), ess.end (), "MagneticField") != ess.end ()) 
    {
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("CMSSW is retrieving IdealMagneticFieldRecord...");
	eventSetup.get<IdealMagneticFieldRecord> ().get (m_field);
	m_fieldGeomCanged = m_watch_field.check (eventSetup);
    }

    if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	sbar->setMessage ("CMSSW has completed building EventSetup.");
    if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			  .arg (m_run)
			  .arg (m_event)
			  .arg (m_ls)
			  .arg (m_orbit)
			  .arg (m_bx)
			  .latin1 ());

    QApplication::restoreOverrideCursor ();
}

const std::string
VisEventSetupService::cscTextRep (int chamber, int ring, int station, int endcap)
{
    std::ostringstream text;
    std::string txtString;
    
    if (m_cscGeom.isValid ())
    {
	std::vector<CSCChamber *> vc = m_cscGeom->chambers ();

	int icount = 0;
	
	for (std::vector<CSCChamber *>::const_iterator it = vc.begin (), end = vc.end (); 
	     it != end; ++it)
	{
	    const CSCChamber *cscChamber = *it;
	
	    if (cscChamber)
	    {		
		DetId detId = cscChamber->geographicalId ();
		int id = detId ();

		bool draw = true;

		(chamber == 0 || chamber == CSCDetId::chamber(id)) ? draw = true : draw = false;
		((ring == 0 || ring == CSCDetId::ring(id)) && draw) ? draw = true : draw = false;
		((station == 0 || station == CSCDetId::station(id)) && draw) ? draw = true : draw = false;
		((endcap == 0 || endcap == CSCDetId::endcap(id)) && draw) ? draw = true : draw = false;

		if (draw) 
		{ 
		    const BoundSurface &bSurface = cscChamber->surface ();

		    LocalPoint  lCentre (0.0, 0.0, 0.0);
		    GlobalPoint gCentre = bSurface.toGlobal (lCentre);

		    text << "<TR align = right>"
			 << "<TD>" << std::setw (4) << ++icount << "</TD>"
			 << "<TD>" << std::setw (12) << id << "</TD>"
			 << "<TD>" << std::hex << std::setw (12) << id << "</TD>" 
			 << "<TD>" << std::dec << std::setw (4)  << 
			    "E" << CSCDetId::endcap (id) << 
			    " S" << CSCDetId::station (id) << 
			    " R" << CSCDetId::ring (id) << 
			    " C" << std::setw (2) << CSCDetId::chamber (id) << std::setw (4) << "</TD>"
			 << "<TD>" << std::setprecision (3) << gCentre.x () << ", " << gCentre.y () << ", " << gCentre.z () 
			 << "</TD></TR>";
		}
	    }
	}
	txtString = text.str ();
    }
    else
    {
	txtString = "<TR align = right><TD COLSPAN = 5>CSC geometry is not valid: Have you asked for an event?</TD></TR>";
    }
    
    return txtString;
}

const std::string
VisEventSetupService::dtTextRep (int station, int sector, int wheel)
{
    std::ostringstream text;
    std::string txtString;
    
    if (m_dtGeom.isValid ())
    {
	std::vector<DTChamber *> vc = m_dtGeom->chambers ();

	int icount = 0;
	
	for (std::vector<DTChamber *>::const_iterator it = vc.begin (), end = vc.end (); 
	     it != end; ++it)
	{
	    const DTChamber *chamber = *it;
	
	    if (chamber)
	    {		
		DTChamberId chId = chamber->id();
		bool draw = true;

		((station == 0 || station == chId.station()) && draw) ? draw = true : draw = false;
		((sector == 0 || sector == chId.sector()) && draw) ? draw = true : draw = false;
		((wheel == 10 || wheel == chId.wheel()) && draw) ? draw = true : draw = false;

		if (draw) 
		{ 
		    const BoundSurface &bSurface = chamber->surface ();

		    LocalPoint  lCentre (0.0, 0.0, 0.0);
		    GlobalPoint gCentre = bSurface.toGlobal (lCentre);

		    text << "<TR align = right>"
			 << "<TD>" << std::setw (4) << ++icount << "</TD>"
			 << "<TD>" << std::dec << std::setw (4)
			 << "WHE"  << chId.wheel()
			 << " STA" << chId.station()
			 << " SEC" << chId.sector() << std::setw (4) << "</TD>"
			 << "<TD>" << std::setprecision (3) << gCentre.x () << ", " << gCentre.y () << ", " << gCentre.z () 
			 << "</TD></TR>";
		}
	    }
	}
	txtString = text.str ();
    }
    else
    {
	txtString = "<TR align = right><TD COLSPAN = 3>DT geometry is not valid: Have you asked for an event?</TD></TR>";
    }
    
    return txtString;
}

const std::string
VisEventSetupService::rpcTextRep (int region, int wheel, int station, int sector)
{
    std::ostringstream text;
    std::string txtString;
    
    if (m_rpcGeom.isValid ())
    {	
	std::vector<RPCRoll *> vc = m_rpcGeom->rolls ();
      
	int icount = 0;
      
	for (std::vector<RPCRoll *>::const_iterator it = vc.begin (), end = vc.end (); 
	     it != end; ++it)
	{
	    const RPCRoll *roll = *it;

	    if (roll)
	    {
		RPCDetId chId = roll->id ();
		if (chId)
		{
		    bool draw = true;

		    ((station == 30 || station == chId.station()) && draw) ? draw = true : draw = false;
		    ((sector == 30 || sector == chId.sector()) && draw) ? draw = true : draw = false;
		    ((wheel == 10 || wheel == chId.ring()) && draw) ? draw = true : draw = false;
		    ((region == 10 || region == chId.region ()) && draw) ? draw = true : draw = false;		    

		    if (draw) 
		    { 
			const BoundSurface &bSurface = roll->surface ();
		  
			LocalPoint  lCentre (0.0, 0.0, 0.0);
			GlobalPoint gCentre = bSurface.toGlobal (lCentre);
		  
			text << "<TR align = right>"
			     << "<TD>" << std::setw (4) << ++icount << "</TD>" 
			     << "<TD>" << std::dec << std::setw (4)  << 
			    "RGN "  << chId.region() << 
			    "; WHE "  << chId.ring() << 
			    "; STA " << chId.station() << 
			    "; SEC " << chId.sector() << std::setw (4) << "</TD>"
		    
			     << "<TD>" << std::setprecision (3) << gCentre.x () << ", " << gCentre.y () << ", " << gCentre.z () 
			     << "</TD></TR>";
		    }
		}
	    }
	}
	txtString = text.str ();
    }  
    else
    {
	txtString = "<TR align = right><TD COLSPAN = 3>RPC geometry is not valid: Have you asked for an event?</TD></TR>";
    }
    
    return txtString;
}

const std::string
VisEventSetupService::trackerTextRep (const std::string name)
{
    std::ostringstream text;
    std::string txtString;

    if (m_trackerGeom.isValid ())
    {
	QApplication::setOverrideCursor (Qt::waitCursor);
	    
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Constructing Tracker Geometry Table for %1...")
			      .arg (QString::fromStdString (name)));
    
	TrackerGeometry::DetContainer::const_iterator it;
	TrackerGeometry::DetContainer::const_iterator end;

	int icount = 0;

	if (name == "Tracker") 
	{	    
	    it = m_trackerGeom->dets ().begin ();
	    end = m_trackerGeom->dets ().end ();
	}
	else
	{	    
	    switch (subDetector (name))
	    {
	    case GeomDetEnumerators::PixelBarrel:
		it = m_trackerGeom->detsPXB ().begin ();
		end = m_trackerGeom->detsPXB ().end ();	    
		break;
	    case GeomDetEnumerators::PixelEndcap:
		it = m_trackerGeom->detsPXF ().begin ();
		end = m_trackerGeom->detsPXF ().end ();	    
		break;
	    case GeomDetEnumerators::TIB:
		it = m_trackerGeom->detsTIB ().begin ();
		end = m_trackerGeom->detsTIB ().end ();	    
		break;
	    case GeomDetEnumerators::TOB:
		it = m_trackerGeom->detsTOB ().begin ();
		end = m_trackerGeom->detsTOB ().end ();	    
		break;
	    case GeomDetEnumerators::TID:
		it = m_trackerGeom->detsTID ().begin ();
		end = m_trackerGeom->detsTID ().end ();	    
		break;
	    case GeomDetEnumerators::TEC:
		it = m_trackerGeom->detsTEC ().begin ();
		end = m_trackerGeom->detsTEC ().end ();	    
		break;
	    default:
		break;
	    }
	}
	
	for (; it != end; ++it)
	{
	    Surface::GlobalPoint pos =  (**it).surface ().position ();
	    text << "<TR align = right>"
		 << "<TD>" << std::setw (4) << ++icount << "</TD>" ;
	    
	    text << "<TD>" << (**it).geographicalId ().rawId () << "</TD>" 
		 << "<TD>" << pos.x () << ", " << pos.y () << ", " << pos.z () << "</TD></TR>";
	}
	txtString = text.str ();
    }
    else 
    {
	txtString = "<TR align = right><TD COLSPAN = 3>Tracker geometry is not valid: Have you asked for an event?</TD></TR>";
    }

    if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	sbar->setMessage ("Tracker Geometry Table Done.");
    if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			  .arg (m_run)
			  .arg (m_event)
			  .arg (m_ls)
			  .arg (m_orbit)
			  .arg (m_bx)
			  .latin1 ());

    QApplication::restoreOverrideCursor ();

    return txtString;
}

const std::string
VisEventSetupService::caloTextRep (const std::string name, const int subdetn)
{
    std::ostringstream text;
    std::string txtString;

    if (m_caloGeom.isValid () && (subdetn != -1))
    {
	QApplication::setOverrideCursor (Qt::waitCursor);
	    
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Constructing Calo Geometry Table for %1...").arg (QString::fromStdString (name)));

	const CaloSubdetectorGeometry *geom = (*m_caloGeom).getSubdetectorGeometry (detectorId (name), subdetn);
	const std::vector<DetId>& ids (geom->getValidDetIds (detectorId (name), subdetn));

	text << "<TR align = right><TD COLSPAN = 3>";	
	text << " has " << ids.size () << " detectors.</TD>";
	
	txtString = text.str ();
    }
    else 
    {
	txtString = "<TR align = right><TD COLSPAN = 3>Calo geometry is not valid: Have you asked for an event?</TD></TR>";
    }
	
    if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	sbar->setMessage ("Calo Geometry Table Done.");
    if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			  .arg (m_run)
			  .arg (m_event)
			  .arg (m_ls)
			  .arg (m_orbit)
			  .arg (m_bx)
			  .latin1 ());

    QApplication::restoreOverrideCursor ();

    return txtString; 
}

const GlobalPoint 
VisEventSetupService::toGlobal (const LocalPoint point, const DetId detId)
{
    return m_globalTrackingGeom.product ()->idToDetUnit (detId)->surface ().toGlobal (point);
}

const GeomDet*
VisEventSetupService::idToDet (const DetId det)
{
    return m_globalTrackingGeom.product ()->idToDet (det);
}

const GlobalPoint
VisEventSetupService::globalPosition (const TrackingRecHit *rechit)
{
    if (m_globalTrackingGeom.isValid ())
	return m_globalTrackingGeom.product ()->idToDet (rechit->geographicalId ())->surface ().toGlobal (this->localPosition (rechit));
    else
	return GlobalPoint ();
}

const LocalPoint
VisEventSetupService::localPosition (const TrackingRecHit *rechit)
{
    if (m_globalTrackingGeom.isValid ())
    {
	const int kBPIX = PixelSubdetector::PixelBarrel;
	const int kFPIX = PixelSubdetector::PixelEndcap;

	if (rechit->geographicalId ().det () == DetId::Tracker && 
	    rechit->geographicalId ().subdetId () != kBPIX && 
	    rechit->geographicalId().subdetId () != kFPIX)
	{
	    std::vector<LocalPoint> points;
	    this->localPositions (rechit, points);
	    if (points.size () == 1)
		return points.front ();
	    else
	    {
		//this is not really nice, but that's all we can do there.
		const SiStripMatchedRecHit2D *matched = dynamic_cast<const SiStripMatchedRecHit2D *>(rechit);
		if (matched)
		{
		    GlobalPoint pos_1 = m_globalTrackingGeom.product ()->idToDetUnit (matched->monoHit ()->geographicalId ())->surface ().toGlobal (points [0]);
		    GlobalPoint pos_2 = m_globalTrackingGeom.product ()->idToDetUnit (matched->stereoHit ()->geographicalId ())->surface ().toGlobal (points [1]);
		    GlobalPoint average ((pos_1.x () + pos_2.x ()) / 2,
					 (pos_1.y () + pos_2.y ()) / 2,
					 (pos_1.z () + pos_2.z ()) / 2);      
		    return m_globalTrackingGeom.product ()->idToDet (rechit->geographicalId ())->surface ().toLocal (average);
		}
		else return LocalPoint ();
	    }
	}
	else
	{
	    return rechit->localPosition ();
	}
    }
    else 
	return LocalPoint ();
}

void
VisEventSetupService::localPositions (const TrackingRecHit *rechit, std::vector<LocalPoint> &points)
{
    const int kBPIX = PixelSubdetector::PixelBarrel;
    const int kFPIX = PixelSubdetector::PixelEndcap;

    if (rechit->geographicalId ().det () == DetId::Tracker && 
	rechit->geographicalId ().subdetId () != kBPIX && 
	rechit->geographicalId ().subdetId () != kFPIX)
    {
	const RecHit2DLocalPos *rechit2D = dynamic_cast<const RecHit2DLocalPos *> (rechit);
	const SiStripRecHit2D *single = dynamic_cast<const SiStripRecHit2D *> (rechit);
	//matched or single rechits
	if (single)
	{
	    DetId detectorId = rechit2D->geographicalId ();
    
	    const SiStripCluster *Cluster = 0;
	    if (single->cluster ().isNonnull ())
		Cluster = single->cluster ().get ();
	    else if (single->cluster_regional ().isNonnull ())
		Cluster = single->cluster_regional ().get ();
	    else 
		points.push_back (LocalPoint ());
	    const StripTopology  *topology = dynamic_cast<const StripTopology *> (&(m_globalTrackingGeom.product ()->idToDetUnit (detectorId)->topology ()));
	    ASSERT(topology);
			
	    points.push_back (topology->localPosition (Cluster->barycenter ()));
	}
	else
	{
	    const SiStripMatchedRecHit2D *matched = dynamic_cast<const SiStripMatchedRecHit2D *> (rechit);
	    if (matched)
	    {
		this->localPositions (matched->monoHit (), points);
		this->localPositions (matched->stereoHit (), points);
	    }
	}
    }   
    else
    {
	points.push_back (rechit->localPosition());
    }
}

float
VisEventSetupService::phi4eta (float eta) const
{
    float phi = 0.087;
    float etaBins [29] = { 0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609,
			   0.696, 0.783, 0.87, 0.957, 1.044, 1.131, 1.218, 1.305,
			   1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043,
			   2.172, 2.322, 2.5, 2.65, 3};
    
    float etaHFBins [14] = {2.853, 2.964, 3.139, 3.314, // HF
			    3.486, 3.664, 3.839, 4.013, 
			    4.191, 4.363, 4.538, 4.716, 
			    4.889, 5.191};
    if (eta > 2.853) 
	for (int i = 0; i < 13; i++) 
	{
	    if ((eta > etaHFBins [i]) && (eta < etaHFBins [i + 1])) 
		phi =  etaHFBins [i + 1] - etaHFBins [i];
	}
    else  
	for (int i = 0; i < 28; i++) 
	{
	    if ((eta > etaBins [i]) && (eta < etaBins [i + 1])) 
		phi =  etaBins [i + 1] - etaBins [i];
	}
    return phi;
}

unsigned
VisEventSetupService::getBinNumber (
    const DetId detId, // const CaloCellGeometry *cell, 
    const unsigned numR, 
    const unsigned numZ, 
    const float radiusR,
    const float radiusZ,
    const float _barrelMaxEta /*=0.f*/, 
    const float beamPipeTheta /*=0.f*/, 
    const float _endcapMaxTheta /*=0.f*/) const
{
    unsigned binNum = 0;
    
    const CaloCellGeometry *cell = (*m_caloGeom).getGeometry (detId); 
    const float y = cell->getPosition ().y ();
    const float z = cell->getPosition ().z ();
    const float eta = fabs (cell->getPosition ().eta ());
    const float theta = etaToTheta (eta);
	
    float barrelMaxEta = _barrelMaxEta;
    float endcapMaxTheta = _endcapMaxTheta;
	
    // determine barrelmaxeta if it's not given yet, by assigning
    // endcapmaxtheta if it's given or else calculating the corner. Same for
    // endcapmaxtheta.
    if (barrelMaxEta <= 0.f)
    {
	if (endcapMaxTheta <= 0.f)
	{
	    endcapMaxTheta = atan (radiusR / radiusZ);
	    barrelMaxEta = thetaToEta (endcapMaxTheta);
	}
	else
	{
	    barrelMaxEta = endcapMaxTheta;
	}
    }
	
    if (endcapMaxTheta <= 0.f)
    {
	endcapMaxTheta = etaToTheta (barrelMaxEta);
    }
	
    // remember that the positive y-Axis points downwards
    if (y < 0.f)
    {
	if (z > 0.f)
	{
	    if (eta > barrelMaxEta)
	    {
		const float deltaTheta = (endcapMaxTheta - beamPipeTheta) / (numR/2);
		binNum = (unsigned) ((theta - beamPipeTheta) / deltaTheta);
	    }
	    else
	    {
		const float deltaEta = barrelMaxEta / (numZ/2);
		binNum = (numZ + numR)/2 - (unsigned) (eta / deltaEta) - 1;
	    }
	}
	else
	{
	    if (eta <= barrelMaxEta)
	    {
		const float deltaEta = barrelMaxEta / (numZ/2);
		binNum = (unsigned)(eta / deltaEta) + (numR + numZ)/2;
	    }
	    else
	    {
		const float deltaTheta = (endcapMaxTheta - beamPipeTheta) / (numR/2);
		binNum = numR + numZ - (unsigned) ((theta - beamPipeTheta) / deltaTheta) - 1;
	    }
	}
    }
    else
    {
	if (z < 0.f)
	{
	    if (eta > barrelMaxEta)
	    {
		const float deltaTheta = (endcapMaxTheta - beamPipeTheta) / (numR/2);
		binNum = (unsigned) ((theta - beamPipeTheta) / deltaTheta) + numR + numZ;
	    }
	    else
	    {
		const float deltaEta = barrelMaxEta / (numZ/2);
		binNum = 3*(numZ + numR)/2 - (unsigned) (eta / deltaEta) - 1;
	    }
	}
	else
	{
	    if (eta <= barrelMaxEta)
	    {
		const float deltaEta = barrelMaxEta / (numZ/2);
		binNum = 3*(numZ + numR)/2 + (unsigned) (eta / deltaEta);
	    }
	    else
	    {
		const float deltaTheta = (endcapMaxTheta - beamPipeTheta) / (numR/2);
		binNum = 2*(numR + numZ) - (unsigned) ((theta - beamPipeTheta) / deltaTheta) - 1;
	    }
	}
    }
    return binNum;
}

unsigned
VisEventSetupService::getBinNumber (
    const float ceta, const float phi,
    const unsigned numR, 
    const unsigned numZ, 
    const float radiusR,
    const float radiusZ,
    const float _barrelMaxEta /*=0.f*/, 
    const float beamPipeTheta /*=0.f*/, 
    const float _endcapMaxTheta /*=0.f*/) const
{
    unsigned binNum = 0;
    const float y = phi;
    const float z = ceta;
    const float eta = fabs (ceta);
    
    const float theta = etaToTheta (eta);
	
    float barrelMaxEta = _barrelMaxEta;
    float endcapMaxTheta = _endcapMaxTheta;
	
    // determine barrelmaxeta if it's not given yet, by assigning
    // endcapmaxtheta if it's given or else calculating the corner. Same for
    // endcapmaxtheta.
    if (barrelMaxEta <= 0.f)
    {
	if (endcapMaxTheta <= 0.f)
	{
	    endcapMaxTheta = atan (radiusR / radiusZ);
	    barrelMaxEta = thetaToEta (endcapMaxTheta);
	}
	else
	{
	    barrelMaxEta = endcapMaxTheta;
	}
    }
	
    if (endcapMaxTheta <= 0.f)
    {
	endcapMaxTheta = etaToTheta (barrelMaxEta);
    }
	
    // remember that the positive y-Axis points downwards
    if (y < 0.f)
    {
	if (z > 0.f)
	{
	    if (eta > barrelMaxEta)
	    {
		const float deltaTheta = (endcapMaxTheta - beamPipeTheta) / (numR/2);
		binNum = (unsigned) ((theta - beamPipeTheta) / deltaTheta);
	    }
	    else
	    {
		const float deltaEta = barrelMaxEta / (numZ/2);
		binNum = (numZ + numR)/2 - (unsigned) (eta / deltaEta) - 1;
	    }
	}
	else
	{
	    if (eta <= barrelMaxEta)
	    {
		const float deltaEta = barrelMaxEta / (numZ/2);
		binNum = (unsigned)(eta / deltaEta) + (numR + numZ)/2;
	    }
	    else
	    {
		const float deltaTheta = (endcapMaxTheta - beamPipeTheta) / (numR/2);
		binNum = numR + numZ - (unsigned) ((theta - beamPipeTheta) / deltaTheta) - 1;
	    }
	}
    }
    else
    {
	if (z < 0.f) 
	{
	    if (eta > barrelMaxEta)
	    {
		const float deltaTheta = (endcapMaxTheta - beamPipeTheta) / (numR/2);
		binNum = (unsigned) ((theta - beamPipeTheta) / deltaTheta) + numR + numZ;
	    }
	    else
	    {
		const float deltaEta = barrelMaxEta / (numZ/2);
		binNum = 3*(numZ + numR)/2 - (unsigned) (eta / deltaEta) - 1;
	    }
	}
	else
	{
	    if (eta <= barrelMaxEta)
	    {
		const float deltaEta = barrelMaxEta / (numZ/2);
		binNum = 3*(numZ + numR)/2 + (unsigned) (eta / deltaEta);
	    }
	    else
	    {
		const float deltaTheta = (endcapMaxTheta - beamPipeTheta) / (numR/2);
		binNum = 2*(numR + numZ) - (unsigned) ((theta - beamPipeTheta) / deltaTheta) - 1;
	    }
	}
    }
    return binNum;
}

float
VisEventSetupService::etaToTheta (float eta) const
{
    return (2.f * atan (exp (- eta)));
}

float
VisEventSetupService::thetaToEta (float theta) const
{
    return (-log (tan (theta / 2.f)));
}

bool
VisEventSetupService::present (const DetId detId) const
{
    return (*m_caloGeom).present (detId);
}

bool
VisEventSetupService::emptyCorners  (const DetId detId) const
{
    const CaloCellGeometry *cell = (*m_caloGeom).getGeometry (detId);
    ASSERT (cell);
    return cell->emptyCorners ();    
}

const CaloCellGeometry::CornersVec&
VisEventSetupService::getCorners (const DetId detId) const
{
    const CaloCellGeometry *cell = (*m_caloGeom).getGeometry (detId);
    ASSERT (cell);
    return cell->getCorners ();
}	

const GlobalPoint&
VisEventSetupService::getCellPosition (const DetId detId) const
{
    const CaloCellGeometry *cell = (*m_caloGeom).getGeometry (detId);
    return cell->getPosition ();
}

SoSeparator*
VisEventSetupService::buildCSCChambers (DetectorRep type, int chamber, int ring, int station, int endcap)
{    
    SoSeparator *sep = new SoSeparator;    
    SoMaterial *mat = new SoMaterial;
    SoDrawStyle *style = new SoDrawStyle;
    style->style = SoDrawStyle::LINES;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x59595900, rgbcomponents); // grey35
    mat->ambientColor.setValue (SbColor (rgbcomponents));
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    mat->specularColor.setValue (SbColor (rgbcomponents));
    mat->emissiveColor.setValue (SbColor (rgbcomponents));
    mat->shininess = 0;
    mat->transparency = 0;
    sep->addChild (mat);
    sep->addChild (style);

    if (m_cscGeom.isValid ())
    {
	QApplication::setOverrideCursor (Qt::waitCursor);
	
	QString message ("Constructing CSC Geometry Rep");
	
	if (endcap != 0)
	{
	    message.append (QString (" for Endcap %1").arg (endcap));
	    if (station != 0)
	    {		
		message.append (QString (", Station %1").arg (station));
		if (ring != 0) 
		{
		    message.append (QString (", Ring %1").arg (ring));
		    if (chamber != 0)
			message.append (QString (", Sector %1").arg (chamber));
		}
	    }
	}
	message.append ("...");
	
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (message.latin1 ());

	std::vector<CSCChamber *> vc = m_cscGeom->chambers ();
    
	for (std::vector<CSCChamber *>::const_iterator it = vc.begin (), end = vc.end (); 
	     it != end; ++it)
	{
	    const CSCChamber *cscChamber = *it;
	
	    if (cscChamber)
	    {
		DetId detId = cscChamber->geographicalId ();
		int id = detId ();
	    
		bool draw = true;
	    
		(chamber == 0 || chamber == CSCDetId::chamber(id)) ? draw = true : draw = false;
		((ring == 0 || ring == CSCDetId::ring(id)) && draw) ? draw = true : draw = false;
		((station == 0 || station == CSCDetId::station(id)) && draw) ? draw = true : draw = false;
		((endcap == 0 || endcap == CSCDetId::endcap(id)) && draw) ? draw = true : draw = false;

		if (draw) 
		{
		    const BoundSurface &bSurface = cscChamber->surface ();

		    LocalPoint  lCentre (0.0, 0.0, 0.0);
		    GlobalPoint gCentre = bSurface.toGlobal (lCentre);
		    {
			if (visible (gCentre, type, GeomDetEnumerators::CSC))
			{
			    SoSeparator* separator = new SoSeparator;
			    addWafer (separator, cscChamber, type);
			    sep->addChild (separator);
			}			
		    }
		}
	    }
	}

	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("CSC Geometry Rep Done.");
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			      .arg (m_run)
			      .arg (m_event)
			      .arg (m_ls)
			      .arg (m_orbit)
			      .arg (m_bx)
			      .latin1 ());

	QApplication::restoreOverrideCursor ();

	return sep;
    }
    else
	return NULL;
}

SoSeparator*
VisEventSetupService::buildDTChambers (DetectorRep type, int station, int sector, int wheel)
{
    SoSeparator *sep = new SoSeparator;    
    SoMaterial *mat = new SoMaterial;
    SoDrawStyle *style = new SoDrawStyle;
    style->style = SoDrawStyle::LINES;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x59595900, rgbcomponents); // grey35
    mat->ambientColor.setValue (SbColor (rgbcomponents));
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    mat->specularColor.setValue (SbColor (rgbcomponents));
    mat->emissiveColor.setValue (SbColor (rgbcomponents));
    mat->shininess = 0;
    mat->transparency = 0;
    sep->addChild (mat);
    sep->addChild (style);

    if (m_dtGeom.isValid ())
    {
	QApplication::setOverrideCursor (Qt::waitCursor);
	
	QString message ("Constructing DT Geometry Rep");
	
	if (wheel != 10)
	{
	    message.append (QString (" for Wheel %1").arg (wheel));
	    if (sector != 0)
	    {		
		message.append (QString (", Sector %1").arg (sector));
		if (station != 0) 
		    message.append (QString (", Station %1").arg (station));

	    }
	}
	message.append ("...");
	
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (message.latin1 ());

	std::vector<DTChamber *> vc = m_dtGeom->chambers ();

	for (std::vector<DTChamber *>::const_iterator it = vc.begin (), end = vc.end (); 
	     it != end; ++it)
	{
	    const DTChamber *chamber = *it;
	
	    if (chamber)
	    {
		DTChamberId chId = chamber->id();
		bool draw = true;
		
		((station == 0 || station == chId.station()) && draw) ? draw = true : draw = false;
		((sector == 0 || sector == chId.sector()) && draw) ? draw = true : draw = false;
		((wheel == 10 || wheel == chId.wheel()) && draw) ? draw = true : draw = false;
	  
		if (draw) 
		{ 	      
		    if (visible ((*it)->surface ().position (), type, GeomDetEnumerators::DT))
		    {
			SoSeparator* separator = new SoSeparator;
			addWafer (separator, *it, type);
			sep->addChild (separator);
		    }
		}
	    }
	}

	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("DT Geometry Rep Done.");
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			      .arg (m_run)
			      .arg (m_event)
			      .arg (m_ls)
			      .arg (m_orbit)
			      .arg (m_bx)
			      .latin1 ());

	QApplication::restoreOverrideCursor ();

	return sep;
    }
    else
	return NULL;
}

SoSeparator*
VisEventSetupService::buildRPCChambers (DetectorRep type, int region, int wheel, int sector, int station)
{    
    SoSeparator *sep = new SoSeparator;    
    SoMaterial *mat = new SoMaterial;
    SoDrawStyle *style = new SoDrawStyle;
    style->style = SoDrawStyle::LINES;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x59595900, rgbcomponents); // grey35
    mat->ambientColor.setValue (SbColor (rgbcomponents));
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    mat->specularColor.setValue (SbColor (rgbcomponents));
    mat->emissiveColor.setValue (SbColor (rgbcomponents));
    mat->shininess = 0;
    mat->transparency = 0;
    sep->addChild (mat);
    sep->addChild (style);

    if (m_rpcGeom.isValid ())
    {    
	QApplication::setOverrideCursor (Qt::waitCursor);
	
	QString message ("Constructing RPC Geometry Rep");
	
	if (region != 10)
	{
	    message.append (QString (" for RGN %1").arg (region));
	    if (wheel != 10)
	    {		
		message.append (QString (", Wheel %1").arg (wheel));
		if (sector != 30) 
		{
		    message.append (QString (", Sector %1").arg (sector));
		    if (station != 30)
			message.append (QString (", Station %1").arg (station));
		}
	    }
	}
	message.append ("...");
	
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (message.latin1 ());

	std::vector<RPCRoll *> vc = m_rpcGeom->rolls ();

	for (std::vector<RPCRoll *>::const_iterator it = vc.begin (), end = vc.end (); 
	     it != end; ++it)
	{
	    const RPCRoll *roll = *it;
	
	    if (roll)
	    {
		RPCDetId chId = roll->id ();
		if (chId)
		{
		    bool draw = true;
	 
		    ((station == 30 || station == chId.station()) && draw) ? draw = true : draw = false;
		    ((sector == 30 || sector == chId.sector()) && draw) ? draw = true : draw = false;
		    ((wheel == 10 || wheel == chId.ring()) && draw) ? draw = true : draw = false;
		    ((region == 10 || region == chId.region ()) && draw) ? draw = true : draw = false;		    
	    
		    if (draw) 
		    { 
			if (visible ((*it)->surface ().position (), type, GeomDetEnumerators::RPCBarrel))
			{
			    SoSeparator* separator = new SoSeparator;
			    addWafer (separator, *it, type);
			    sep->addChild (separator);
			}
		    }
		}
	    }
	}

	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("RPC Geometry Rep Done.");
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			      .arg (m_run)
			      .arg (m_event)
			      .arg (m_ls)
			      .arg (m_orbit)
			      .arg (m_bx)
			      .latin1 ());

	QApplication::restoreOverrideCursor ();

	return sep;
    }
    else
	return NULL;
}

SoSeparator*
VisEventSetupService::buildTracker (DetectorRep type, const std::string name)
{
    SoSeparator *sep = new SoSeparator;    
    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x59595900, rgbcomponents); // grey35
    mat->ambientColor.setValue (SbColor (rgbcomponents));
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    mat->specularColor.setValue (SbColor (rgbcomponents));
    mat->emissiveColor.setValue (SbColor (rgbcomponents));
    mat->shininess = 0;
    mat->transparency = 0;
    sep->addChild (mat);
    SoDrawStyle *style = new SoDrawStyle;
    style->style = SoDrawStyle::LINES;
    style->lineWidth.setValue (1.0);
    sep->addChild (style);
    
    if (m_trackerGeom.isValid ())
    {
	QApplication::setOverrideCursor (Qt::waitCursor);
	    
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Constructing Tracker Geometry Rep for %1...").arg (QString::fromStdString (name)));

	TrackerGeometry::DetContainer::const_iterator it;
	TrackerGeometry::DetContainer::const_iterator end;

	if (name == "Tracker") 
	{	    
	    it = m_trackerGeom->dets ().begin ();
	    end = m_trackerGeom->dets ().end ();
	}
	else
	{	    
	    switch (subDetector (name))
	    {
	    case GeomDetEnumerators::PixelBarrel:
		it = m_trackerGeom->detsPXB ().begin ();
		end = m_trackerGeom->detsPXB ().end ();	    
		break;
	    case GeomDetEnumerators::PixelEndcap:
		it = m_trackerGeom->detsPXF ().begin ();
		end = m_trackerGeom->detsPXF ().end ();	    
		break;
	    case GeomDetEnumerators::TIB:
		it = m_trackerGeom->detsTIB ().begin ();
		end = m_trackerGeom->detsTIB ().end ();	    
		break;
	    case GeomDetEnumerators::TOB:
		it = m_trackerGeom->detsTOB ().begin ();
		end = m_trackerGeom->detsTOB ().end ();	    
		break;
	    case GeomDetEnumerators::TID:
		it = m_trackerGeom->detsTID ().begin ();
		end = m_trackerGeom->detsTID ().end ();	    
		break;
	    case GeomDetEnumerators::TEC:
		it = m_trackerGeom->detsTEC ().begin ();
		end = m_trackerGeom->detsTEC ().end ();	    
		break;
	    default:
		it = m_trackerGeom->dets ().begin ();
		end = m_trackerGeom->dets ().end ();
		break;
	    }
	}
	for (; it != end; ++it)
	{
	    if (visible ((*it)->surface ().position (), type, subDetector (name)))
	    {
		SoSeparator* separator = new SoSeparator;
		addWafer (separator, *it, type);
		sep->addChild (separator);
	    }
	}	

	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("Tracker Geometry Rep Done.");
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			      .arg (m_run)
			      .arg (m_event)
			      .arg (m_ls)
			      .arg (m_orbit)
			      .arg (m_bx)
			      .latin1 ());

	QApplication::restoreOverrideCursor ();

	return sep;
    }
    else
	return NULL;
}

SoSeparator*	 	
VisEventSetupService::buildDet (DetectorRep type, std::vector<const GeomDetUnit *>& detUnits)
{
    SoSeparator *sep = new SoSeparator;    
 
    addMaterial (sep, type);

    for (std::vector<const GeomDetUnit *>::const_iterator it = detUnits.begin (), end = detUnits.end (); it != end; ++it)    
    {
	if (visible ((*it)->surface ().position (), type, (**it).type ().subDetector ()))
	{
	    SoSeparator* separator = new SoSeparator;
	    addWafer (separator, *it, type);
	    sep->addChild (separator);
	}
    }
    return sep;
}

SoSeparator*
VisEventSetupService::buildCaloDet (DetectorRep type, const std::string detId, int subdetn) 
{
    SoSeparator *sep = new SoSeparator;    
    SoMaterial *mat = new SoMaterial;
    SoDrawStyle *style = new SoDrawStyle;
    style->style = SoDrawStyle::LINES;
    style->lineWidth.setValue (1.0);
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x59595900, rgbcomponents); // grey35
    mat->ambientColor.setValue (SbColor (rgbcomponents));
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    mat->specularColor.setValue (SbColor (rgbcomponents));
    mat->emissiveColor.setValue (SbColor (rgbcomponents));
    mat->shininess = 0;
    mat->transparency = 0;
    sep->addChild (mat);
    sep->addChild (style);

    if (m_caloGeom.isValid ())
    {
	QApplication::setOverrideCursor (Qt::waitCursor);

	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Constructing Calo Geometry Rep for %1...").arg (QString::fromStdString (detId)));

	const CaloSubdetectorGeometry *geom = (*m_caloGeom).getSubdetectorGeometry (detectorId (detId), subdetn);
	const std::vector<DetId>& ids (geom->getValidDetIds (detectorId (detId), subdetn));
	for (std::vector<DetId>::const_iterator i = ids.begin (), iEnd = ids.end (); i != iEnd; ++i) 
	{
	    const CaloCellGeometry *cell = geom->getGeometry (*i);
	
	    // FIXME: sub detector int
	    if (visible (cell->getPosition (), type, subdetn))
	    {
		const CaloCellGeometry::CornersVec& corners = cell->getCorners ();
		ASSERT (corners.size () == 8);	
		
		// Draw the hit in the crystal
		IgSoCrystalHit *crystalHit = new IgSoCrystalHit;
// 		float energy = 0.5;
// 		crystalHit->energy.setValue (energy);
	
// 		crystalHit->scale.setValue (0.10);
// 		crystalHit->relativeWidth.setValue (0.5);
		crystalHit->drawCrystal.setValue (true);
		crystalHit->drawHit.setValue (false);

		if (detId == "Hcal")
		{
		    crystalHit->front1.setValue (corners [0].x () / 100.0, corners [0].y () / 100.0, corners [0].z () / 100.0);
		    crystalHit->front2.setValue (corners [1].x () / 100.0, corners [1].y () / 100.0, corners [1].z () / 100.0);
		    crystalHit->front3.setValue (corners [2].x () / 100.0, corners [2].y () / 100.0, corners [2].z () / 100.0);
		    crystalHit->front4.setValue (corners [3].x () / 100.0, corners [3].y () / 100.0, corners [3].z () / 100.0);
	
		    crystalHit->back1.setValue  (corners [4].x () / 100.0, corners [4].y () / 100.0, corners [4].z () / 100.0);
		    crystalHit->back2.setValue  (corners [5].x () / 100.0, corners [5].y () / 100.0, corners [5].z () / 100.0);
		    crystalHit->back3.setValue  (corners [6].x () / 100.0, corners [6].y () / 100.0, corners [6].z () / 100.0);
		    crystalHit->back4.setValue  (corners [7].x () / 100.0, corners [7].y () / 100.0, corners [7].z () / 100.0);

		    crystalHit->showLines.setValue (true);
		}
		else if (detId == "Ecal") 
		{
		    crystalHit->front1.setValue (corners [3].x () / 100.0, corners [3].y () / 100.0, corners [3].z () / 100.0);
		    crystalHit->front2.setValue (corners [2].x () / 100.0, corners [2].y () / 100.0, corners [2].z () / 100.0);
		    crystalHit->front3.setValue (corners [1].x () / 100.0, corners [1].y () / 100.0, corners [1].z () / 100.0);
		    crystalHit->front4.setValue (corners [0].x () / 100.0, corners [0].y () / 100.0, corners [0].z () / 100.0);
	    
		    crystalHit->back1.setValue  (corners [7].x () / 100.0, corners [7].y () / 100.0, corners [7].z () / 100.0);
		    crystalHit->back2.setValue  (corners [6].x () / 100.0, corners [6].y () / 100.0, corners [6].z () / 100.0);
		    crystalHit->back3.setValue  (corners [5].x () / 100.0, corners [5].y () / 100.0, corners [5].z () / 100.0);
		    crystalHit->back4.setValue  (corners [4].x () / 100.0, corners [4].y () / 100.0, corners [4].z () / 100.0);	

		    crystalHit->showLines.setValue (true);
		}

// 		if (type == RZDet || type == RPhiDet)
// 		    crystalHit->showLines = true;
	    
		sep->addChild (crystalHit);
	    }
	}

	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("Calo Geometry Rep Done.");
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			      .arg (m_run)
			      .arg (m_event)
			      .arg (m_ls)
			      .arg (m_orbit)
			      .arg (m_bx)
			      .latin1 ());

	QApplication::restoreOverrideCursor ();

	return sep;
    }
    else
	return NULL;
}

SoSeparator*
VisEventSetupService::buildLegoDet (const double scale)
{    
    SoSeparator *gridSep = new SoSeparator;
    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x59595900, rgbcomponents); // grey35
    mat->ambientColor.setValue (SbColor (rgbcomponents));
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    mat->specularColor.setValue (SbColor (rgbcomponents));
    mat->emissiveColor.setValue (SbColor (rgbcomponents));
    mat->shininess = 0;
    mat->transparency = 0;
    gridSep->addChild (mat);

    // HF
    float etaHFBins [43] = { 0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609,
			     0.696, 0.783, 0.87, 0.957, 1.044, 1.131, 1.218, 1.305,
			     1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043,
			     2.172, 2.322, 2.5, 2.65, 3,
			     2.853, 2.964, 3.139, 3.314, // HF
			     3.486, 3.664, 3.839, 4.013, 
			     4.191, 4.363, 4.538, 4.716, 
			     4.889, 5.191};
    
    SoVertexProperty *vtx = new SoVertexProperty;
    SoIndexedLineSet *grid = new SoIndexedLineSet;
    SoMFInt32 coords;
    int vertex = 6;
    int coord = 0;
    int tick = 0;
    
    float x = float (2 * M_PI);
    float z = 5.191;
    
    vtx->vertex.set1Value (0, SbVec3f (0, 0, -z));
    vtx->vertex.set1Value (1, SbVec3f (0, 0,  z));
    vtx->vertex.set1Value (2, SbVec3f (x, 0,  z));
    vtx->vertex.set1Value (3, SbVec3f (x, 0, -z));
    vtx->vertex.set1Value (4, SbVec3f (0, 0, -z));
    vtx->vertex.set1Value (5, SbVec3f (0, 1, -z));
    
    for (coord = 0; coord < vertex; coord++)
	coords.set1Value (coord, coord);
    coords.set1Value (coord++, SO_END_LINE_INDEX);
    
    for (tick = 0; tick < 43; tick++)
    {
	vtx->vertex.set1Value (vertex, SbVec3f (0, 0, etaHFBins [tick]));       
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, etaHFBins [tick]));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);
	
	vtx->vertex.set1Value (vertex, SbVec3f (0, 0, -etaHFBins [tick]));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, -etaHFBins [tick]));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);
    }
    
    int ticks = 36;
    x = 0;
    
    for (tick = 0; tick < ticks; tick++)
    {
	x = tick * M_PI / 18.0;
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, -z));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, z));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);
	
	for (int ttow = 1; ttow < 2; ttow++)
	{
	    vtx->vertex.set1Value (vertex, SbVec3f (x + ttow * 0.087, 0, -1.74));
	    coords.set1Value (coord++, vertex++);
	    vtx->vertex.set1Value (vertex, SbVec3f (x + ttow * 0.087, 0, 1.74));
	    coords.set1Value (coord++, vertex++);
	    coords.set1Value (coord++, SO_END_LINE_INDEX);
	}
    }
    
    //
    float ztick = 5;
    float ytick = 0.1;
    float xtick = M_PI / 4;
    
    for (int etaTicks = 0; etaTicks < 11; etaTicks++, ztick--)
    {
	vtx->vertex.set1Value (vertex, SbVec3f (0, ytick, ztick));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (0, 0, ztick));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (-ytick, 0, ztick));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);
    }
    for (int phiTicks = 0; phiTicks < 8; phiTicks++)
    {
	vtx->vertex.set1Value (vertex, SbVec3f (xtick, ytick, -z));
	coords.set1Value (coord++, vertex++);
        vtx->vertex.set1Value (vertex, SbVec3f (xtick, 0, -z));
        coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (xtick, 0, -z - ytick));
        coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);
	xtick += M_PI / 4;
    }
    //

    grid->vertexProperty = vtx;
    grid->coordIndex = coords;
    
    gridSep->addChild (grid);
    
    SoSeparator         *phiLabels = new SoSeparator;
    SoText2             *text;
    SoText2             *phiLabelText [8];
    SoTranslation       *phiStart = new SoTranslation;
    SoTranslation       *phiTranslation = new SoTranslation;
    
    z = 5.191;
    
    phiStart->translation = SbVec3f (M_PI / 4, 0, -z - 0.6);
    phiTranslation->translation = SbVec3f (M_PI / 4, 0, 0);
    phiLabels->addChild (phiStart);
    
    for (tick = 0; tick < 8; tick ++) 
    {
        text = phiLabelText [tick] = new SoText2;
	phiLabels->addChild (text);
	phiLabels->addChild (phiTranslation);
    }
    
    phiLabelText [0]->string = "PI/4";
    phiLabelText [1]->string = "PI/2";
    phiLabelText [2]->string = "3PI/4";
    phiLabelText [3]->string = "PI";
    phiLabelText [4]->string = "5PI/4";
    phiLabelText [5]->string = "3PI/2";
    phiLabelText [6]->string = "7PI/4";
    phiLabelText [7]->string = "2PI";
    
    gridSep->addChild (phiLabels);
    
    z = -5.191;
    
    // scale
    SoSeparator         *labelScale = new SoSeparator;
    SoText2             *labelScaleText = new SoText2;
    SoTranslation       *labelScaleOffset = new SoTranslation;
    char scaleChars [12] = "1.0 GeV";
    sprintf (scaleChars, "%.2G GeV", scale);
    labelScaleText->string = scaleChars;
    labelScaleOffset->translation
        = SbVec3f (-0.6, 1, z - 0.6);
    labelScale->addChild (labelScaleOffset);
    labelScale->addChild (labelScaleText);
    gridSep->addChild (labelScale);
    
    SoSeparator         *etaLabels = new SoSeparator;
    SoTranslation       *etaStart = new SoTranslation;
    SoTranslation       *etaTranslation = new SoTranslation;
    
    etaStart->translation = SbVec3f (-0.6, 0, -5.0);
    etaTranslation->translation = SbVec3f (0, 0, 1);
    etaLabels->addChild (etaStart);
    
    for (tick = 0; tick < 11; tick++, z++)
    {
        text = new SoText2;
	char textLabel [5];
	sprintf (textLabel, "%.0f", z);
	text->string = textLabel;
	etaLabels->addChild (text);
	etaLabels->addChild (etaTranslation);
    }
    gridSep->addChild (etaLabels);

    return gridSep;
}

SoSeparator*
VisEventSetupService::buildHitDets (DetectorRep type, std::vector<DetId> &dets)
{
    SoSeparator *sep = new SoSeparator;    
    SoMaterial *mat = new SoMaterial;
    SoDrawStyle *style = new SoDrawStyle;
    style->style = SoDrawStyle::LINES;
    style->lineWidth.setValue (2.0);
    sep->addChild (style);

    switch (type)
    {
    case FullDet:// FFC0CB 255;192;203
	mat->ambientColor.setValue (1, 0.752941, 0.796078);
	mat->diffuseColor.setValue (1, 0.752941, 0.796078);
	mat->specularColor.setValue (1, 0.752941, 0.796078);
	mat->emissiveColor.setValue (1, 0.752941, 0.796078);
	mat->shininess = 0;
	mat->transparency = 0;
	break;
    case RZDet:
    case RPhiDet:
    default: // 140;23;23
	mat->ambientColor.setValue (0.5490196, 0.0090196, 0.0090196);
	mat->diffuseColor.setValue (0.5490196, 0.0090196, 0.0090196);
	mat->specularColor.setValue (0.5490196, 0.0090196, 0.0090196);
	mat->emissiveColor.setValue (0.5490196, 0.0090196, 0.0090196);
	mat->shininess = 0;
	mat->transparency = 0;
	break;
    }
    sep->addChild (mat);

    if (m_globalTrackingGeom.isValid ())
    {
	for (std::vector<DetId>::const_iterator it = dets.begin (), end = dets.end (); it != end; ++it)    
	{
	    const GeomDet *geomDet = m_globalTrackingGeom->idToDet (*it);
	    //GeomDetUnit
	    if (visible (geomDet->surface ().position (), type, geomDet->subDetector ()))
	    {
		SoSeparator* separator = new SoSeparator;
		addWafer (separator, geomDet, type);
		sep->addChild (separator);
	    }
	}
	return sep;
    }
    else
	return NULL;
}

SoSeparator*
VisEventSetupService::annotateCSCChambers (DetectorRep type, int chamber, int ring, int station, int endcap)
{
    SoSeparator *sep = new SoSeparator;    
//     SoMaterial *mat = new SoMaterial;
//     SoDrawStyle *style = new SoDrawStyle;
//     style->style = SoDrawStyle::LINES;
//     style->lineWidth.setValue (1.0);
//     float rgbcomponents [4];
//     IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
//     mat->diffuseColor.setValue (SbColor (rgbcomponents));
//     mat->emissiveColor.setValue (SbColor (rgbcomponents));
//     sep->addChild (mat);
//     sep->addChild (style);

    if (m_cscGeom.isValid ())
    {
	QApplication::setOverrideCursor (Qt::waitCursor);
	
	QString message ("Annotating CSC Geometry ");
	std::string annot;
	
	if (endcap != 0)
	{
	    message.append (QString (" for "));
	    message.append (QString ("Endcap %1").arg (endcap));
	    annot.append (QString ("E%1").arg (endcap).latin1 ());
	    if (station != 0)
	    {		
		message.append (QString (", Station %1").arg (station));
		annot.append (QString (" S%1").arg (station).latin1 ());
		if (ring != 0) 
		{
		    message.append (QString (", Ring %1").arg (ring));
		    annot.append (QString (" R%1").arg (ring).latin1 ());
		    if (chamber != 0)
		    {			
			message.append (QString (" Chamber %1").arg (chamber));
			annot.append (QString (" C%1").arg (chamber).latin1 ());
		    }		    
		}
	    }
	}
	message.append ("...");
	
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (message.latin1 ());

	std::vector<CSCChamber *> vc = m_cscGeom->chambers ();
    
	for (std::vector<CSCChamber *>::const_iterator it = vc.begin (), end = vc.end (); 
	     it != end; ++it)
	{
	    const CSCChamber *cscChamber = *it;
	
	    if (cscChamber)
	    {
		DetId detId = cscChamber->geographicalId ();
		int id = detId ();
	    
		bool draw = true;
	    
		(chamber == 0 || chamber == CSCDetId::chamber(id)) ? draw = true : draw = false;
		((ring == 0 || ring == CSCDetId::ring(id)) && draw) ? draw = true : draw = false;
		((station == 0 || station == CSCDetId::station(id)) && draw) ? draw = true : draw = false;
		((endcap == 0 || endcap == CSCDetId::endcap(id)) && draw) ? draw = true : draw = false;

		if (draw) 
		{
		    const BoundSurface &bSurface = cscChamber->surface ();

		    LocalPoint  lCentre (0.0, 0.0, 0.0);
		    GlobalPoint gCentre = bSurface.toGlobal (lCentre);
		    {
			if (visible (gCentre, type, GeomDetEnumerators::CSC))
			{
			    SoSeparator* separator = new SoSeparator;
			    addDescription (separator, cscChamber, annot);
			    sep->addChild (separator);
			}
		    }
		}
	    }
	}

	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("CSC Geometry Annotation Done.");
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			      .arg (m_run)
			      .arg (m_event)
			      .arg (m_ls)
			      .arg (m_orbit)
			      .arg (m_bx)
			      .latin1 ());

	QApplication::restoreOverrideCursor ();

	return sep;
    }
    else
	return NULL;
}

SoSeparator*
VisEventSetupService::annotateDTChambers (DetectorRep type, int station, int sector, int wheel) 
{    
    SoSeparator *sep = new SoSeparator;    
//     SoMaterial *mat = new SoMaterial;
//     SoDrawStyle *style = new SoDrawStyle;
//     style->style = SoDrawStyle::LINES;
//     style->lineWidth.setValue (1.0);
//     float rgbcomponents [4];
//     IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
//     mat->diffuseColor.setValue (SbColor (rgbcomponents));
//     mat->emissiveColor.setValue (SbColor (rgbcomponents));
//     sep->addChild (mat);
//     sep->addChild (style);

    if (m_dtGeom.isValid ())
    {
	QApplication::setOverrideCursor (Qt::waitCursor);
	
	QString message ("Annotating DT Geometry");
	std::string annot;
	
	if (wheel != 10)
	{
	    message.append (QString (" for Wheel %1").arg (wheel));
	    annot.append (QString ("W%1").arg (wheel).latin1 ());
	    if (sector != 0)
	    {		
		message.append (QString (", Sector %1").arg (sector));
		annot.append (QString (" SE%1").arg (sector).latin1 ());
		if (station != 0)
		{
		    message.append (QString (", Station %1").arg (station));
		    annot.append (QString (" ST%1").arg (station).latin1 ());
		}
	    }
	}
	message.append ("...");
	
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (message.latin1 ());

	std::vector<DTChamber *> vc = m_dtGeom->chambers ();

	for (std::vector<DTChamber *>::const_iterator it = vc.begin (), end = vc.end (); 
	     it != end; ++it)
	{
	    const DTChamber *chamber = *it;
	
	    if (chamber)
	    {
		DTChamberId chId = chamber->id();
		bool draw = true;
		
		((station == 0 || station == chId.station()) && draw) ? draw = true : draw = false;
		((sector == 0 || sector == chId.sector()) && draw) ? draw = true : draw = false;
		((wheel == 10 || wheel == chId.wheel()) && draw) ? draw = true : draw = false;
	  
		if (draw) 
		{ 	      
		    if (visible ((*it)->surface ().position (), type, GeomDetEnumerators::DT))
		    {
			SoSeparator* separator = new SoSeparator;
			addDescription (separator, *it, annot);
			sep->addChild (separator);
		    }
		}
	    }
	}

	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("DT Geometry Annotation Done.");
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			      .arg (m_run)
			      .arg (m_event)
			      .arg (m_ls)
			      .arg (m_orbit)
			      .arg (m_bx)
			      .latin1 ());

	QApplication::restoreOverrideCursor ();

	return sep;
    }
    else
	return NULL;
}

SoSeparator*
VisEventSetupService::annotateRPCChambers (DetectorRep type, int region, int wheel, int sector, int station) 
{    
    SoSeparator *sep = new SoSeparator;    
//     SoMaterial *mat = new SoMaterial;
//     SoDrawStyle *style = new SoDrawStyle;
//     style->style = SoDrawStyle::LINES;
//     style->lineWidth.setValue (1.0);
//     float rgbcomponents [4];
//     IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
//     mat->diffuseColor.setValue (SbColor (rgbcomponents));
//     mat->emissiveColor.setValue (SbColor (rgbcomponents));
//     sep->addChild (mat);
//     sep->addChild (style);
    if (m_rpcGeom.isValid ())
    {    
	QApplication::setOverrideCursor (Qt::waitCursor);
	
	QString message ("Annotating RPC Geometry ");
	std::string annot;
	
	if (region != 10)
	{
	    message.append (QString (" for RGN %1").arg (region));
	    annot.append (QString ("R%1").arg (region).latin1 ());
	    if (wheel != 10)
	    {		
		message.append (QString (", Wheel %1").arg (wheel));
		annot.append (QString ("W%1").arg (wheel).latin1 ());
		if (sector != 30) 
		{
		    message.append (QString (", Sector %1").arg (sector));
		    annot.append (QString (" SE%1").arg (sector).latin1 ());
		    if (station != 30)
		    {
			message.append (QString (", Station %1").arg (station));
			annot.append (QString (" ST%1").arg (station).latin1 ());
		    }
		}
	    }
	}
	message.append ("...");
	
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (message.latin1 ());

	std::vector<RPCRoll *> vc = m_rpcGeom->rolls ();

	for (std::vector<RPCRoll *>::const_iterator it = vc.begin (), end = vc.end (); 
	     it != end; ++it)
	{
	    const RPCRoll *roll = *it;
	
	    if (roll)
	    {
		RPCDetId chId = roll->id ();
		if (chId)
		{
		    bool draw = true;
	 
		    ((station == 30 || station == chId.station()) && draw) ? draw = true : draw = false;
		    ((sector == 30 || sector == chId.sector()) && draw) ? draw = true : draw = false;
		    ((wheel == 10 || wheel == chId.ring()) && draw) ? draw = true : draw = false;
		    ((region == 10 || region == chId.region ()) && draw) ? draw = true : draw = false;		    
	    
		    if (draw) 
		    { 
			if (visible ((*it)->surface ().position (), type, GeomDetEnumerators::RPCBarrel))
			{
			    SoSeparator* separator = new SoSeparator;
			    addDescription (separator, *it, annot);
			    sep->addChild (separator);
			}
		    }
		}
	    }
	}

	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("RPC Annotation Done.");
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			      .arg (m_run)
			      .arg (m_event)
			      .arg (m_ls)
			      .arg (m_orbit)
			      .arg (m_bx)
			      .latin1 ());

	QApplication::restoreOverrideCursor ();

	return sep;
    }
    else
	return NULL;
}

SoSeparator*
VisEventSetupService::annotateTracker (DetectorRep type, const std::string name) 
{    
    SoSeparator *sep = new SoSeparator;    
//     SoMaterial *mat = new SoMaterial;
//     SoDrawStyle *style = new SoDrawStyle;
//     style->style = SoDrawStyle::LINES;
//     style->lineWidth.setValue (1.0);
//     float rgbcomponents [4];
//     IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
//     mat->diffuseColor.setValue (SbColor (rgbcomponents));
//     mat->emissiveColor.setValue (SbColor (rgbcomponents));
//     sep->addChild (mat);
//     sep->addChild (style);
    
    if (m_trackerGeom.isValid ())
    {
	QApplication::setOverrideCursor (Qt::waitCursor);
	std::string annot;
	    
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Annotating Tracker Geometry for %1...").arg (QString::fromStdString (name)));

	TrackerGeometry::DetContainer::const_iterator it;
	TrackerGeometry::DetContainer::const_iterator end;

	if (name == "Tracker") 
	{	    
	    it = m_trackerGeom->dets ().begin ();
	    end = m_trackerGeom->dets ().end ();
	}
	else
	{	    
	    switch (subDetector (name))
	    {
	    case GeomDetEnumerators::PixelBarrel:
		it = m_trackerGeom->detsPXB ().begin ();
		end = m_trackerGeom->detsPXB ().end ();	    
		break;
	    case GeomDetEnumerators::PixelEndcap:
		it = m_trackerGeom->detsPXF ().begin ();
		end = m_trackerGeom->detsPXF ().end ();	    
		break;
	    case GeomDetEnumerators::TIB:
		it = m_trackerGeom->detsTIB ().begin ();
		end = m_trackerGeom->detsTIB ().end ();	    
		break;
	    case GeomDetEnumerators::TOB:
		it = m_trackerGeom->detsTOB ().begin ();
		end = m_trackerGeom->detsTOB ().end ();	    
		break;
	    case GeomDetEnumerators::TID:
		it = m_trackerGeom->detsTID ().begin ();
		end = m_trackerGeom->detsTID ().end ();	    
		break;
	    case GeomDetEnumerators::TEC:
		it = m_trackerGeom->detsTEC ().begin ();
		end = m_trackerGeom->detsTEC ().end ();	    
		break;
	    default:
		it = m_trackerGeom->dets ().begin ();
		end = m_trackerGeom->dets ().end ();
		break;
	    }
	}
	for (; it != end; ++it)
	{
	    if (visible ((*it)->surface ().position (), type, subDetector (name)))
	    {
		SoSeparator* separator = new SoSeparator;
		uint32_t id = (*it)->geographicalId ().rawId ();
		annot = boost::lexical_cast<std::string> (trackerModuleName (id));
		addDescription (separator, *it, annot);
		sep->addChild (separator);
	    }
	}	

	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage ("Tracker Annotation Done.");
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
	    sbar->setMessage (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
			      .arg (m_run)
			      .arg (m_event)
			      .arg (m_ls)
			      .arg (m_orbit)
			      .arg (m_bx)
			      .latin1 ());

	QApplication::restoreOverrideCursor ();

	return sep;
    }
    else
	return NULL;
}

SoSeparator*
VisEventSetupService::annotateCaloDet (DetectorRep /* type */, const std::string /* detId */, int /* subdetn */)
{
    return NULL;
}

/** Add material to the scene graph based on the view*/
void
VisEventSetupService::addMaterial (SoSeparator *separator, DetectorRep type)
{    
    SoMaterial *mat = new SoMaterial;

    switch (type)
    {
    case FullDet:
	mat->ambientColor.setValue (0.88445997, 0.840042, 0.53582799);
	mat->diffuseColor.setValue (0.66334498, 0.630032, 0.401871);
	mat->specularColor.setValue (0.49999201, 0.49999201, 0.49999201);
	mat->emissiveColor.setValue (0, 0, 0);
	mat->shininess = 0.1;
	mat->transparency = 0;
	break;
    default:
	mat->ambientColor.setValue (0.66334498, 0.66334498, 0.66334498);
	mat->diffuseColor.setValue (0.66334498, 0.66334498, 0.66334498);
	mat->specularColor.setValue (0.66334498, 0.66334498, 0.66334498);
	mat->emissiveColor.setValue (0.66334498, 0.66334498, 0.66334498);
	SoDrawStyle *style = new SoDrawStyle;
	style->style = SoDrawStyle::LINES;
	separator->addChild (style);
	break;
    }
    
    separator->addChild (mat);
}

const std::string
VisEventSetupService::trackerModuleName (DetId detId)
{
    std::ostringstream outs;
    switch (detId.subdetId ())
    {
    case PixelSubdetector::PixelBarrel:
	outs << PXBDetId (detId);
	break;
    case PixelSubdetector::PixelEndcap:
	outs << PXFDetId (detId);
	break;
    case StripSubdetector::TIB:
	outs << TIBDetId (detId);
	break;
    case StripSubdetector::TID:
	outs << TIDDetId (detId);
	break;
    case StripSubdetector::TOB:
	outs << TOBDetId (detId);
	break;
    case StripSubdetector::TEC:
	outs << TECDetId (detId);
	break;
    default:
	break;
    }
    
    return outs.str ();
}

void
VisEventSetupService::addDescription (SoSeparator *separator, const GeomDet *det, const std::string label)
{
    // position of the wafer's center
    float posx = det->surface ().position ().x () / 100.0;  // cm -> m
    float posy = det->surface ().position ().y () / 100.0;  // cm -> m
    float posz = det->surface ().position ().z () / 100.0;  // cm -> m

    float angle;
    SbVec3f axis;
    createRotation (det, axis, angle);

    SoTransform *transform = new SoTransform;
    transform->translation.setValue (posx, posy, posz);
    transform->rotation.setValue (axis, angle);

    SoSeparator *labelSep = new SoSeparator;
    SoText2  *cLabel = new SoText2;
    cLabel->string = label.c_str ();
    labelSep->addChild (cLabel);
    
    separator->addChild (transform);
    separator->addChild (labelSep);
}

/** Transform and add the DetUnit to the scene graph */
void
VisEventSetupService::addWafer (SoSeparator *separator, const GeomDet *det, DetectorRep type)
{    
    // position of the wafer's center
    float posx = det->surface ().position ().x () / 100.0;  // cm -> m
    float posy = det->surface ().position ().y () / 100.0;  // cm -> m
    float posz = det->surface ().position ().z () / 100.0;  // cm -> m
    if (type == RZDet) 
    {
	posx = 0;
	posy = sqrt (det->surface ().position ().x () * det->surface ().position ().x () 
		     + det->surface ().position ().y () * det->surface ().position ().y ()) / 100.0; 
	if (det->surface ().position ().y () < 0.0) posy = - posy;
    }
    float angle;
    SbVec3f axis;
    createRotation (det, axis, angle);

    SoTransform *transform = new SoTransform;
    transform->translation.setValue (posx, posy, posz);
    if (type == RZDet)
    {
	int subdet = det->geographicalId ().subdetId (); 
	if (subdet == 1 || subdet == 3 || subdet == 5) //barrel
	{
	    SbMatrix sbm (1, 0, 0, 0,
			  0, 0, -1, 0,
			  0, 1, 0, 0,
			  0, 0, 0, 1);
	    SbRotation sbr (sbm);
	    sbr.getValue (axis, angle);
	    transform->rotation.setValue (axis, angle);
        }  
	else 
	    transform->rotation.setValue (axis, angle);
    }  
    else 
	transform->rotation.setValue (axis, angle);

    // Trapezoidal
    const Bounds *b = &((det->surface ()).bounds ());
    if (dynamic_cast<const TrapezoidalPlaneBounds *> (b))
    {
	const TrapezoidalPlaneBounds *b2 = dynamic_cast<const TrapezoidalPlaneBounds *> (b);
	std::vector< float > par = b2->parameters ();
	
	// these parameters are half-lengths, as in CMSIM/GEANT3
	float hBottomEdge = par [0];
	float hTopEdge    = par [1];
	float thickness   = par [2];
	float apothem     = par [3];

	IgSoG4Trap *g4Trap = new IgSoG4Trap ();
	g4Trap->dz = thickness / 100.; 
	g4Trap->dy1 = apothem / 100.;
	g4Trap->dy2 = apothem / 100.;
	g4Trap->dx1 = hBottomEdge / 100.;
	g4Trap->dx2 = hTopEdge / 100.;
	g4Trap->dx3 = hBottomEdge / 100.;
	g4Trap->dx4 = hTopEdge / 100.;
	g4Trap->alp1 = 0.0;
	g4Trap->alp2 = 0.0;
	separator->addChild (transform);
	separator->addChild (g4Trap);
    }
    if (dynamic_cast<const RectangularPlaneBounds *> (b))
    {
	SoCube *cube = new SoCube; // Default width 2, height 2, depth 2
	    
	// Rectangular
	float length = det->surface ().bounds ().length () / 200.0; // cm -> m
	float width = det->surface ().bounds ().width () / 200.0;   // cm -> m
	float thickness = det->surface ().bounds ().thickness () / 200.0;  // cm -> m
	
	transform->scaleFactor.setValue (width, length, thickness);
	separator->addChild (transform);
	separator->addChild (cube);
    }
}

/** Get rotation of a DetUnit */ 
void
VisEventSetupService::createRotation (const GeomDet *det, SbVec3f & axis, float & angle)
{    
    // Extract the coeff of the rotation matrix
    // with a projection on the basis vectors
    TkRotation< float > rot = det->surface ().rotation ();
    Basic3DVector< float > nx (1., 0., 0.); // x basis vector
    Basic3DVector< float > ny (0., 1., 0.); // y basis vector
    Basic3DVector< float > nz (0., 0., 1.); // z basis vector
    Basic3DVector< float > px = rot * nx; // Rxx Ryx Rzx
    Basic3DVector< float > py = rot * ny; // Rxy Ryy Rzy
    Basic3DVector< float > pz = rot * nz; // Rxz Ryz Rzz
    /* the rotation matrix is
     *	Rxx Rxy Rxz
     *	Ryx Ryy Ryz
     *	Rzx Rzy Rzz
     */
    // SbMatrix in row major order.
    // The 4th dimension is set empiricly set to
    // the identity
    SbMatrix sbm (px.x (), py.x (), pz.x (), 0,
		  px.y (), py.y (), pz.y (), 0,
		  px.z (), py.z (), pz.z (), 0,
		  0, 0, 0, 1);
    SbRotation sbr (sbm);

    float x;
    float y;
    float z;

    sbr.getValue (axis, angle);
    axis.getValue (x, y, z);
}

/** Transform and add the DetUnit to the scene graph */
void
VisEventSetupService::addBoxWafer (SoSeparator *separator, const GeomDet *det)
{    
    const int NVERT = 8;

    Basic3DVector< float > nx (1., 0., 0.); // x basis vector
    Basic3DVector< float > ny (0., 1., 0.); // y basis vector
    Basic3DVector< float > nz (0., 0., 1.); // z basis vector

    float posx = 0.;
    float posy = 0.;
    float posz = 0.;
    float x = 0.;
    float y = 0.;
    float z = 0.;

    static int faceIndices[] = 
	{0, 4, 3, 7, 2, 6, 1, 5, 0, 4, SO_END_FACE_INDEX,
	 1, 0, 2, 3, SO_END_FACE_INDEX,
	 6, 7, 5, 4, SO_END_FACE_INDEX
	};     

    static SoIndexedTriangleStripSet *detFaceSet = 0;
    if (detFaceSet == 0)
    {
	detFaceSet = new SoIndexedTriangleStripSet;
	detFaceSet->coordIndex.setValues(0, sizeof (faceIndices)/sizeof (int), faceIndices);
    }
    
    TkRotation< float > detRot = det->surface ().rotation ();
    Basic3DVector< float > px = detRot * nx; // Rxx Ryx Rzx
    Basic3DVector< float > py = detRot * ny; // Rxy Ryy Rzy
    Basic3DVector< float > pz = detRot * nz; // Rxz Ryz Rzz

    // set the rotation matrix
    SbMatrix rotMatrix (px.x (), py.x (), pz.x (), 0,
			px.y (), py.y (), pz.y (), 0,
			px.z (), py.z (), pz.z (), 0,
			0, 0, 0, 1);

    // position of the wafer's center
    posx = det->surface ().position ().x () / 100.0;  // cm -> m
    posy = det->surface ().position ().y () / 100.0;  // cm -> m
    posz = det->surface ().position ().z () / 100.0;  // cm -> m

    x = det->surface ().bounds ().width () / 100.0;   // cm -> m
    y = det->surface ().bounds ().length () / 100.0; // cm -> m
    z = det->surface ().bounds ().thickness () / 100.0;  // cm -> m
		
    SbVec3f points [NVERT] = {
	SbVec3f ( 0.5f, -0.5f, -0.5f),      // Origin (0, 0, 0) in the center of the cube:
	SbVec3f ( 0.5f,  0.5f, -0.5f),      //   7------6      Z
	SbVec3f ( 0.5f,  0.5f,  0.5f),      //  /|     /|      |
	SbVec3f ( 0.5f, -0.5f,  0.5f),      // 3------2 |      |
	SbVec3f (-0.5f, -0.5f, -0.5f),      // | 4----|-5     /--->Y
	SbVec3f (-0.5f,  0.5f, -0.5f),      // |/     |/     /
	SbVec3f (-0.5f,  0.5f,  0.5f),      // 0------1     X
	SbVec3f (-0.5f, -0.5f,  0.5f)       // 
    };
    
    SoMFVec3f vertexData;
    vertexData.setValues (0, NVERT, points);

    SoCoordinate3 *detCoords = new SoCoordinate3;
    detCoords->point = vertexData;
    
    SoTransform *detTransform = new SoTransform;
    detTransform->translation.setValue (posx, posy, posz);
    detTransform->rotation = SbRotation (rotMatrix);
    detTransform->scaleFactor.setValue (x, y, z);

    // Create the separator for the faces.
    SoSeparator *facesNode = new SoSeparator;
    facesNode->addChild (detTransform);
    facesNode->addChild (detCoords);
    facesNode->addChild (detFaceSet);

    separator->addChild (facesNode);
}

SoSeparator*
VisEventSetupService::refitTrack (reco::TrackRef track, double in, double out, double step) 
{
    if (track.isNonnull ())
    {
	SoSeparator *sep = new SoSeparator;
	std::vector<SbVec3f> myTrack;

	reco::TransientTrack aRealTrack (track, &*m_field);
	// float recPt = aRealTrack.impactPointTSCP ().momentum ().perp ();
  
	//	std::cout << " p: "<< aRealTrack.impactPointTSCP ().momentum ().mag () << " pT: " << recPt << std::endl;
	//      std::cout << " and the charge is ... " << aRealTrack.impactPointTSCP ().charge () << std::endl;

	//Get a propagator and inner/outer state
	SteppingHelixPropagator propagator ( &*m_field, alongMomentum);
	SteppingHelixPropagator reversePropagator ( &*m_field, oppositeToMomentum);
	GlobalPoint gPin ((*track).innerPosition ().x (),
			  (*track).innerPosition ().y (),
			  (*track).innerPosition ().z ());
	GlobalPoint gPout ((*track).outerPosition ().x (),
			   (*track).outerPosition ().y (),
			   (*track).outerPosition ().z ());
	GlobalVector InOutVector = (gPout - gPin);

	// Define rotation for plane perpendicular to InOutVector
	// z axis coincides with perp
	GlobalVector zAxis = InOutVector.unit();
	// x axis has no global Z component
	GlobalVector xAxis;
	if ( zAxis.x() != 0 || zAxis.y() != 0) {
	  // precision is not an issue here, just protect against divizion by zero
	  xAxis = GlobalVector( -zAxis.y(), zAxis.x(), 0).unit();
	}
	else { // perp coincides with global Z
	  xAxis = GlobalVector( 1, 0, 0);
	}
	// y axis obtained by cross product
	GlobalVector yAxis( zAxis.cross( xAxis));
	Surface::RotationType rot( xAxis.x(), xAxis.y(), xAxis.z(),
				   yAxis.x(), yAxis.y(), yAxis.z(),
				   zAxis.x(), zAxis.y(), zAxis.z());

	// Define step size and number of extra steps on inside and outside
	int nSteps = 0;
	int nStepsInside = 0;
	int nStepsOutside = 0;
	GlobalVector StepVector;
	if( step > 0.01 ) {
	  StepVector = InOutVector * step;
	  nSteps = int (0.5 + 1.0/step);
	  nStepsInside = int (0.5 + in/step);
	  nStepsOutside = int (0.5 + out/step);
	} else {
	  StepVector = InOutVector * 0.01;
	  nSteps = 100;
	  nStepsInside = int (0.5 + in*100);
	  nStepsOutside = int (0.5 + out*100);	  
	} 

	GlobalVector gVin ((*track).innerMomentum ().x (),
			   (*track).innerMomentum ().y (),
			   (*track).innerMomentum ().z ());
	GlobalVector gVout ((*track).outerMomentum ().x (),
			    (*track).outerMomentum ().y (),
			    (*track).outerMomentum ().z ());
	GlobalTrajectoryParameters GTPin (gPin, gVin, aRealTrack.impactPointTSCP ().charge (), &*m_field);
	FreeTrajectoryState FTSin (GTPin);
	GlobalTrajectoryParameters GTPout (gPout, gVout, aRealTrack.impactPointTSCP ().charge (), &*m_field);
	FreeTrajectoryState FTSout (GTPout);

	int nGood = 0;
	int nBad = 0;

	// Do nStepsInside propagations on inside to plot track
	GlobalPoint GP = gPin; 
	GP -= nStepsInside * StepVector;
	Basic3DVector<float> Basic3DV (StepVector.x (), StepVector.y (), StepVector.z ());
	for (int istep = 0; istep < nStepsInside; ++istep) 
	{
	    GP += StepVector;
	    Surface::PositionType pos (GP.x (), GP.y (), GP.z ());
	    PlaneBuilder::ReturnType SteppingPlane = PlaneBuilder ().plane (pos, rot);
	    TrajectoryStateOnSurface trj =  reversePropagator.propagate (FTSin, *SteppingPlane);
	    if (trj.isValid ())
	    {
		float x = trj.globalPosition ().x () / 100.0;
		float y = trj.globalPosition ().y () / 100.0;
		float z = trj.globalPosition ().z () / 100.0;		   
		myTrack.push_back (SbVec3f (x, y, z));
		nGood++;
	    } 
	    else 
	    {
		nBad++;
	    }
	}
        // Do nStep propagations from track Inner state to track Outer state


	GP = gPin - StepVector;
	float w = 0.;
	for (int istep = 0; istep < nSteps+1 ; ++istep) 
	  { // from innerPosition to outerPosition
	    GP += StepVector;
	    Surface::PositionType pos (GP.x (), GP.y (), GP.z ());
	    PlaneBuilder::ReturnType SteppingPlane = PlaneBuilder ().plane (pos, rot);
	    TrajectoryStateOnSurface trj_in =  propagator.propagate (FTSin, *SteppingPlane);
	    TrajectoryStateOnSurface trj_out =  reversePropagator.propagate (FTSout, *SteppingPlane);
	    if (trj_in.isValid () && trj_out.isValid ()) 
	    {
		float x1 = trj_in.globalPosition ().x () / 100.0;
		float y1 = trj_in.globalPosition ().y () / 100.0;
		float z1 = trj_in.globalPosition ().z () / 100.0;		   

		float x2 = trj_out.globalPosition ().x () / 100.0;
		float y2 = trj_out.globalPosition ().y () / 100.0;
		float z2 = trj_out.globalPosition ().z () / 100.0;		   

		float ww = 0.;
		(w < 0.4999) ? ww = w : ww = 1.0 - w;
		float w2 = 0.5*ww*ww/((1.0-ww)*(1.0-ww));
		if(w>0.4999) w2=1.0-w2;  

		float x = (1.0-w2)*x1 + w2*x2;
		float y = (1.0-w2)*y1 + w2*y2;
		float z = (1.0-w2)*z1 + w2*z2;

		myTrack.push_back (SbVec3f (x, y, z));
		nGood++;
	    } 
	    else 
	    {
		nBad++;
	    }
	    w += 1.0/float(nSteps);
	}
	// Do nStepsInside propagations on Outside to plot track
	GP = gPout;
	for (int istep = 0; istep < nStepsOutside; ++istep) 
	{
	    GP += StepVector;
	    Surface::PositionType pos (GP.x (), GP.y (), GP.z ());
	    PlaneBuilder::ReturnType SteppingPlane = PlaneBuilder ().plane (pos, rot);
	    TrajectoryStateOnSurface trj =  propagator.propagate (FTSout, *SteppingPlane);
	    if (trj.isValid ())
	    {
		float x = trj.globalPosition ().x () / 100.0;
		float y = trj.globalPosition ().y () / 100.0;
		float z = trj.globalPosition ().z () / 100.0;		   
		myTrack.push_back (SbVec3f (x, y, z));
		nGood++;
	    } 
	    else 
	    {
		nBad++;
	    }
	}




	// Save the new track
	IgSoSimpleTrajectory* reconstructedTrack = new IgSoSimpleTrajectory;
	reconstructedTrack->lineWidth = 3.0;
	// FIXME: m_Tracks.push_back (myTrack);
	int n = 0;
	for (std::vector<SbVec3f>::const_iterator it = myTrack.begin (), end = myTrack.end (); it != end; ++it) 
	{
	    reconstructedTrack->controlPoints.set1Value (n, (*it));
	    reconstructedTrack->markerPoints.set1Value (n, (*it));
	    
	    n++;
	}
	sep->addChild (reconstructedTrack);
	
	//	std::cout << "eeeh... " << nGood << " out of " << nGood+nBad << " propagations successful " << std::endl;
	//      std::cout << " And track is based on " << nRecHit << " valid RecHits " << std::endl;

	return sep;	
    }
    else
	return NULL;
}

bool
VisEventSetupService::visible (const GlobalPoint & point, DetectorRep type, int subDetector)
{
    bool visible = false;
    double width = 1.0;
    double length = 10.0;
    if (subDetector == GeomDetEnumerators::PixelBarrel) width = 1.0;
    if (subDetector == GeomDetEnumerators::PixelEndcap) width = 1.0;
    if (subDetector == GeomDetEnumerators::TOB) width = 10.0;
    if (subDetector == GeomDetEnumerators::TIB) width = 5.5;
    if (subDetector == GeomDetEnumerators::TID) width = 15.0;
    if (subDetector == GeomDetEnumerators::TEC) width = 30.0;
    if (subDetector == GeomDetEnumerators::CSC) width = 75.0;
    if (subDetector == GeomDetEnumerators::RPCBarrel) width = 270.0;
    if (subDetector == GeomDetEnumerators::DT) width = 170.0;
    
//     std::cout << m_phiStart.value () << std::endl;
//     std::cout << m_phiDelta.value () << std::endl;
    
    switch (type)
    {
    case FullDet:
	visible = true;
	break;
    case RZDet:
	{	    
	    double p = point.phi ();
	    if (p < 0) p += 2 * M_PI;
	    double p0 = m_phiStart.value ();
	    double pD = m_phiDelta.value ();
	
	    double pMin = p0 - pD;
	    double pMax = p0 + pD;
	
	    ((p >= pMin && p <= pMax) || 
	     (p >= pMin + M_PI && p <= pMax + M_PI)) ?  visible = true : visible = false;
	    //fabs (point.x ()) < width ? visible = true : visible = false; 
	}
	break;
    case RPhiDet:
      	fabs (point.z ()) < length ? visible = true : visible = false;
	break;
    default:
	break;
    }

    return visible;
}

int
VisEventSetupService::subDetector (const std::string &key) 
{ 
    std::map<std::string, GeomDetEnumerators::SubDetector> type; 

    type ["PixelBarrel"] = GeomDetEnumerators::PixelBarrel;
    type ["PixelEndcap"] = GeomDetEnumerators::PixelEndcap;
    type ["TIB"] = GeomDetEnumerators::TIB;
    type ["TOB"] = GeomDetEnumerators::TOB;
    type ["TID"] = GeomDetEnumerators::TID;
    type ["TEC"] = GeomDetEnumerators::TEC;
    type ["CSC"] = GeomDetEnumerators::CSC;
    type ["DT"] = GeomDetEnumerators::DT;
    type ["RPCBarrel"] = GeomDetEnumerators::RPCBarrel;
    type ["RPCEndcap"] = GeomDetEnumerators::RPCEndcap;
    
    return type [key]; 
}

DetId::Detector
VisEventSetupService::detectorId (const std::string &key) 
{ 
    std::map<std::string, DetId::Detector> type; 

    type ["Tracker"] = DetId::Tracker;
    type ["Muon"] = DetId::Muon;
    type ["Ecal"] = DetId::Ecal;
    type ["Hcal"] = DetId::Hcal;
    type ["Calo"] = DetId::Calo;

    return type [key]; 
}
