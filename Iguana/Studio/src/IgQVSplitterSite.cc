//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Studio/interface/IgQVSplitterSite.h"
#include <qsplitter.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQVSplitterSite::IgQVSplitterSite (IgState *, IgSite *parent)
    : IgQSplitterSite (parent, new QSplitter (Qt::Vertical,
					      hostFrom (parent)))
{}

const char *
IgQVSplitterSite::catalogLabel (void)
{ return "QVSplitter"; }
