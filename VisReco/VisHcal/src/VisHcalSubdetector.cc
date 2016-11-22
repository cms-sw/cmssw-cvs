//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisHcal/interface/VisHcalSubdetector.h"
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
VisHcalSubdetector::subDet (const std::string &key) 
{ 
     std::map<std::string, HcalSubdetector> type; 
 
     type ["HcalEmpty"]        = HcalEmpty;
     type ["HcalBarrel"]       = HcalBarrel;
     type ["HcalEndcap"]       = HcalEndcap;
     type ["HcalOuter"]        = HcalOuter;
     type ["HcalForward"]      = HcalForward;
     type ["HcalTriggerTower"] = HcalTriggerTower;
     type ["HcalOther"]        = HcalOther;

     return type [key]; 
}

int
VisHcalSubdetector::otherSubDet (const std::string &key) 
{ 
     std::map<std::string, HcalOtherSubdetector> type; 
 
     type ["HcalOtherEmpty"]  = HcalOtherEmpty;
     type ["HcalCalibration"] = HcalCalibration;

     return type [key]; 
}

const std::string
VisHcalSubdetector::subDetName (HcalSubdetector key) 
{ 
    std::map<HcalSubdetector, std::string> type; 
    
    type [HcalEmpty] 	    = "HcalEmpty";
    type [HcalBarrel] 	    = "HcalBarrel";
    type [HcalEndcap] 	    = "HcalEndcap";
    type [HcalOuter] 	    = "HcalOuter";
    type [HcalForward] 	    = "HcalForward";
    type [HcalTriggerTower] = "HcalTriggerTower";
    type [HcalOther] 	    = "HcalOther";
 
    return type [key]; 
}

const std::string
VisHcalSubdetector::otherSubDetName (HcalOtherSubdetector key) 
{ 
    std::map<HcalOtherSubdetector, std::string> type; 
    
    type [HcalOtherEmpty]  = "HcalOtherEmpty";
    type [HcalCalibration] = "HcalCalibration";
 
    return type [key]; 
}
