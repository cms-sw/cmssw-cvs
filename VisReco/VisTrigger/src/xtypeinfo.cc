#include "VisReco/VisTrigger/interface/VisTriggerResultsTwig.h"
#include "VisReco/VisTrigger/interface/VisL1MuDTChambPhContainerTwig.h"
#include "VisReco/VisTrigger/interface/VisL1MuDTChambThContainerTwig.h"
#include "VisReco/VisTrigger/interface/VisL1MuDTTrackContainerTwig.h"
#include "VisReco/VisTrigger/interface/VisL1GlobalTriggerReadoutRecordTwig.h"
#include "VisReco/VisTrigger/interface/xtypeinfo.h"
#include "VisFramework/VisFrameworkBase/interface/xtypeinfo.h"

XTYPEINFO_DEF_1 (VisTriggerResultsTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisL1MuDTChambPhContainerTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisL1MuDTChambThContainerTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisL1MuDTTrackContainerTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisL1GlobalTriggerReadoutRecordTwig, VisQueuedTwig, false);
