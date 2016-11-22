//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialFilter.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4Details.h"

#include <Inventor/nodes/SoMaterial.h>
#include <G4Material.hh>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
std::list<VisG4MaterialFilter*> VisG4MaterialFilter::m_filterList;

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>


//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4MaterialFilter::VisG4MaterialFilter (bool enabled)
    :m_nextFilter (0),
     m_enabled (enabled)
{
    m_filterList.push_back (this);    
}

SoMaterial *
VisG4MaterialFilter::next (const VisSoMaterialDetails &details, 
			   SoMaterial *soMat)
{
    ASSERT (soMat);
    
    if (m_nextFilter != 0)
    {
	if (m_nextFilter->enabled () == true)
	    soMat = m_nextFilter->getSoMaterial (details, soMat);    
	else
	    soMat = m_nextFilter->next (details, soMat);	
    }

    return soMat;    
}

void
VisG4MaterialFilter::appendFilter (VisG4MaterialFilter *filter)
{
    m_nextFilter = filter;    
}

bool
VisG4MaterialFilter::enabled (void)
{
    return m_enabled;    
}

void
VisG4MaterialFilter::setEnabled (bool enabled)
{
    m_enabled = enabled;    
}

VisG4MaterialFilter::VisG4MaterialFilterList &
VisG4MaterialFilter::filterList (void)
{
    return m_filterList;    
}
