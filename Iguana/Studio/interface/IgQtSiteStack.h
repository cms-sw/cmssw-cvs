#ifndef IGUANA_STUDIO_IG_QT_SITE_STACK_H
# define IGUANA_STUDIO_IG_QT_SITE_STACK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Studio/interface/IgQtSite.h"
# include <qstring.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtSiteStack : public IgQtSite
{
public:
    IgQtSiteStack (void);
    QString currentName (void);    
    IgQtSiteStack	*hostWithName (const QString &name);    
    virtual void	tile (void);
    virtual void	cascade (void);    
private:
    QString 		m_currentName;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_SITE_STACK_H
