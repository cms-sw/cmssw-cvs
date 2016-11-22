#include "VisReco/VisPF/interface/VisPFClusterTwig.h"
#include "VisReco/VisPF/interface/VisPFRecHitTwig.h"
#include "VisReco/VisPF/interface/VisPFRecTrackTwig.h"
#include "VisReco/VisPF/interface/VisTrackingParticleTwig.h"
#include "VisReco/VisPF/interface/VisGsfPFRecTrackTwig.h"

#include "VisReco/VisPF/interface/xtypeinfo.h"
#include "VisFramework/VisFrameworkBase/interface/xtypeinfo.h"

XTYPEINFO_DEF_1 (VisPFClusterTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisPFRecHitTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisPFRecTrackTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisTrackingParticleTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisGsfPFRecTrackTwig, VisQueuedTwig, false);
