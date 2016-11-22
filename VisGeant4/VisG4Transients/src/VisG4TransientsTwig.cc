//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Transients/interface/VisG4TransientsTwig.h"
//#include "VisGeant4/VisG4Transients/interface/VisG4TracksTwig.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4TransientsTwig::VisG4TransientsTwig (IgTwig *parent)
    : IgSimpleTwig (parent, "Event", true, true, true)
{}

void
VisG4TransientsTwig::update (void)
{
    // FIXME: add back transients -- needs to synchronise with setup!
    // m_tracks = new VisG4TracksTwig (m_transients);
}
