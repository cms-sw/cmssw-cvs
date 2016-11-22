#ifndef STUDIO_IG_FILE_PROXY_H
# define STUDIO_IG_FILE_PROXY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgFileProxy
{
public:
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual ~IgFileProxy (void);

    virtual std::string fetch (const std::string &file) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // STUDIO_IG_FILE_PROXY_H
