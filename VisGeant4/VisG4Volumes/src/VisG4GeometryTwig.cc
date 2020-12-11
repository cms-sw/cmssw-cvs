//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4GeometryTwig.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4VolumeTwig.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4Navigator.h"
#include "VisGeant4/VisG4Core/interface/VisG4VisManager.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include <G4TransportationManager.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VisAttributes.hh>
#include <set>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

// FIXME: Use G4StateManager / G4VStateDependent?

VisG4GeometryTwig::VisG4GeometryTwig (VisG4VisManager *visManager, IgTwig *parent)
    : IgSimpleTwig (parent, "Detector", true, true, true)
{ visManager->notify (lat::CreateCallback (this, &VisG4GeometryTwig::update)); }

void
VisG4GeometryTwig::update (void)
{
    destroy ();

    G4VPhysicalVolume *world
	= G4TransportationManager::GetTransportationManager ()
	->GetNavigatorForTracking ()->GetWorldVolume ();

    if (! world)
	return;
    
    // FIXME: retain unchanged structure?
    ASSERT (world->GetLogicalVolume ());
    ASSERT (! world->IsReplicated ());
    VisG4Navigator::build (1, world, new VisG4VolumeTwig
			  (this, world, true, false, true));

    // Ensure that every logical volume has an unique vis attributes
    // object.  Several logical volumes may share one and the same vis
    // attrs, and some volumes might not have any attributes at all.
    // This ensures every logical volume has them -- if not, default
    // ones are created -- and that the attributes are not shared.
    // Note that we blindly apply this to all volumes in the store
    // as that is much faster than walking the geometry hierarchy.
    std::set<G4VisAttributes *> seen;
    G4LogicalVolumeStore *volumes = G4LogicalVolumeStore::GetInstance ();
    for (unsigned i = 0; i < volumes->size (); ++i)
    {
	G4LogicalVolume *volume = (*volumes) [i];
	G4VisAttributes *attrs = const_cast<G4VisAttributes *>
				 (volume->GetVisAttributes ());

	if (! attrs)
	    volume->SetVisAttributes (attrs = new G4VisAttributes);

    	if (seen.count (attrs))
	    volume->SetVisAttributes (attrs = new G4VisAttributes (*attrs));

	seen.insert (attrs);
    }

    // Make sure reps reflect the change
    IgRepSet::invalidate (this, STRUCTURE_MASK | SELF_MASK);
}
