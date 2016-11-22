//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisEcal/interface/VisEcalSubdetector.h"
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

int
VisEcalSubdetector::subDet (const std::string &key) 
{ 
     std::map<std::string, EcalSubdetector> type; 
 
     type ["EcalBarrel"]       = EcalBarrel;
     type ["EcalEndcap"]       = EcalEndcap;
     type ["EcalPreshower"]    = EcalPreshower;
     type ["EcalTriggerTower"] = EcalTriggerTower;
     type ["EcalLaserPnDiode"] = EcalLaserPnDiode;

     return type [key]; 
}

const std::string
VisEcalSubdetector::subDetName (EcalSubdetector key) 
{ 
    std::map<EcalSubdetector, std::string> type; 
    
    type [EcalBarrel] 	    = "EcalBarrel";
    type [EcalEndcap] 	    = "EcalEndcap";
    type [EcalPreshower]    = "EcalPreshower";
    type [EcalTriggerTower] = "EcalTriggerTower";
    type [EcalLaserPnDiode] = "EcalLaserPnDiode";
 
    return type [key]; 
}
