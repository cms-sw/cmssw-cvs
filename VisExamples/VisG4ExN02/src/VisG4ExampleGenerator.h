#ifndef IG_G4_DEMO_IG_G4DEMO_GENERATOR_H
# define IG_G4_DEMO_IG_G4DEMO_GENERATOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisExamples/VisG4ExN02/interface/config.h"
# include <G4VUserPrimaryGeneratorAction.hh>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisG4ExampleDetector;
class G4ParticleGun;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_G4_DEMO_API VisG4ExampleGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    VisG4ExampleGenerator (VisG4ExampleDetector *detector);
    ~VisG4ExampleGenerator (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void	GeneratePrimaries (G4Event *event);

private:
    VisG4ExampleDetector	*m_detector;
    G4ParticleGun	*m_gun;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_G4_DEMO_IG_G4DEMO_GENERATOR_H
