//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Core/interface/VisG4VisManager.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (VisG4VisManager, "Services/Geant4/Vis Manager");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4VisManager::VisG4VisManager (IgState *state)
    : m_state (state)
{
    // Init state stuff
    ASSERT (m_state);
    state->put (s_key, this);

    // Make myself known.
    fpConcreteInstance = this;
}

VisG4VisManager::~VisG4VisManager (void)
{ m_state->detach (s_key); fpConcreteInstance = 0; }

void
VisG4VisManager::notify (lat::Callback onChange)
{ m_callbacks.push_back (onChange); }

void
VisG4VisManager::GeometryHasChanged (void)
{
    // NB: This might get invoked even if the setup was not complete!
    // Broadcast the message onwards; this causes twigs to rebuild.
    CallbackList::iterator j;
    for (j = m_callbacks.begin (); j != m_callbacks.end (); ++j)
	(*j) ();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Unimplemented, just to keep G4 happy.  Use VisG4VisSceneHandler.
void VisG4VisManager::Draw (const G4Circle &, const G4Transform3D &) 
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4NURBS &, const G4Transform3D &)
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4Polyhedron &, const G4Transform3D &)
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4Polyline &, const G4Transform3D &)
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4Polymarker &, const G4Transform3D &)
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4Scale &, const G4Transform3D &)
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4Square &, const G4Transform3D &)
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4Text &, const G4Transform3D &)
{ ASSERT (false); }

void VisG4VisManager::Draw2D (const G4Circle&, const G4Transform3D&)
{ ASSERT (false); }

void VisG4VisManager::Draw2D (const G4NURBS&, const G4Transform3D&)
{ ASSERT (false); }

void VisG4VisManager::Draw2D (const G4Polyhedron&, const G4Transform3D&)
{ ASSERT (false); }

void VisG4VisManager::Draw2D (const G4Polyline&, const G4Transform3D&)
{ ASSERT (false); }

void VisG4VisManager::Draw2D (const G4Polymarker&, const G4Transform3D&)
{ ASSERT (false); }

void VisG4VisManager::Draw2D (const G4Square&, const G4Transform3D&)
{ ASSERT (false); }

void VisG4VisManager::Draw2D (const G4Text&, const G4Transform3D&)
{ ASSERT (false); }

void VisG4VisManager::Draw2D (const G4Text &)
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4VHit &)
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4VTrajectory &, int)
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4LogicalVolume &,
			    const G4VisAttributes &,
			    const G4Transform3D &)
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4VPhysicalVolume &,
			    const G4VisAttributes &,
			    const G4Transform3D &)
{ ASSERT (false); }

void VisG4VisManager::Draw (const G4VSolid &,
			    const G4VisAttributes&,
			    const G4Transform3D &)
{ ASSERT (false); }

void VisG4VisManager::DispatchToModel (const G4VTrajectory &, 
				       G4int)
{ ASSERT (false); }

G4bool 	
VisG4VisManager::FilterTrajectory (const G4VTrajectory &)
{ 
    ASSERT (false); 
    return 0;
}

G4bool 	
VisG4VisManager::FilterHit (const G4VHit &)
{ 
    ASSERT (false); 
    return 0;
}
