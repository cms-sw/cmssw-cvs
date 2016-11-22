//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4FilterTable.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialNameFilter.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4FilterCache.h"

#ifdef ZERO
 #include "Ig_Modules/IgPython/interface/IgPythonFactory.h"
#endif

#include <classlib/utils/DebugAids.h>

#include <iostream>


//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4FilterTable::VisG4FilterTable (void)
{
}

VisG4FilterTable *VisG4FilterTable::instance (void)
{
    static VisG4FilterTable *instance__ = 0;
    if (instance__ == 0)
    {
	instance__ = new VisG4FilterTable ();	
	
	new VisG4MaterialNameFilter ();
#ifdef ZERO
	IgPythonInterpreter *myPyInt = new IgPythonInterpreter;	
	IgPythonFactory<VisG4Filter> factory (myPyInt);
	factory.loadScript ("VisG4RegExpFilter");
	factory.set ("VisG4RegExpFilter");
	VisG4Filter *regexFilter = &(factory.create());
	regexFilter->finalizeInitialization ();
#endif
    }
    ASSERT (instance__);
    
    return instance__;    
}

void 
VisG4FilterTable::add (VisG4Filter *filter)
{
    ASSERT (filter);
    ASSERT (filter->name () != "");    
        
    if (m_filterTable.find (filter->name ()) == m_filterTable.end ())
    {
	m_filterTable[filter->name ()] = filter;
    }
}
