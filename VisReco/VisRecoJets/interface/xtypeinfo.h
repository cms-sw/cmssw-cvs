#ifndef VIS_RECO_JETS_XTYPEINFO_H
# define VIS_RECO_JETS_XTYPEINFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/xtypeinfo.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisCaloJetTwig;
class VisCaloTowerTwig;
class VisGenJetTwig;
class VisGenJetCollectionTwig;
class VisJetTagTwig;
class VisCandidateTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

XTYPEINFO_DECLARE (, VisCaloJetTwig);
XTYPEINFO_DECLARE (, VisCaloTowerTwig);
XTYPEINFO_DECLARE (, VisGenJetTwig);
XTYPEINFO_DECLARE (, VisGenJetCollectionTwig);
XTYPEINFO_DECLARE (, VisJetTagTwig);
XTYPEINFO_DECLARE (, VisCandidateTwig);

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_RECO_JETS_XTYPEINFO_H
