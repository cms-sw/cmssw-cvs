#ifndef VIS_ECAL_VIS_BASIC_CLUSTER_COLLECTION_TWIG_H
# define VIS_ECAL_VIS_BASIC_CLUSTER_COLLECTION_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisBinningStrategy.h"
# include "VisFramework/VisFrameworkBase/interface/VisCaloAnnotation.h"
# include "VisFramework/VisFrameworkBase/interface/VisCaloEnergyScale.h"
# include "VisFramework/VisFrameworkBase/interface/VisEnergyCut.h"
# include "VisFramework/VisFrameworkBase/interface/VisLegoScale.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

class EcalRecHit;

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisBasicClusterCollectionTwig : public VisQueuedTwig
{
public:
    VisBasicClusterCollectionTwig (IgState *state, IgTwig *parent, const std::string &name = "",
				   const std::string &friendlyName = "",
				   const std::string &moduleLabel = "",
				   const std::string &instanceName = "",
				   const std::string &processName = "",
				   unsigned color = 0xff009c00);
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
    std::string			m_text;
    const std::string		m_friendlyName;
    const std::string		m_moduleLabel;
    const std::string		m_instanceName;
    const std::string 		m_processName;
    VisBinningStrategy		m_binning;
    VisLegoScale 		m_scale;
    VisEnergyCut		m_cut;
    VisCaloEnergyScale		m_escale;
    VisCaloAnnotation		m_annotation;
    std::vector<reco::BasicCluster>  m_clusters;
    std::vector<EcalRecHit>	m_clusterRecHits;
    unsigned                    m_rgba;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_ECAL_VIS_BASIC_CLUSTER_COLLECTION_TWIG_H
