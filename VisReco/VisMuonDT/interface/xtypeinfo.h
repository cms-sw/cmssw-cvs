#ifndef VIS_MUON_DT_XTYPEINFO_H
# define VIS_MUON_DT_XTYPEINFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/xtypeinfo.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisDTDigiTwig;
class VisDTRecHitsTwig;
class VisDT2DSegmentTwig;
class VisDT4DSegmentTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

XTYPEINFO_DECLARE (, VisDTDigiTwig);
XTYPEINFO_DECLARE (, VisDTRecHitsTwig);
XTYPEINFO_DECLARE (, VisDT2DSegmentTwig);
XTYPEINFO_DECLARE (, VisDT4DSegmentTwig);

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MUON_DT_XTYPEINFO_H
