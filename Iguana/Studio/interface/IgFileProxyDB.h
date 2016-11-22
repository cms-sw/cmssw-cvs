#ifndef STUDIO_IG_FILE_PROXY_DB_H
# define STUDIO_IG_FILE_PROXY_DB_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgPluginFactory.h"
# include "Iguana/Studio/interface/IgFileProxy.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgFileProxyDB : public IgPluginFactory<IgFileProxy *(void)>
{
 public:
    static IgFileProxyDB *get (void);
    
 private:
    IgFileProxyDB (void);
    static IgFileProxyDB s_instance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // STUDIO_IG_FILE_PROXY_DB_H
