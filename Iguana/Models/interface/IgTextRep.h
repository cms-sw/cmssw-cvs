#ifndef IGUANA_MODELS_IG_TEXT_REP_H
# define IGUANA_MODELS_IG_TEXT_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Models/interface/config.h"
# include "Iguana/Framework/interface/IgRep.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTextModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_MODELS_API IgTextRep : public IgRep
{
public:
    ~IgTextRep (void);
    // default constructor
    // default copy constructor
    // default assignment operator

    virtual const std::string &	text (void) const = 0;
    virtual void		setText (const std::string &text) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_MODELS_IG_TEXT_REP_H
