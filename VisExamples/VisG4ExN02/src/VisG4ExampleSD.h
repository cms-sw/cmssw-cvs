#ifndef IG_G4_DEMO_IG_G4DEMO_SD_H
# define IG_G4_DEMO_IG_G4DEMO_SD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisExamples/VisG4ExN02/src/VisG4ExampleHit.h"
# include <G4VSensitiveDetector.hh>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_G4_DEMO_API VisG4ExampleSD : public G4VSensitiveDetector
{
public:
    VisG4ExampleSD (const std::string &name);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void	Initialize (G4HCofThisEvent *HCE);
    virtual bool	ProcessHits (G4Step *step, G4TouchableHistory *);

private:
    VisG4ExampleHitsCollection *m_hits;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_G4_DEMO_IG_G4DEMO_SD_H
