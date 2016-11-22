#ifndef STUDIO_IG_FILE_LOCAL_PROXY_H
# define STUDIO_IG_FILE_LOCAL_PROXY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Studio/interface/IgFileProxy.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgFileLocalProxy : public IgFileProxy
{
public:
    virtual std::string fetch (const std::string &file);
    static const char * catalogLabel (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // STUDIO_IG_FILE_LOCAL_PROXY_H
