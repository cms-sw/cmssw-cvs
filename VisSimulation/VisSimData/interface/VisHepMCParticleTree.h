// HEADER FILE


#ifndef HEP_MC_PARTICLE_TREE_H
# define HEP_MC_PARTICLE_TREE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SimTrack;
class SoSeparator;

namespace HepMC 
{
    class GenParticle;
}

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class HepMCParticleTree
{
public:
    
    HepMCParticleTree (void) {};
    HepMCParticleTree (std::vector<HepMC::GenParticle *> particles);
    HepMCParticleTree (HepMC::GenParticle *particle);
    ~HepMCParticleTree (void);    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    void makeSeparatorTree (SoSeparator *separator, const float mx, const float my, const float mz);
 
private:
    HepMC::GenParticle		       *m_particle;
    std::vector<HepMCParticleTree *>	m_daughters;   
  
    float recursiveMakeSeparatorTree (SoSeparator *separator, float x, float *nextY, float z);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_TRACKER_RECO_VIS_SIM_TRACKS_TREE_H
