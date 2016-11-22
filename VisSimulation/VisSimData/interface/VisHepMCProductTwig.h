#ifndef VIS_SIM_DATA_VIS_HEP_MCPRODUCT_TWIG_H
# define VIS_SIM_DATA_VIS_HEP_MCPRODUCT_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"

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

class VisHepMCProductTwig : public VisQueuedTwig
{
public:
    VisHepMCProductTwig (IgState *state, IgTwig *parent, const std::string &name = "",
			 const std::string &friendlyName = "",
			 const std::string &moduleLabel = "",
			 const std::string &instanceName = "",
			 const std::string &productName = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void	       	onNewEvent (const edm::Event &event,
					    const edm::EventSetup &eventSetup);

    using VisQueuedTwig::update;
    virtual void        	update (IgTextRep *rep);
    virtual void       		update (Ig3DRep *rep);
    virtual void       		update (IgLegoRep *rep);
    virtual void       		update (IgRPhiRep *rep);
    virtual void       		update (IgRZRep *rep);

private:
    std::string			m_text;
    std::string			m_friendlyName;
    std::string			m_moduleLabel;
    std::string			m_instanceName;
    std::string 		m_processName;
    std::map<int, HepMC::GenParticle *> m_pMap;
    std::map<int, IgSimpleTwig *> m_twigMap;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_SIM_DATA_VIS_HEP_MCPRODUCT_TWIG_H
