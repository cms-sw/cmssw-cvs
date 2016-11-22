#ifndef IG_G4_DEMO_IG_G4DEMO_EVENT_ACTION_H
# define IG_G4_DEMO_IG_G4DEMO_EVENT_ACTION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisExamples/VisG4ExN02/interface/config.h"
# include <G4UserEventAction.hh>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_G4_DEMO_API VisG4ExampleEventAction : public G4UserEventAction
{
public:
    // implicit constructor
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void	EndOfEventAction (const G4Event *event);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_G4_DEMO_IG_G4DEMO_EVENT_ACTION_H
