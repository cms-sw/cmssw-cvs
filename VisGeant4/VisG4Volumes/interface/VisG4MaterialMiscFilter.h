#ifndef VIS_G4_VOLUMES_VIS_G4_MATERIAL_MISC_FILTER_H
# define VIS_G4_VOLUMES_VIS_G4_MATERIAL_MISC_FILTER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/VisG4MaterialFilter.h"
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoMaterial;
class IgSoMaterialMap;
class IgG4ToSoMaterialMap;
class VisSoMaterialDetails;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
/** This class provides a conversion between G4Materials and SoMaterials, based
    on the name of the G4Material. If no entry is found in the conversion map,
    the subsequent filter is called.
    To be able to work, this filter requires two env variable to be defined.
    SO_MATERIALS_BASEDIR which point to a directory containing directories 
    which contain iv files of the materials.
    MATERIALS_CONFIG is a text file which in made of entries
    ---BEGIN FILE---
    G4MaterialName SoMaterialName
    Pb metal.9
    etc...
    ---END FILE---
    which actually map G4->So.
 */
class VIS_G4_VOLUMES_API VisG4MaterialLUTFilter : public VisG4MaterialFilter
{
public:
    VisG4MaterialLUTFilter (bool enabled = false);
    virtual SoMaterial *getSoMaterial (const VisSoMaterialDetails &details, 
				       SoMaterial *soMat);
    virtual const std::string name (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    IgSoMaterialMap *m_materialMap;
    IgG4ToSoMaterialMap *m_g4ToSoMap;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_VIS_G4_MATERIAL_MISC_FILTER_H
