#ifndef IG_G4_DEMO_IG_G4DEMO_RUN_ACTION_H
# define IG_G4_DEMO_IG_G4DEMO_RUN_ACTION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisExamples/VisG4ExN02/interface/config.h"
# include <G4UserRunAction.hh>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_G4_DEMO_API VisG4ExampleRunAction : public G4UserRunAction
{
public:
    // implicit constructor
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    void		BeginOfRunAction (const G4Run *run);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_G4_DEMO_IG_G4DEMO_RUN_ACTION_H
