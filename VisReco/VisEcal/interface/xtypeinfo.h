#ifndef VIS_ECAL_XTYPEINFO_H
# define VIS_ECAL_XTYPEINFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/xtypeinfo.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisEcalUncalibratedRecHitTwig;
class VisEcalRecHitTwig;
class VisBasicClusterTwig;
class VisBasicClusterCollectionTwig;
class VisPreshowerClusterTwig;
class VisSuperClusterTwig;
class VisSuperClusterCollectionTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

XTYPEINFO_DECLARE (, VisEcalUncalibratedRecHitTwig);
XTYPEINFO_DECLARE (, VisEcalRecHitTwig);
XTYPEINFO_DECLARE (, VisBasicClusterTwig);
XTYPEINFO_DECLARE (, VisBasicClusterCollectionTwig);
XTYPEINFO_DECLARE (, VisPreshowerClusterTwig);
XTYPEINFO_DECLARE (, VisSuperClusterTwig);
XTYPEINFO_DECLARE (, VisSuperClusterCollectionTwig);

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_ECAL_XTYPEINFO_H
