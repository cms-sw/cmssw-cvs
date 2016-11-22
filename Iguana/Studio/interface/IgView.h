#ifndef IGUANA_STUDIO_IG_VIEW_H
# define IGUANA_STUDIO_IG_VIEW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgView
{
public:
    IgView (IgPage *page);
    virtual ~IgView (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    virtual IgState	*state (void);    
    IgPage		*page (void);    

    std::string 	title (void) const;
    void	 	setTitle (const char *name);
    
private:
    IgPage		*m_page;
    std::string		m_title;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_VIEW_H
