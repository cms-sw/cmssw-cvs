#ifndef VIS_G4_VOLUMES_IG_MATERIAL_TRANSPARENCY_BY_DENSITY_FILTER_H
# define VIS_G4_VOLUMES_IG_MATERIAL_TRANSPARENCY_BY_DENSITY_FILTER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/VisG4MaterialFilter.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VIS_G4_VOLUMES_API VisG4MaterialDensityFilter : public VisG4MaterialFilter
{
public:
    VisG4MaterialDensityFilter (bool enabled);
    virtual SoMaterial *getSoMaterial (const VisSoMaterialDetails &details, SoMaterial *soMat);
    virtual const std::string name (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_IG_MATERIAL_TRANSPARENCY_BY_DENSITY_FILTER_H
