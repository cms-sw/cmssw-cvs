#ifndef VIS_G4_VOLUMES_VIS_G4_MATERIAL_CACHE_FILTER_H
# define VIS_G4_VOLUMES_VIS_G4_MATERIAL_CACHE_FILTER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/VisG4MaterialFilter.h"
# include "VisGeant4/VisG4Volumes/interface/VisG4Details.h"
# include <map>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class G4Material;
class SoMaterial;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VIS_G4_VOLUMES_API VisG4MaterialCacheFilter : public VisG4MaterialFilter
{
public:
    VisG4MaterialCacheFilter (bool enabled = true);
    virtual SoMaterial *getSoMaterial (const VisSoMaterialDetails &details, 
				       SoMaterial *soMat);
    virtual const std::string name (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    typedef std::map<const VisSoMaterialDetails, 
	SoMaterial *, VisSoMaterialDetails::less> MaterialCache;
    MaterialCache m_materialCache;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_VIS_G4_MATERIAL_CACHE_FILTER_H
