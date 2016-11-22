#ifndef VIS_G4_VOLUMES_VIS_G4MATERIAL_FALSE_COLOR_FILTER_H
# define VIS_G4_VOLUMES_VIS_G4MATERIAL_FALSE_COLOR_FILTER_H

//<<<<<< INCLUDES                                                       >>>>>>
#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialFilter.h"
#include <map>
#include <string>
#include <istream>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class SoMaterial;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisG4MaterialFalseColorFilter : public VisG4MaterialFilter
{
public:
    VisG4MaterialFalseColorFilter (bool enabled = false);
    virtual SoMaterial *getSoMaterial (const VisSoMaterialDetails &details, 
			       SoMaterial *soMat);
    virtual const std::string name (void);    

    /// Configuration for this filter found in colorByVolume.txt
    void loadConfig (std::istream &stream);    
private:
    typedef std::map <std::string, SoMaterial *> TwigToSoMap;
    TwigToSoMap m_twigToSoMap;    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_VIS_G4MATERIAL_FALSE_COLOR_FILTER_H
