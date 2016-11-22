#ifndef VIS_ECAL_VIS_PRESHOWER_CLUSTER_TWIG_H
# define VIS_ECAL_VIS_PRESHOWER_CLUSTER_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisBinningStrategy.h"
# include "VisFramework/VisFrameworkBase/interface/VisCaloEnergyScale.h"
# include "VisFramework/VisFrameworkBase/interface/VisEcalEnergyCut.h"
# include "VisFramework/VisFrameworkBase/interface/VisLegoScale.h"
# include "VisFramework/VisFrameworkBase/interface/VisEcalRMax.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

namespace reco 
{
  class PreshowerCluster;
}

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisPreshowerClusterTwig : public VisQueuedTwig
{
public:
    VisPreshowerClusterTwig (IgState *state, IgTwig *parent, 
			     const std::string &name = "",
			     const std::string &friendlyName = "",
			     const std::string &moduleLabel = "",
			     const std::string &instanceName = "",
			     const std::string &processName = "",
			     unsigned color = 0xff009c00);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    void        onNewEvent (const edm::Event &event,
			    const edm::EventSetup &eventSetup);
 
    using VisQueuedTwig::update;
    virtual void        update (Ig3DRep *rep);
    virtual void        update (IgLegoRep *rep);
    virtual void        update (IgTextRep *rep);
    virtual void        update (IgRZRep *rep);
    virtual void        twigChanged (void);

private:
    std::string                 m_name;
    const std::string		m_friendlyName;
    const std::string		m_moduleLabel;
    const std::string		m_instanceName;
    const std::string 		m_processName;
    VisBinningStrategy		m_binning;
    VisLegoScale 		m_scale;
    VisEcalEnergyCut		m_cut;
    VisCaloEnergyScale		m_escale;
    VisEcalRMax			m_rMax;
    std::string                 m_text;
    std::vector<reco::PreshowerCluster> m_clusters;
    unsigned                    m_rgba;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_ECAL_VIS_PRESHOWER_CLUSTER_TWIG_H
