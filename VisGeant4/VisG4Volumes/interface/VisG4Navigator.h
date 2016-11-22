#ifndef VIS_G4_VOLUMES_VIS_G4_NAVIGATOR_H
# define VIS_G4_VOLUMES_VIS_G4_NAVIGATOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class G4VPhysicalVolume;
class G4LogicalVolume;
class VisG4Path;
class VisG4VolumeTwig;
class IgTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Helper utilties for building twig trees from Geant4 volumes.  Used
    by #VisG4VolumeTwig.  */
class VIS_G4_VOLUMES_API VisG4Navigator
{
public:
    static void		build (int levels, G4VPhysicalVolume *from,
			       IgTwig *parent);
    static void		build (int levels, G4VPhysicalVolume *from,
			       VisG4VolumeTwig *twig);
    static unsigned	findLogical (G4VPhysicalVolume *volume, unsigned index);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_VIS_G4_NAVIGATOR_H
