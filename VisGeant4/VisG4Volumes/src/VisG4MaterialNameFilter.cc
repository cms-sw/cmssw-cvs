//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialNameFilter.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4FilterTable.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4Details.h"
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

VisG4MaterialNameFilter::VisG4MaterialNameFilter (void)
	:VisG4Filter (),
	m_name ("Air")
{
	finalizeInitialization ();    
}

	bool
VisG4MaterialNameFilter::isToBeDrawn (const VisSoMaterialDetails &details)
{
	bool returnValue;

	if (details.g4MaterialDetails ().name () == m_name)
	{
		returnValue = true;	
	}
	else
	{
		returnValue = false;	
	}        
	return returnValue;    
}

	void 
VisG4MaterialNameFilter::setName (const char *name)
{
	m_name = name;    
}

	std::string
VisG4MaterialNameFilter::getName (void)
{
	return m_name;    
}

	std::string
VisG4MaterialNameFilter::name (void)
{
    return std::string ("ByMaterialName");    
}
