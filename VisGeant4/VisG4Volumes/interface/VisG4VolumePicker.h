#ifndef VIS_G4_VOLUMES_VIS_G4_VOLUME_PICKER_H
# define VIS_G4_VOLUMES_VIS_G4_VOLUME_PICKER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/config.h"
# include "VisGeant4/VisG4Core/interface/VisG4Path.h"
# include <G4Transform3D.hh>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4VSolid;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Utility to select and compute the transformation matrix, solid and
    replica status for a particular #VisG4Path.  */
class VIS_G4_VOLUMES_API VisG4VolumePicker
{
public:
    static void apply (VisG4Path			path,
		       G4VPhysicalVolume	*&pv,
		       G4LogicalVolume		*&lv,
		       G4VSolid			*&solid,
		       G4Transform3D		&pos,
		       bool			&replica);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_VIS_G4_VOLUME_PICKER_H
