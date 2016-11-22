//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4FilterCache.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4FilterTable.h"
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

VisG4FilterCache::VisG4FilterCache (void)
    :VisG4Filter (OR)
{
    finalizeInitialization ();
}

bool
VisG4FilterCache::isToBeDrawn (const VisSoMaterialDetails &details)
{
    return m_visualizationCache[details];	
}

bool 
VisG4FilterCache::compute (const VisSoMaterialDetails &details)
{
    ASSERT (m_nextFilter);
    bool returnValue;
    
    if (m_visualizationCache.find (details) != m_visualizationCache.end ())
    {
	returnValue = isToBeDrawn (details);
    }
    else
    {
	returnValue = m_nextFilter->compute (details);
	m_visualizationCache[details] = returnValue;	
    }
    
    return returnValue;
}

std::string
VisG4FilterCache::name (void)
{
    return std::string ("Cache");    
}
