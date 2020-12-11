#include "VisReco/VisMuonDT/interface/VisDTDigiTwig.h"
#include "VisReco/VisMuonDT/interface/VisDTRecHitsTwig.h"
#include "VisReco/VisMuonDT/interface/VisDT2DSegmentTwig.h"
#include "VisReco/VisMuonDT/interface/VisDT4DSegmentTwig.h"
#include "VisReco/VisMuonDT/interface/xtypeinfo.h"
#include "VisFramework/VisFrameworkBase/interface/xtypeinfo.h"

XTYPEINFO_DEF_1 (VisDTDigiTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisDTRecHitsTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisDT2DSegmentTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisDT4DSegmentTwig, VisQueuedTwig, false);
