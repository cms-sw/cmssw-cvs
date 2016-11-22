//<<<<<< INCLUDES                                                       >>>>>>

#include "VisExamples/VisG4ExN02/src/VisG4ExampleEventAction.h"
#include <G4Event.hh>
#include <G4TrajectoryContainer.hh>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
VisG4ExampleEventAction::EndOfEventAction (const G4Event *event)
{
    int				eventId = event->GetEventID ();
    G4TrajectoryContainer	*trajs = event->GetTrajectoryContainer ();
    int				n = trajs ? trajs->entries () : 0;

    if (eventId < 100 || eventId % 100 == 0) 
	std::cout << "*** Event " << eventId << ": "
		  << n << " trajectories stored in this event.\n";
}
