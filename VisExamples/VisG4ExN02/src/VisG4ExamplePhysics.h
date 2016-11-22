#ifndef IG_G4_DEMO_IG_G4_DEMO_PHYSICS_H
# define IG_G4_DEMO_IG_G4_DEMO_PHYSICS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisExamples/VisG4ExN02/interface/config.h"
# include <G4VUserPhysicsList.hh>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_G4_DEMO_API VisG4ExamplePhysics : public G4VUserPhysicsList
{
public:
    VisG4ExamplePhysics (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
    // Construct particle and physics
    virtual void	ConstructParticle (void);
    virtual void	ConstructProcess (void);
    virtual void	SetCuts (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_G4_DEMO_IG_G4_DEMO_PHYSICS_H
