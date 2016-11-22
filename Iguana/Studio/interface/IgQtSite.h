#ifndef IGUANA_STUDIO_IG_QT_SITE_H
# define IGUANA_STUDIO_IG_QT_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgSite.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtSite : public IgSite
{
public:
    static QWidget *	selfFrom (IgSite *site);
    static QWidget *	hostFrom (IgSite *site);
    static void		host (IgSite *site, QWidget *child);

protected:
    virtual QWidget *	self (void) = 0;
    virtual QWidget *	host (void) = 0;
    virtual void	host (QWidget *child) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_SITE_H
