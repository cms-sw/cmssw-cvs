#ifndef VIS_MUON_CSC_XTYPEINFO_H
# define VIS_MUON_CSC_XTYPEINFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/xtypeinfo.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisCSCStripDigiTwig;
class VisCSCWireDigiTwig;
class VisCSCComparatorDigiTwig;
class VisCSCCorrelatedLCTDigiTwig;
class VisCSCALCTDigiTwig;
class VisCSCCLCTDigiTwig;
class VisCSCRecHit2DTwig;
class VisCSCSegmentTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

XTYPEINFO_DECLARE (, VisCSCStripDigiTwig);
XTYPEINFO_DECLARE (, VisCSCWireDigiTwig);
XTYPEINFO_DECLARE (, VisCSCComparatorDigiTwig);
XTYPEINFO_DECLARE (, VisCSCCorrelatedLCTDigiTwig);
XTYPEINFO_DECLARE (, VisCSCALCTDigiTwig);
XTYPEINFO_DECLARE (, VisCSCCLCTDigiTwig);
XTYPEINFO_DECLARE (, VisCSCRecHit2DTwig);
XTYPEINFO_DECLARE (, VisCSCSegmentTwig);

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MUON_CSC_XTYPEINFO_H
