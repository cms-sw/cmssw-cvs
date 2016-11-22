#ifndef VIS_G4_VOLUMES_VIS_G4_GEOMETRY_TWIG_H
# define VIS_G4_VOLUMES_VIS_G4_GEOMETRY_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/config.h"
# include "Iguana/Framework/interface/IgTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisG4VisManager;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** G4 detector geometry root twig.

    This is the root twig for the current G4 navigation geometry.
    Whenever the geometry changes the subtree is rebuilt to reflect
    the geometry hierarchy.  All subtwigs are always of type
    #VisG4VolumeTwig, and by default in "logical" mode. */
class VIS_G4_VOLUMES_API VisG4GeometryTwig : public IgSimpleTwig
{
public:
    VisG4GeometryTwig (VisG4VisManager *visManager, IgTwig *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void	update (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_VIS_G4_GEOMETRY_TWIG_H
