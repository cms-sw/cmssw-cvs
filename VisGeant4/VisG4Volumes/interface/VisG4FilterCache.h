#ifndef VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_CACHE_H
# define VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_CACHE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/config.h"
# include "VisGeant4/VisG4Volumes/interface/VisG4Filter.h"
# include "VisGeant4/VisG4Volumes/interface/VisG4Details.h"

# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VIS_G4_VOLUMES_API VisG4FilterCache : public VisG4Filter
{
public:
    VisG4FilterCache (void);
    
    virtual std::string name (void);    
    virtual bool isToBeDrawn (const VisSoMaterialDetails &details);    
    virtual bool compute (const VisSoMaterialDetails &details);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    typedef std::map<VisSoMaterialDetails, bool, 
	VisSoMaterialDetails::less> VisualizationCache;
    VisualizationCache m_visualizationCache;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_CACHE_H
