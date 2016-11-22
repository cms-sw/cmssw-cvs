#ifndef VIS_DETECTOR_VIS_DET_IV_TWIG_H
# define VIS_DETECTOR_VIS_DET_IV_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoNode;
class Ig3DRep;
class IgTextRep;
class IgRPhiRep;
class IgRZRep;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisDetIvTwig : public IgSimpleTwig
{
public:
    enum VisDetectorRep { VisAllDet, Vis3DDet, VisRPhiDet, VisRZDet };

    VisDetIvTwig (IgState *state, IgTwig *parent, std::string name, 
		  std::string fileName1, std::string fileName2 = "", 
		  std::string fileName3 = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~VisDetIvTwig (void);
    
    virtual void        update (IgTextRep *rep);
    virtual void        update (Ig3DRep *rep);
    virtual void        update (IgRPhiRep *rep);
    virtual void        update (IgRZRep *rep);
    
private:
    std::string         m_name;
    std::vector<SoNode *> m_dets;
    bool		m_done3D;
    bool 		m_doneRPhi;
    bool		m_doneRZ;

    SoNode*		emptyNode (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_DETECTOR_VIS_DET_IV_TWIG_H
