//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialDensityFilter.h"
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

VisG4MaterialDensityFilter::VisG4MaterialDensityFilter (bool enabled)
    :VisG4MaterialFilter (enabled)
{
}

SoMaterial *
VisG4MaterialDensityFilter::getSoMaterial (const VisSoMaterialDetails &details, SoMaterial *soMat)
{
    ASSERT (soMat);
    
    //Use 0.7 transparency if density is lower than air.
    //FIXME: detector using gasses could have a density lower than air!
    //We must figure out a way to decide wether or not a something should be transparent.
    
    //Air is 0.00129 g/cm3...Let's keep a bit safe.
    if ( details.g4MaterialDetails ().density () < 0.005*(g/cm3) ) 
    {
	soMat->transparency = 0.7;
    }
    else
    {
	soMat->transparency = 0.0;
    }

    return soMat;
}

const std::string
VisG4MaterialDensityFilter::name (void)
{
    return "Material by density filter";
}
