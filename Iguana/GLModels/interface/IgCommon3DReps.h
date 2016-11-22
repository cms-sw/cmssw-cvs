#ifndef IGUANA_GL_MODELS_IG_COMMON_3D_REPS_H
# define IGUANA_GL_MODELS_IG_COMMON_3D_REPS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLModels/interface/config.h"
# include <classlib/utils/MultiMethod.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRepContext;
class IgTwig;
class IgTextModel;
class IgTextRep;
class Ig3DModel;
class Ig2DModel;
class IgLegoModel;
class IgRZModel;
class IgRPhiModel;
class IgSpareModel;
class Ig3DBaseRep;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

IGUANA_GL_MODELS_API MMM_DECLARE(IgRepContext *, doRepresent,
				 (IgTwig *twig, IgTextModel *model));
IGUANA_GL_MODELS_API MMM_DECLARE(void,		  doUpdate,
				 (IgTwig *twig, IgTextRep *rep, unsigned mask));

IGUANA_GL_MODELS_API MMM_DECLARE(IgRepContext *, doRepresent,
				 (IgTwig *twig, Ig3DModel *model));
IGUANA_GL_MODELS_API MMM_DECLARE(void,		  doUpdate,
				 (IgTwig *twig, Ig3DBaseRep *rep, unsigned mask));

IGUANA_GL_MODELS_API MMM_DECLARE(IgRepContext *, doRepresent,
				 (IgTwig *twig, Ig2DModel *model));
IGUANA_GL_MODELS_API MMM_DECLARE(IgRepContext *, doRepresent,
				 (IgTwig *twig, IgLegoModel *model));
IGUANA_GL_MODELS_API MMM_DECLARE(IgRepContext *, doRepresent,
				 (IgTwig *twig, IgRZModel *model));
IGUANA_GL_MODELS_API MMM_DECLARE(IgRepContext *, doRepresent,
				 (IgTwig *twig, IgRPhiModel *model));

//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_MODELS_IG_COMMON_3D_REPS_H
