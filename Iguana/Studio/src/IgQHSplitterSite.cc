//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Studio/interface/IgQHSplitterSite.h"
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

IgQHSplitterSite::IgQHSplitterSite (IgState *, IgSite *parent)
    : IgQSplitterSite (parent, new QSplitter (Qt::Horizontal,
					      hostFrom (parent)))
{}

const char *
IgQHSplitterSite::catalogLabel (void)
{ return "QHSplitter"; }
