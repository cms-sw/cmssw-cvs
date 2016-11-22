#ifndef VIS_G4_FIELDS_VIS_G4_ANNOTATIONS_TWIG_H
# define VIS_G4_FIELDS_VIS_G4_ANNOTATIONS_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Fields/interface/config.h"
# include "Iguana/Framework/interface/IgTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Placeholder for G4-related annotation objects.  To be replaced by
    more generic content management. */
class VIS_G4_FIELDS_API VisG4AnnotationsTwig : public IgSimpleTwig
{
public:
    VisG4AnnotationsTwig (IgTwig *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void	update (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_FIELDS_VIS_G4_ANNOTATIONS_TWIG_H
