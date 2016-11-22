#ifndef VIS_HCAL_VIS_HFREC_HIT_TWIG_H
# define VIS_HCAL_VIS_HFREC_HIT_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisBinningStrategy.h"
# include "VisFramework/VisFrameworkBase/interface/VisCaloAnnotation.h"
# include "VisFramework/VisFrameworkBase/interface/VisCaloEnergyScale.h"
# include "VisFramework/VisFrameworkBase/interface/VisEnergyCut.h"
# include "VisFramework/VisFrameworkBase/interface/VisLegoScale.h"
# include "VisFramework/VisFrameworkBase/interface/VisPhiStart.h"
# include "VisFramework/VisFrameworkBase/interface/VisPhiDelta.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class HFRecHit;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisHFRecHitTwig : public VisQueuedTwig
{
public:
    VisHFRecHitTwig (IgState *state, IgTwig *parent,
		     const std::string &name = "",
		     const std::string &friendlyName = "",
		     const std::string &moduleLabel = "",
		     const std::string &instanceName = "",
		     const std::string &processName = "",
		     unsigned color = 0x0066ff00);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);
    using VisQueuedTwig::update;
    virtual void	update (Ig3DRep *rep);
    virtual void       	update (IgLegoRep *rep);
    virtual void       	update (IgTextRep *rep);
    virtual void       	update (IgRZRep *rep);
    virtual void        twigChanged (void);

private:
    std::string         	m_name;
    const std::string		m_friendlyName;
    const std::string		m_moduleLabel;
    const std::string		m_instanceName;
    const std::string		m_processName;
    VisBinningStrategy		m_binning;
    VisCaloAnnotation		m_annotation;
    VisCaloEnergyScale		m_escale;
    VisEnergyCut		m_cut;
    VisLegoScale 		m_scale;
    VisPhiStart			m_phiStart;
    VisPhiDelta			m_phiDelta;    
    std::string			m_text;
    std::vector<HFRecHit> 	m_recHits;
    unsigned            	m_rgba;
    float 			m_maxEnergy;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_HCAL_VIS_HFREC_HIT_TWIG_H
