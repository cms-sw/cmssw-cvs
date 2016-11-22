#ifndef VIS_EVENT_SETUP_XTYPEINFO_H
# define VIS_EVENT_SETUP_XTYPEINFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/xtypeinfo.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisCaloGeometryTwig;
class VisCSCGeometryTwig;
class VisDTGeometryTwig;
class VisRPCGeometryTwig;
class VisTrackerGeometryTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

XTYPEINFO_DECLARE (, VisCaloGeometryTwig);
XTYPEINFO_DECLARE (, VisCSCGeometryTwig);
XTYPEINFO_DECLARE (, VisDTGeometryTwig);
XTYPEINFO_DECLARE (, VisRPCGeometryTwig);
XTYPEINFO_DECLARE (, VisTrackerGeometryTwig);

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_EVENT_SETUP_XTYPEINFO_H
