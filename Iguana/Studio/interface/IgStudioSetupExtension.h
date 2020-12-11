#ifndef IGUANA_STUDIO_IG_STUDIO_SETUP_EXTENSION_H
# define IGUANA_STUDIO_IG_STUDIO_SETUP_EXTENSION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgStudioDriver;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgStudioSetupExtension : public IgExtension
{
public:
    // implicit constructor
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual bool	setup (IgStudioDriver *into);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_STUDIO_SETUP_EXTENSION_H
