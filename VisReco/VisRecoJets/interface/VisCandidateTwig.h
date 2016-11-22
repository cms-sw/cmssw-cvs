#ifndef VIS_RECO_JETS_VIS_CANDIDATE_TWIG_H
# define VIS_RECO_JETS_VIS_CANDIDATE_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisCaloAnnotation.h"
# include "VisFramework/VisFrameworkBase/interface/VisCaloEnergyScale.h"
# include "VisFramework/VisFrameworkBase/interface/VisEnergyCut.h"
# include "VisFramework/VisFrameworkBase/interface/VisLegoScale.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

namespace reco
{
    class Candidate;
}

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCandidateTwig : public VisQueuedTwig
{
public:
    VisCandidateTwig (IgState *state, IgTwig *parent, const std::string &name = "",
		      const std::string &friendlyName = "",
		      const std::string &moduleLabel = "",
		      const std::string &instanceName = "",
		      const std::string &processName = "");

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
    virtual void        	scaleChanged (void);

private:
    std::string			m_text;
    const std::string		m_friendlyName;
    const std::string		m_moduleLabel;
    const std::string		m_instanceName;
    const std::string		m_processName; 
    std::vector<const reco::Candidate *> m_candidates;
    VisLegoScale 		m_scale;
    VisEnergyCut		m_cut;
    VisCaloEnergyScale		m_escale;
    VisCaloAnnotation		m_annotation;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_RECO_JETS_VIS_CANDIDATE_TWIG_H
