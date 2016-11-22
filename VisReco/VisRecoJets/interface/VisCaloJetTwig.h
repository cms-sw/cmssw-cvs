#ifndef VIS_RECO_JETS_VIS_CALO_JET_TWIG_H
# define VIS_RECO_JETS_VIS_CALO_JET_TWIG_H

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

namespace reco 
{
    class CaloJet;
}

class CaloTower;
class CaloTowerDetId;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCaloJetTwig : public VisQueuedTwig
{
public:
    VisCaloJetTwig (IgState *state, IgTwig *parent, const std::string &name = "",
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
    virtual void        	twigChanged (void);

private:
    std::string			m_text;
    const std::string		m_friendlyName;
    const std::string		m_moduleLabel;
    const std::string		m_instanceName;
    const std::string		m_processName;
    std::vector<reco::CaloJet>  m_jets;
    //    std::vector< std::pair<CaloTowerDetId, CaloTower> > m_towers;
    VisBinningStrategy		m_binning;
    VisCaloAnnotation		m_annotation;
    VisLegoScale 		m_scale;
    VisEnergyCut		m_cut;
    VisCaloEnergyScale		m_escale;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_RECO_JETS_VIS_CALO_JET_TWIG_H
