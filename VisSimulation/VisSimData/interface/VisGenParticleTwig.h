#ifndef VIS_SIM_DATA_VIS_GEN_PARTICLE_TWIG_H
# define VIS_SIM_DATA_VIS_GEN_PARTICLE_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

namespace HepMC
{
    class GenParticle;
}

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisGenParticleTwig : public IgSimpleTwig
{
public:
    VisGenParticleTwig (IgTwig *parent, 
			const std::string &name, HepMC::GenParticle &genParticle);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    int		id (void);

private:
    HepMC::GenParticle &m_genParticle;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_SIM_DATA_VIS_GEN_PARTICLE_TWIG_H
