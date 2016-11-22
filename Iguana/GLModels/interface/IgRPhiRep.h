#ifndef IGUANA_GL_MODELS_IG_RPHI_REP_H
# define IGUANA_GL_MODELS_IG_RPHI_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLModels/interface/config.h"
# include "Iguana/GLModels/interface/Ig3DBaseRep.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRPhiModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// Never ever use the fact that I can be cast silently to the
// SoSeparator base class!  Always use the methods here.

class IGUANA_GL_MODELS_API IgRPhiRep : public Ig3DBaseRep
{
public:
    IgRPhiRep (IgRPhiModel *model, IgRPhiRep *parent, IgRPhiRep *prev, SoGroup *node);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_MODELS_IG_RPHI_REP_H
