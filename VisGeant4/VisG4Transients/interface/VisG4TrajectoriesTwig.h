#ifndef VIS_G4_TRANSIENTS_VIS_G4_TRAJECTORIES_TWIG_H
# define VIS_G4_TRANSIENTS_VIS_G4_TRAJECTORIES_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Transients/interface/config.h"
# include "Iguana/Framework/interface/IgTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Preliminary Geant4 trajectories twig. */
class VIS_G4_TRANSIENTS_API VisG4TrajectoriesTwig : public IgSimpleTwig
{
public:
    VisG4TrajectoriesTwig (IgTwig *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_TRANSIENTS_VIS_G4_TRAJECTORIES_TWIG_H
