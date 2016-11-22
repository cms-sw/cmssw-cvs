#ifndef VIS_G4_TRANSIENTS_VIS_G4_TRANSIENTS_TWIG_H
# define VIS_G4_TRANSIENTS_VIS_G4_TRANSIENTS_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Transients/interface/config.h"
# include "Iguana/Framework/interface/IgTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Placeholder for G4-related transient objects (event-related data).
    To be replaced by more generic content management. */
class VIS_G4_TRANSIENTS_API VisG4TransientsTwig : public IgSimpleTwig
{
public:
    VisG4TransientsTwig (IgTwig *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void	update (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_TRANSIENTS_VIS_G4_TRANSIENTS_TWIG_H
