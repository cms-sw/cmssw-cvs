//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Fields/interface/VisG4AnnotationsTwig.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4AnnotationsTwig::VisG4AnnotationsTwig (IgTwig *parent)
    : IgSimpleTwig (parent, "Annotations", true, true, true)
{}

void
VisG4AnnotationsTwig::update (void)
{
    // FIXME: Put stuff into the scene: annotations.
    // path.push (0, 0);
    // scene->AddRunDurationModel (new VisG4TextModel (path, text));
}
