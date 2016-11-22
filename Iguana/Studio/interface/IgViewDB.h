#ifndef IGUANA_STUDIO_IG_VIEW_DB_H
# define IGUANA_STUDIO_IG_VIEW_DB_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgPluginFactory.h"
# include "Iguana/Framework/interface/IgPluginInfo.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;
class IgView;
class IgPluginInfo;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgViewDB : public IgPluginFactory<IgView * (IgPage *)>
{
public:
    static IgViewDB *get (void);

private:
    IgViewDB (void);
    static IgViewDB s_instance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_SITE_DB_H
