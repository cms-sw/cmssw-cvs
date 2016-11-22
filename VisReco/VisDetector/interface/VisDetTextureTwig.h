#ifndef VIS_DETECTOR_VIS_DET_TEXTURE_TWIG_H
# define VIS_DETECTOR_VIS_DET_TEXTURE_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisTexture.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DRep;
class IgRPhiRep;
class IgRZRep;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisDetTextureTwig : public IgSimpleTwig
{
public:
    VisDetTextureTwig (IgState *state, IgTwig *parent, std::string name, 
		       std::string fileName1 = "", std::string fileName2 = "", 
		       std::string fileName3 = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~VisDetTextureTwig (void);
    
    virtual void        update (Ig3DRep *rep);
    virtual void        update (IgRPhiRep *rep);
    virtual void        update (IgRZRep *rep);
    virtual void	twigChanged (void);
    
private:
    std::string         m_name;
    std::string		m_fileName1;
    std::string		m_fileName2;
    std::string 	m_fileName3; 
    bool		m_done3D;
    bool 		m_doneRPhi;
    bool		m_doneRZ;
    VisTexture		m_texture;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_DETECTOR_VIS_DET_TEXTURE_TWIG_H
