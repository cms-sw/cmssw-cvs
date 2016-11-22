#ifndef VIS_MUON_VIS_MUON_ENERGY_TWIG_H
# define VIS_MUON_VIS_MUON_ENERGY_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "DataFormats/MuonReco/interface/MuonEnergy.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisMuonEnergyTwig : public VisQueuedTwig
{
public:
    VisMuonEnergyTwig (IgState *state, IgTwig *parent,
		       const std::string &name,
		       reco::MuonEnergy energy);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);
    using VisQueuedTwig::update;
    virtual void	update (Ig3DRep *rep);
    virtual void       	update (IgLegoRep *rep);
    virtual void       	update (IgTextRep *rep);
    virtual void       	update (IgRPhiRep *rep);
    virtual void       	update (IgRZRep *rep);
    virtual void        configChanged (void);

private:
    reco::MuonEnergy 	m_energy;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MUON_VIS_MUON_ENERGY_TWIG_H
