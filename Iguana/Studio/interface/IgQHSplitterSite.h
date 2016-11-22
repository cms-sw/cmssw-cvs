#ifndef IGUANA_STUDIO_IG_Q_H_SPLITTER_SITE_H
# define IGUANA_STUDIO_IG_Q_H_SPLITTER_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/IgQSplitterSite.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQHSplitterSite : public IgQSplitterSite
{
public:
    IgQHSplitterSite (IgState *state, IgSite *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static const char *	catalogLabel (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_Q_H_SPLITTER_SITE_H
