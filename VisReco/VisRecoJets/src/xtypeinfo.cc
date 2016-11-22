#include "VisReco/VisRecoJets/interface/VisCaloJetTwig.h"
#include "VisReco/VisRecoJets/interface/VisCaloTowerTwig.h"
#include "VisReco/VisRecoJets/interface/VisGenJetTwig.h"
#include "VisReco/VisRecoJets/interface/VisGenJetCollectionTwig.h"
#include "VisReco/VisRecoJets/interface/VisJetTagTwig.h"
#include "VisReco/VisRecoJets/interface/VisCandidateTwig.h"
#include "VisReco/VisRecoJets/interface/xtypeinfo.h"
#include "VisFramework/VisFrameworkBase/interface/xtypeinfo.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"

XTYPEINFO_DEF_1 (VisCaloJetTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisCaloTowerTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisGenJetTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisGenJetCollectionTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisJetTagTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisCandidateTwig, VisQueuedTwig, false);
