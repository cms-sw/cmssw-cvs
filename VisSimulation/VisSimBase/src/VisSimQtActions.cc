//<<<<<< INCLUDES                                                       >>>>>>

#include "VisSimulation/VisSimBase/interface/VisSimQtActions.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "Iguana/Studio/interface/IgQtAppLoopService.h"
#include "Iguana/Studio/interface/IgTwigOps.h"
#include "VisGeant4/VisG4Core/interface/VisG4VisManager.h"
#include "VisGeant4/VisG4Core/interface/VisG4UIManager.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4TwigOps.h"
#include <classlib/utils/Callback.h>
#include <classlib/utils/DebugAids.h>
#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (VisSimQtActions, "Services/Simulation/GUI/Menu Actions");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisSimQtActions::VisSimQtActions (IgState *state)
    : m_state (state)
{
    m_state->put (s_key, this);
    
    IgQtAppLoopService *loopService = IgQtAppLoopService::get (m_state);
    
    ASSERT (loopService);
    loopService->schedule (lat::CreateCallback (this, 
						&VisSimQtActions::init));    
}

VisSimQtActions::~VisSimQtActions (void)
{
    ASSERT (m_state);
    m_state->detach (s_key);    
}

void
VisSimQtActions::init (void)
{
    // Setup G4
    qApp->lock ();
 
    // Register object action menus
    ASSERT (IgQtObjectMenuService::get (m_state));
    IgQtObjectMenuService::get (m_state)
	->add (lat::CreateCallback (new IgTwigOps, &IgTwigOps::menuAction));
    IgQtObjectMenuService::get (m_state)
	->add (lat::CreateCallback (new VisG4TwigOps, &VisG4TwigOps::menuAction));
    
    qApp->unlock (false);
}
