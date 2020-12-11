//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuon/interface/VisMuonEnergyTwig.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include <sstream>
#include <iomanip>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisMuonEnergyTwig::VisMuonEnergyTwig (IgState *state, IgTwig *parent,
				      const std::string &name /* = "" */,
				      reco::MuonEnergy energy)
    : VisQueuedTwig (state, parent, name),
      m_energy (energy)
{
}

void
VisMuonEnergyTwig::onNewEvent (const edm::Event &event,
			       const edm::EventSetup &eventSetup)
{}

void
VisMuonEnergyTwig::configChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisMuonEnergyTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    std::ostringstream text;

    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    text << "Total energy: "  << m_energy.tower << " GeV"
	 << "\nTotal energy in 3x3 tower shape: " << m_energy.towerS9 << " GeV"
	 << "\nRecHit based energy (eta-phi size don't match!) "
	 << "energy deposited in crossed ECAL crystals: " << m_energy.em << " GeV"
	 << "\nEnergy deposited in 3x3 ECAL crystal shape around crossed crystal: " << m_energy.emS9 << " GeV"
	 << "\nEnergy deposited in crossed HCAL tower (RecHits): " << m_energy.had << " GeV"
	 << "\nEnergy deposited in 3x3 HCAL tower shape around crossed tower (RecHits): " << m_energy.hadS9 << " GeV"
	 << "\nEnergy deposited in crossed HO tower (RecHits): " << m_energy.ho << " GeV"
	 << "\nEnergy deposited in 3x3 HO tower shape around crossed tower (RecHits): " << m_energy.hoS9 << " GeV";
    
    rep->setText (text.str ());
}

void
VisMuonEnergyTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisMuonEnergyTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
}

void
VisMuonEnergyTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
}

void
VisMuonEnergyTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
}
