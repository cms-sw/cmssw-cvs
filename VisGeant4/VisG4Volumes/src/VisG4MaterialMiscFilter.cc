//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialMiscFilter.h"
#include "Iguana/Controls/interface/IgG4ToSoMaterialMap.h"
#include "Iguana/Controls/interface/IgSoMaterialMap.h"

#include <G4Material.hh>

#include <Inventor/nodes/SoMaterial.h>

#include <classlib/iobase/DirIterator.h>
#include <classlib/iobase/Filename.h>

#include <iostream>
#include <cstdlib>
#include <fstream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4MaterialLUTFilter::VisG4MaterialLUTFilter (bool enabled)
    :VisG4MaterialFilter (enabled)
{
    m_materialMap = IgSoMaterialMapSingleton::instance ();
    m_g4ToSoMap = IgG4ToSoMaterialMapSingleton::instance ();    
}

SoMaterial *
VisG4MaterialLUTFilter::getSoMaterial (const VisSoMaterialDetails &details, SoMaterial *soMat)
{
    std::string g4Name = details.g4MaterialDetails ().name ();
    ASSERT (m_g4ToSoMap);
    ASSERT (m_materialMap);        
    ASSERT (soMat);

    if (m_g4ToSoMap->find (g4Name.c_str ()) != m_g4ToSoMap->end ()) 
    {
	std::string &soName = (*m_g4ToSoMap) [g4Name.c_str ()];

	SoMaterial *tmpMat = (*m_materialMap) [soName.c_str ()];
	if (tmpMat != 0)
	{
	    soMat->transparency = tmpMat->transparency;
	    soMat->shininess = tmpMat->shininess;
	    soMat->ambientColor = tmpMat->ambientColor;
	    soMat->diffuseColor = tmpMat->diffuseColor;
	    soMat->specularColor = tmpMat->specularColor;
	    soMat->emissiveColor = tmpMat->emissiveColor;
	}
    }
    return next (details, soMat);
}

const std::string
VisG4MaterialLUTFilter::name (void)
{
    return "G4 material look-up table filter";    
}
