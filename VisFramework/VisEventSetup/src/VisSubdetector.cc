//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisEventSetup/interface/VisSubdetector.h"
#include <map>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const std::string
VisSubdetector::detName (DetId::Detector key) 
{ 
    std::map<DetId::Detector, std::string> type; 

    type [DetId::Tracker] = "Tracker";
    type [DetId::Muon] = "Muon";
    type [DetId::Ecal] = "Ecal";
    type [DetId::Hcal] = "Hcal";
    type [DetId::Calo] = "Calo";

    return type [key]; 
}

const std::string
VisSubdetector::subDetName (DetId::Detector det, int key) 
{ 
    std::map<int, std::string> type; 

    switch (det) 
    {
    case DetId::Tracker: 
	// type [DetId::Tracker] = "Tracker";
	type [1] = "Pixel Barrel"; // edm::ESHandle<TrackerGeometry> m_pDD; eventSetup.get<TrackerDigiGeometryRecord> ().get (m_pDD);
	type [2] = "Pixel Endcap";
	type [3] = "TIB";
	type [4] = "TID";
	type [5] = "TOB";
	type [6] = "TEC";
	break;
    case DetId::Muon:
	// type [DetId::Muon] = "Muon";
	type [1] = "DT";	// edm::ESHandle<DTGeometry>   m_pDD; eventSetup.get<MuonGeometryRecord> ().get (m_pDD);
	type [2] = "CSC";	// edm::ESHandle<CSCGeometry>  m_pDD; eventSetup.get<MuonGeometryRecord> ().get (m_pDD);
	type [3] = "RPC";	// edm::ESHandle<RPCGeometry>  m_pDD; eventSetup.get<MuonGeometryRecord> ().get (m_pDD);
	break;
    case DetId::Ecal:
	// type [DetId::Ecal] = "Ecal";
	type [1] = "EcalBarrel"; // edm::ESHandle<CaloGeometry> pG; eventSetup.get<IdealGeometryRecord>().get (pG); 
	                         // const CaloSubdetectorGeometry *geom = (*m_pDD).getSubdetectorGeometry (detectorId (m_detID), m_subdetn);
	type [2] = "EcalEndcap";
	type [3] = "EcalPreshower";
	type [4] = "EcalTriggerTower";
	type [5] = "EcalLaserPnDiode";
	break;
    case DetId::Hcal:
	// type [DetId::Hcal] = "Hcal";
	//type [0] = "HcalEmpty";
	type [1] = "HcalBarrel";
	type [2] = "HcalEndcap";
	type [3] = "HcalOuter";
	type [4] = "HcalForward";
	type [5] = "HcalTriggerTower";
	type [6] = "HcalCalibration";
	type [7] = "HcalComposite";
	break;
    case DetId::Calo:
	// type [DetId::Calo] = "Calo";
	break;
    default:
	break;
    }
    
    return type [key]; 
}
