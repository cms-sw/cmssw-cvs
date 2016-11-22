//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4Filter.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4FilterTable.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4Details.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4Filter::VisG4Filter (enum VisG4Filter::CompositionOperations op)
    :m_nextFilter (0),
     m_op (op)
{
}

bool
VisG4Filter::compute (const VisSoMaterialDetails &details)
{
    bool thisResult;    
    bool nextResult;
    bool returnValue;

    thisResult = isToBeDrawn (details);

    if (thisResult == true && m_op == OR)
    {
	return true;	
    }

    if (thisResult == false && m_op == AND)
    {
	return false;	
    }

    if (m_nextFilter != 0)
    {
	 nextResult = m_nextFilter->isToBeDrawn (details);	
    }
    else
    {
	m_op = OR;
	nextResult = false;	
    }
    
    switch (m_op)
    {
    case AND:
	returnValue = thisResult & nextResult;
	break;	
    case OR:
	returnValue = thisResult | nextResult;
	break;
    case XOR:
	returnValue = thisResult ^ nextResult;
	break;
    default:
	returnValue = false;
	break;
    }    
    return returnValue;    
}

void
VisG4Filter::addFilter (VisG4Filter *filter)
{
    m_nextFilter = filter;    
}

VisG4Filter &
VisG4Filter::operator<< (VisG4Filter &filter)
{
    m_nextFilter = &filter;
    return *m_nextFilter;    
}

void
VisG4Filter::setCompositionOperation (enum CompositionOperations op)
{
    m_op = op;    
}

int
VisG4Filter::getCompositionOperation (void)
{
    return m_op;    
}

void 
VisG4Filter::finalizeInitialization (void)
{
    VisG4FilterTable::instance ()->add (this);    
}
