//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4Navigator.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4VolumeTwig.h"
#include <classlib/utils/DebugAids.h>
#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>
#include <algorithm>
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

void
VisG4Navigator::build (int levels, G4VPhysicalVolume *from, IgTwig *to)
{
    // Check that the volume is valid (world) volume, in particular
    // that it isn't replicated.  (FIXME: can we allow this without to
    // being a VisG4VolumeTwig?)
    ASSERT (from);
    ASSERT (from->GetLogicalVolume ());
    // ASSERT (! from->IsReplicated ());
    ASSERT (levels > 0);

    // Check that to is valid and empty
    ASSERT (to);
    ASSERT (! to->children ());

    // Add this one and descend
    build (levels, from, new VisG4VolumeTwig (to, from));
}

void
VisG4Navigator::build (int levels,
		      G4VPhysicalVolume *from,
		      VisG4VolumeTwig *twig)
{
    ASSERT (from);
    ASSERT (from->GetLogicalVolume ());
    ASSERT (twig);
    ASSERT (! twig->children ());

    if (! levels)
	return;

    G4LogicalVolume		*mother = from->GetLogicalVolume ();
    std::set<G4LogicalVolume *>	seen;

    for (int i = 0; i < mother->GetNoDaughters (); ++i)
    {
	G4VPhysicalVolume *daughter = mother->GetDaughter (i);
	G4LogicalVolume   *logdaughter = daughter->GetLogicalVolume ();

	// Process this physical volume.  We do not deal with replicas
	// here, they get mapped into this twig when the scene graph
	// is rendered.  However we do deal with logical vs. physical
	// mapping here, creating children in the mode required.
	if (twig->logical () && ! seen.insert (logdaughter).second)
	    continue;

	VisG4VolumeTwig *t = new VisG4VolumeTwig (twig, daughter);
	t->logical (twig->logical ());
	build (levels-1, daughter, t);
    }
}

/** Locate in @a volume the logical child index of @a index'th
    physical child.  */
unsigned
VisG4Navigator::findLogical (G4VPhysicalVolume *volume, unsigned index)
{
    ASSERT (volume);
    ASSERT (volume->GetLogicalVolume ());
    ASSERT (int(index) < volume->GetLogicalVolume ()->GetNoDaughters ());

    G4LogicalVolume *mother = volume->GetLogicalVolume ();
    G4LogicalVolume *desired = mother->GetDaughter (index)->GetLogicalVolume ();

    // Compute the logical index of the first node that matches
    // `desired'.  Avoid dynamic memory allocation, this code is
    // getting hit all the time.  The static array is ugly, but it
    // speeds things up considerably.  We know the array tends to be
    // small, so searching it is fast.

    ASSERT (desired);
    static std::vector<G4LogicalVolume *> seen;

    seen.clear ();
    seen.reserve (index);
    for (int i = 0; i < mother->GetNoDaughters (); ++i)
    {
	// Get the child
	G4VPhysicalVolume *daughter = mother->GetDaughter (i);
	G4LogicalVolume   *logdaughter = daughter->GetLogicalVolume ();

	// Remember this volume; if we've already seen it, skip;
	// otherwise add it to the vector.
	if (std::find (seen.begin (), seen.end (), logdaughter) == seen.end ())
	{
	    if (logdaughter == desired)
		return seen.size ();

	    seen.push_back (logdaughter);
	}
    }

    ASSERT (false);
    return 0;
}
