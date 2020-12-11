#ifndef VIS_ECAL_VIS_SUPER_CLUSTER_TWIG_H
# define VIS_ECAL_VIS_SUPER_CLUSTER_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisBinningStrategy.h"
# include "VisFramework/VisFrameworkBase/interface/VisCaloAnnotation.h"
# include "VisFramework/VisFrameworkBase/interface/VisCaloEnergyScale.h"
# include "VisFramework/VisFrameworkBase/interface/VisEnergyCut.h"
# include "VisFramework/VisFrameworkBase/interface/VisLegoScale.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class EcalRecHit;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisSuperClusterTwig : public VisQueuedTwig
{
public:
    VisSuperClusterTwig (IgState *state, IgTwig *parent, const std::string &name,
			 std::vector <EcalRecHit> &recHits);
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
    virtual void        	twigChanged (void);

private:
    std::string                 m_name;
    VisBinningStrategy		m_binning;
    VisLegoScale 		m_scale;
    VisEnergyCut		m_cut;
    VisCaloEnergyScale		m_escale;
    VisCaloAnnotation		m_annotation;
    std::vector <EcalRecHit>	m_recHits;
    unsigned                    m_rgba;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_ECAL_VIS_SUPER_CLUSTER_TWIG_H
