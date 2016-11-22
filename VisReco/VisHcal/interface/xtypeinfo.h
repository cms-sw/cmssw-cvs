#ifndef VIS_HCAL_XTYPEINFO_H
# define VIS_HCAL_XTYPEINFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/xtypeinfo.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisHBHERecHitTwig;
class VisHFRecHitTwig;
class VisHORecHitTwig;
class VisHFDataFrameTwig;
//class VisHcalDetTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

XTYPEINFO_DECLARE (, VisHBHERecHitTwig);
XTYPEINFO_DECLARE (, VisHFRecHitTwig);
XTYPEINFO_DECLARE (, VisHORecHitTwig);
XTYPEINFO_DECLARE (, VisHFDataFrameTwig);
//XTYPEINFO_DECLARE (, VisHcalDetTwig);

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_HCAL_XTYPEINFO_H
