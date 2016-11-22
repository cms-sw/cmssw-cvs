//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4VolumePicker.h"
#include <G4VPhysicalVolume.hh>
#include <G4VPVParameterisation.hh>
#include <G4LogicalVolume.hh>
#include <G4VSolid.hh>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Pick one volume identified by @a path, i.e. a particular child
    chain, including replica choices.  All intermediate volumes are
    ignored except for maintaining the transformation matrix and
    keeping track of whether we are inside a replica or not.  The
    resulting volume information is filled into @a pv, @a lv, @a
    solid, @a pos and @a replica.  Upon call, @a pv and @a path should
    be initialised to the starting point for the traversal; no other
    arguments need to be initialised.  If @a path is empty, then
    information is returned about @a pv, which cannot be a replicated
    volume.  If @a path is non-empty, information is returned about
    that child.  */
void
VisG4VolumePicker::apply (VisG4Path		path,
			 G4VPhysicalVolume	*&pv,
			 G4LogicalVolume	*&lv,
			 G4VSolid		*&solid,
			 G4Transform3D		&pos,
			 bool			&replica)
{
    // FIXME: Cache last rendered path with transformation matrix plus
    // in-replica-flag for each path item.  We will be rendering very
    // similar paths so saving this infomration can be a huge win.

    // First handle the end of recursion: the beginning of the path.
    if (path.empty ())
    {
	ASSERT (! pv->IsReplicated ());
	lv = pv->GetLogicalVolume ();
	solid = lv->GetSolid ();
	pos = G4Transform3D::Identity;
	replica = false;
	return;
    }

    // Accumulate path one level above (terminates with code above).
    apply (path.prefix (), pv, lv, solid, pos, replica);

    // Get the daughter specified by this path trailer.
    ASSERT (path.daughter () < pv->GetLogicalVolume ()->GetNoDaughters ());
    pv = pv->GetLogicalVolume ()->GetDaughter (path.daughter ());
    lv = pv->GetLogicalVolume ();

    // Get solid and accumulate transformation.
    G4RotationMatrix	rotation;
    G4ThreeVector	translation;
    
    if (! pv->IsReplicated ())
    {
	ASSERT (! path.replica ());
	solid = lv->GetSolid ();
	rotation = pv->GetObjectRotationValue ();
	translation = pv->GetTranslation ();
    }
    else
    {
	// Replicated or parametrised physical volume.
	EAxis	 axis;
	G4int	 n;
	G4double width;
	G4double offset;
	G4bool	 consuming;
	G4int	 i = path.replica ();

	pv->GetReplicationData (axis, n, width, offset, consuming);
	ASSERT (i >= 0 && i < n);
	replica = true;

	if (G4VPVParameterisation *param = pv->GetParameterisation ())
	{
	    // Parametrised volume.
	    solid = param->ComputeSolid (i, pv);
	    param->ComputeTransformation (i, pv);
	    solid->ComputeDimensions (param, i, pv);
	    rotation = pv->GetObjectRotationValue ();
	    translation = pv->GetTranslation ();
	}
	else
	{
	    // Plain replicated volume with a linear formula.
	    switch (axis)
	    {
	    case kXAxis:
		translation = G4ThreeVector (-width*(n-1)*.5 + i*width,0,0);
		break;
	    case kYAxis:
		translation = G4ThreeVector (0,-width*(n-1)*.5 + i*width,0);
		break;
	    case kZAxis:
		translation = G4ThreeVector (0,0,-width*(n-1)*.5 + i*width);
		break;
	    case kRho:
		ASSERT (false);
		break;
	    case kPhi:
		// Minus Sign because for the physical volume we need the
		// coordinate system rotation.
		rotation.rotateZ (-(offset+(i+0.5)*width));
		break;
	    default:
		ASSERT (false);
		break;
	    } 

	    solid = lv->GetSolid ();
	    rotation = rotation.inverse ();
	}
    }

    pos = pos * G4Transform3D (rotation, translation);
}
