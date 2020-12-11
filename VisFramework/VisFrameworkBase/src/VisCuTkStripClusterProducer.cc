//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisCuTkStripClusterProducer.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCuTkStripClusterProducer::VisCuTkStripClusterProducer (IgState *state, lat::Callback notify)
    : VisActiveConfigurable<std::string> (state, notify, "siStripClusters", "VisCuTkStripClusterProducer")
{}
