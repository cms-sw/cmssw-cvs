#ifndef VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_CATEGORY_H
# define VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/config.h"
# include "Iguana/GLBrowsers/interface/IgControlCategory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
class IgPage;
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VIS_G4_VOLUMES_API VisG4FilterCategory : public IgControlCategory
{
public:
    VisG4FilterCategory (IgPage *page);

    //virtual void	attach (void);
    //virtual void 	detach (void);
    //virtual void	update (void);
    //virtual void	apply (void);    

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_IG_VISUALIZATION_FILTER_CATEGORY_H
