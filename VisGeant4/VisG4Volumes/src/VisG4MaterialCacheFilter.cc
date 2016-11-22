//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialCacheFilter.h"
#include <Inventor/nodes/SoMaterial.h>
#include <G4Material.hh>
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

VisG4MaterialCacheFilter::VisG4MaterialCacheFilter (bool enabled)
    :VisG4MaterialFilter (enabled)
{
}

SoMaterial *
VisG4MaterialCacheFilter::getSoMaterial (const VisSoMaterialDetails &details, SoMaterial *soMat)
{    
    ASSERT (soMat == 0);
    
    //    soMat = m_materialCache [details];
    
    if (soMat == 0)
    {
	soMat = new SoMaterial ();	
	soMat = next (details, soMat);	
	m_materialCache [details] = soMat;	
    }
    
    soMat->ref ();

    return soMat;
}

const std::string
VisG4MaterialCacheFilter::name (void)
{
    return "Material caching filter";    
}
