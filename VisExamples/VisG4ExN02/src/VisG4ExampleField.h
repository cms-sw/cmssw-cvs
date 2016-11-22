#ifndef IG_G4_DEMO_IG_G4DEMO_FIELD_H
# define IG_G4_DEMO_IG_G4DEMO_FIELD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisExamples/VisG4ExN02/interface/config.h"
# include <G4UniformMagField.hh>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_G4_DEMO_API VisG4ExampleField : G4UniformMagField
{
public:
    VisG4ExampleField (G4ThreeVector fieldVector = G4ThreeVector ());
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_G4_DEMO_IG_G4DEMO_FIELD_H
