#ifndef IGUANA_STUDIO_IG_QT_BAD_SITE_ERROR_H
# define IGUANA_STUDIO_IG_QT_BAD_SITE_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgSite;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// FIXME: inherit generic exception chaining!
// FIXME: inherit something from object browser?
class IGUANA_STUDIO_API IgQtBadSiteError
{
public:
    IgQtBadSiteError (IgSite *site);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    IgSite		*m_site;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_BAD_SITE_ERROR_H
