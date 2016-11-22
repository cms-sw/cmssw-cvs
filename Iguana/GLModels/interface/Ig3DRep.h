#ifndef IGUANA_GL_MODELS_IG_3D_REP_H
# define IGUANA_GL_MODELS_IG_3D_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLModels/interface/config.h"
# include "Iguana/GLModels/interface/Ig3DBaseRep.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// Never ever use the fact that I can be cast silently to the
// SoSeparator base class!  Always use the methods here.

class IGUANA_GL_MODELS_API Ig3DRep : public Ig3DBaseRep
{
public:
    Ig3DRep (Ig3DModel *model, Ig3DRep *parent, Ig3DRep *prev, SoGroup *node);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_MODELS_IG_3D_REP_H
