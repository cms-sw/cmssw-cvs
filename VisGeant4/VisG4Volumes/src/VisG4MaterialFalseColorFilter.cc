//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Volumes/interface/VisG4MaterialFalseColorFilter.h"
#include <Inventor/nodes/SoMaterial.h>
#include <classlib/utils/DebugAids.h>
#include <iostream>
#include <fstream>
#include <sstream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>



VisG4MaterialFalseColorFilter::VisG4MaterialFalseColorFilter (bool enabled)
    :VisG4MaterialFilter (enabled)
{
    std::string defaultConfig ("TRAK 1 1 0\n"
		 "CALO 0 0 1\n"
		 "MUON 1 0 0\n"
		 "BEAM 0 1 1\n"
		 "VCAL 0.5 1 0.5\n");
    
    std::istringstream defaultStream (defaultConfig);
    std::ifstream configuration ("colorByVolume.txt");
    
    loadConfig (defaultStream);
    loadConfig (configuration);
}

SoMaterial *
VisG4MaterialFalseColorFilter::getSoMaterial (const VisSoMaterialDetails &details, 
					      SoMaterial *soMat)
{
    std::string fullTwigPath = details.pathDetails ().path ();

    static int randomness = 0;
    randomness = (randomness + 1) % 10;
        
    for (TwigToSoMap::iterator i = m_twigToSoMap.begin (); 
	 i != m_twigToSoMap.end ();
	 i++)
    {
	std::string key = i->first;
	SoMaterial *tmpMat = i->second;	
	
	if (fullTwigPath.find (key) != std::string::npos)
	{
	    soMat->transparency = 1/(((float)randomness)+1);
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
VisG4MaterialFalseColorFilter::name (void)
{
    return std::string ("False color filter");    
}

void
VisG4MaterialFalseColorFilter::loadConfig (std::istream &stream)
{
    if (stream.good ())
    {
	while (stream.eof () != true)
	{
	    std::string name;
	    float r, g, b;	    
	    stream >> name;
	    stream >> r;
	    stream >> g;
	    stream >> b;
	    stream >> std::ws;

	    std::cerr << "*" << name << "*"
		      << r << " "
		      << g << " "
		      << b << " " << std::endl;
	    
	    SoMaterial *mat;
	    mat =  new SoMaterial;
	    mat->diffuseColor = SbVec3f (r, g, b);
	    m_twigToSoMap [name] = mat;
	}
    }
}
