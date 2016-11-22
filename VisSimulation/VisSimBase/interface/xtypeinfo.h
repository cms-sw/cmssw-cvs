#ifndef VIS_SIM_BASE_XTYPEINFO_H
# define VIS_SIM_BASE_XTYPEINFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/xtypeinfo.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisG4EventTwig;
class VisG4GeomTwig;
class VisG4TracksTwig;
class VisG4TrackPtTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

XTYPEINFO_DECLARE (, VisG4EventTwig);
XTYPEINFO_DECLARE (, VisG4GeomTwig);
XTYPEINFO_DECLARE (, VisG4TracksTwig);
XTYPEINFO_DECLARE (, VisG4TrackPtTwig);

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_SIM_BASE_XTYPEINFO_H
