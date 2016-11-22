#ifndef IGUANA_MODELS_XTYPEINFO_H
# define IGUANA_MODELS_XTYPEINFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Models/interface/config.h"
# include <classlib/utils/XTypeInfo.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgTextRep;
class IgSimpleTextRep;
class IgTextModel;
class IgSimpleTextModel;

XTYPEINFO_DECLARE (IGUANA_MODELS_API, IgTextRep);
XTYPEINFO_DECLARE (IGUANA_MODELS_API, IgSimpleTextRep);
XTYPEINFO_DECLARE (IGUANA_MODELS_API, IgTextModel);
XTYPEINFO_DECLARE (IGUANA_MODELS_API, IgSimpleTextModel);

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_MODELS_XTYPEINFO_H
