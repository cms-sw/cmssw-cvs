#ifndef VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_BY_MATERIAL_NAME_H
# define VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_BY_MATERIAL_NAME_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/VisG4Filter.h"

# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VIS_G4_VOLUMES_API VisG4MaterialNameFilter : public VisG4Filter
{
public:
    VisG4MaterialNameFilter (void);
    virtual std::string name (void);    
    
    bool isToBeDrawn (const VisSoMaterialDetails &details);
    std::string getName (void);
    void setName (const char *name);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    std::string m_name;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_BY_MATERIAL_NAME_H
