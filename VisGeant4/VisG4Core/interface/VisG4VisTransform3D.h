#ifndef VIS_G4_CORE_VIS_G4_VIS_TRANSFORM_3D_H
# define VIS_G4_CORE_VIS_G4_VIS_TRANSFORM_3D_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Core/interface/config.h"
# include <Inventor/SbLinear.h>
# include <G4Transform3D.hh>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Utility to convert Geant4 transformation matrix into an
    OpenInventor one.  */
class VIS_G4_CORE_API VisG4VisTransform3D : public G4Transform3D
{
public:
    VisG4VisTransform3D (const G4Transform3D &t);
    /**/	operator SbMatrix (void) const;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
VisG4VisTransform3D::VisG4VisTransform3D (const G4Transform3D &t)
    : G4Transform3D (t)
{}

inline
VisG4VisTransform3D::operator SbMatrix (void) const
{
    return SbMatrix (xx_,  yx_,  zx_, 0.,
		     xy_,  yy_,  zy_, 0.,
		     xz_,  yz_,  zz_, 0.,
		     dx_ / m, dy_ / m, dz_ / m, 1.);
}

#endif // VIS_G4_CORE_VIS_G4_VIS_TRANSFORM_3D_H
