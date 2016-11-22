#ifndef VIS_SIM_BASE_VIS_G4GEOM_TWIG_H
# define VIS_SIM_BASE_VIS_G4GEOM_TWIG_H

# include "VisGeant4/VisG4Volumes/interface/VisG4GeometryTwig.h"

class IgState;

class VisG4GeomTwig : public VisG4GeometryTwig
{
public:
  VisG4GeomTwig (IgState *state, IgTwig *parent);
};

#endif // VIS_SIM_BASE_VIS_G4GEOM_TWIG_H
