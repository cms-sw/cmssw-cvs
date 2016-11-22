#ifndef VIS_ECAL_VIS_ECAL_UNCALIBRATED_REC_HIT_H
# define VIS_ECAL_VIS_ECAL_UNCALIBRATED_REC_HIT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisBinningStrategy.h"
# include "VisFramework/VisFrameworkBase/interface/VisCaloAnnotation.h"
# include "VisFramework/VisFrameworkBase/interface/VisCaloEnergyScale.h"
# include "VisFramework/VisFrameworkBase/interface/VisEcalEnergyCut.h"
# include "VisFramework/VisFrameworkBase/interface/VisLegoScale.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class EcalUncalibratedRecHit;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisEcalUncalibratedRecHitTwig : public VisQueuedTwig
{
public:
    VisEcalUncalibratedRecHitTwig (IgState *state, IgTwig *parent, 
				   const std::string &name = "",
				   const std::string &friendlyName = "",
				   const std::string &moduleLabel = "",
				   const std::string &instanceName = "",
				   const std::string &processName = "",
				   unsigned color = 0xff009c00);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructorvirtual 
    void        onNewEvent (const edm::Event &event,
			    const edm::EventSetup &eventSetup);
 
    using VisQueuedTwig::update;
    virtual void        update (Ig3DRep *rep);
    virtual void        update (IgLegoRep *rep);
    virtual void        update (IgTextRep *rep);
    virtual void        update (IgRPhiRep *rep);
    virtual void        update (IgRZRep *rep);
    virtual void        twigChanged (void);
    
private:
    std::string                 m_name;
    const std::string		m_friendlyName;
    const std::string		m_moduleLabel;
    const std::string		m_instanceName;
    const std::string           m_processName;
    VisBinningStrategy		m_binning;
    VisLegoScale 		m_scale;
    VisEcalEnergyCut		m_cut;
    VisCaloEnergyScale		m_escale;
    VisCaloAnnotation		m_annotation;
    std::string 		m_text;
    std::vector<EcalUncalibratedRecHit>     m_recHits;
    std::string                 m_detID;
    int                         m_subdetn;
    unsigned                    m_rgba;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_ECAL_VIS_ECAL_UNCALIBRATED_REC_HIT_H
