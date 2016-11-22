#ifndef VIS_RECO_XTYPEINFO_H
#define VIS_RECO_XTYPEINFO_H

#include "Iguana/Framework/interface/xtypeinfo.h"

class VisPFClusterTwig;
class VisPFRecHitTwig;
class VisPFRecTrackTwig;
class VisTrackingParticleTwig;
class VisGsfPFRecTrackTwig;

XTYPEINFO_DECLARE (, VisPFClusterTwig);
XTYPEINFO_DECLARE (, VisPFRecHitTwig);
XTYPEINFO_DECLARE (, VisPFRecTrackTwig);
XTYPEINFO_DECLARE (, VisTrackingParticleTwig);
XTYPEINFO_DECLARE (, VisGsfPFRecTrackTwig);

#endif
