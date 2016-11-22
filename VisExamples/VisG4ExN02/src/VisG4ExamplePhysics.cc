//<<<<<< INCLUDES                                                       >>>>>>

#include "VisExamples/VisG4ExN02/src/VisG4ExamplePhysics.h"
#include <G4ParticleDefinition.hh>
#include <G4ParticleWithCuts.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>
#include <G4ParticleTypes.hh>
#include <G4ParticleTable.hh>
#include <G4Material.hh>
#include <G4Decay.hh>

#include <G4ComptonScattering.hh>
#include <G4GammaConversion.hh>
#include <G4PhotoElectricEffect.hh>
#include <G4MultipleScattering.hh>

#include <G4eIonisation.hh>
#include <G4eBremsstrahlung.hh>
#include <G4eplusAnnihilation.hh>

#include <G4MuIonisation.hh>
#include <G4MuBremsstrahlung.hh>
#include <G4MuPairProduction.hh>

#include <G4hIonisation.hh>

#include <G4UserSpecialCuts.hh>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4ExamplePhysics::VisG4ExamplePhysics (void)
{ defaultCutValue = 2.0*mm; SetVerboseLevel (1); }

void
VisG4ExamplePhysics::ConstructParticle (void)
{
    // pseudo-particles
    G4Geantino::GeantinoDefinition();
    G4ChargedGeantino::ChargedGeantinoDefinition();

    // gamma
    G4Gamma::GammaDefinition();
    G4Geantino::GeantinoDefinition ();

    // leptons
    //  e+/-
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();

    // mu+/-
    G4MuonPlus::MuonPlusDefinition();
    G4MuonMinus::MuonMinusDefinition();

    // nu_e
    G4NeutrinoE::NeutrinoEDefinition();
    G4AntiNeutrinoE::AntiNeutrinoEDefinition();

    // nu_mu
    G4NeutrinoMu::NeutrinoMuDefinition();
    G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();

    //  mesons
    //    light mesons
    G4PionPlus::PionPlusDefinition();
    G4PionMinus::PionMinusDefinition();
    G4PionZero::PionZeroDefinition();
    G4Eta::EtaDefinition();
    G4EtaPrime::EtaPrimeDefinition();
    G4KaonPlus::KaonPlusDefinition();
    G4KaonMinus::KaonMinusDefinition();
    G4KaonZero::KaonZeroDefinition();
    G4AntiKaonZero::AntiKaonZeroDefinition();
    G4KaonZeroLong::KaonZeroLongDefinition();
    G4KaonZeroShort::KaonZeroShortDefinition();

    //  barions
    G4Proton::ProtonDefinition();
    G4AntiProton::AntiProtonDefinition();

    G4Neutron::NeutronDefinition();
    G4AntiNeutron::AntiNeutronDefinition();
}

void
VisG4ExamplePhysics::ConstructProcess (void)
{
    AddTransportation ();

    ////////////////////////////////////////////////////////////
    theParticleIterator->reset();
    while ((*theParticleIterator) ())
    {
	G4ParticleDefinition	*p = theParticleIterator->value ();
	G4ProcessManager	*pm = p->GetProcessManager ();
	G4String		name = p->GetParticleName ();
     
	if (name == "gamma")
	{
	    pm->AddDiscreteProcess (new G4GammaConversion ());
	    pm->AddDiscreteProcess (new G4ComptonScattering ());      
	    pm->AddDiscreteProcess (new G4PhotoElectricEffect ());
	}
	else if (name == "e-")
	{
	    G4VProcess *multipleScattering	= new G4MultipleScattering ();
	    G4VProcess *ionisation		= new G4eIonisation ();
	    G4VProcess *bremsstrahlung		= new G4eBremsstrahlung ();

	    pm->AddProcess (multipleScattering);
	    pm->AddProcess (ionisation);
	    pm->AddProcess (bremsstrahlung);

	    pm->SetProcessOrdering (multipleScattering, idxAlongStep,	1);
	    pm->SetProcessOrdering (ionisation,         idxAlongStep,	2);

	    pm->SetProcessOrdering (multipleScattering, idxPostStep, 1);
	    pm->SetProcessOrdering (ionisation,         idxPostStep, 2);
	    pm->SetProcessOrdering (bremsstrahlung,     idxPostStep, 3);
	}
	else if (name == "e+")
	{
	    G4VProcess *multipleScattering	= new G4MultipleScattering ();
	    G4VProcess *ionisation		= new G4eIonisation ();
	    G4VProcess *bremsstrahlung		= new G4eBremsstrahlung ();
	    G4VProcess *annihilation		= new G4eplusAnnihilation ();

	    pm->AddProcess (multipleScattering);
	    pm->AddProcess (ionisation);
	    pm->AddProcess (bremsstrahlung);
	    pm->AddProcess (annihilation);

	    pm->SetProcessOrderingToFirst (annihilation, idxAtRest);

	    pm->SetProcessOrdering (multipleScattering, idxAlongStep, 1);
	    pm->SetProcessOrdering (ionisation,         idxAlongStep, 2);

	    pm->SetProcessOrdering (multipleScattering, idxPostStep, 1);
	    pm->SetProcessOrdering (ionisation,         idxPostStep, 2);
	    pm->SetProcessOrdering (bremsstrahlung,     idxPostStep, 3);
	    pm->SetProcessOrdering (annihilation,       idxPostStep, 4);
	}
	else if (name == "mu+" || name == "mu-")
	{
	    G4VProcess *multipleScattering	= new G4MultipleScattering ();
	    G4VProcess *bremsstrahlung		= new G4MuBremsstrahlung ();
	    G4VProcess *pairProduction		= new G4MuPairProduction ();
	    G4VProcess *ionisation		= new G4MuIonisation ();

	    pm->AddProcess (ionisation);
	    pm->AddProcess (multipleScattering);
	    pm->AddProcess (bremsstrahlung);
	    pm->AddProcess (pairProduction);

	    pm->SetProcessOrdering (multipleScattering, idxAlongStep,1);
	    pm->SetProcessOrdering (ionisation,         idxAlongStep,2);

	    pm->SetProcessOrdering (multipleScattering, idxPostStep,1);
	    pm->SetProcessOrdering (ionisation,         idxPostStep,2);
	    pm->SetProcessOrdering (bremsstrahlung,     idxPostStep,3);
	    pm->SetProcessOrdering (pairProduction,     idxPostStep,4);
	}
	else if (! p->IsShortLived ()
		 && p->GetPDGCharge () != 0.0
		 && p->GetParticleName () != "chargedgeantino")
	{
	    // all others charged particles except geantino
	    G4VProcess *multipleScattering	= new G4MultipleScattering ();
	    G4VProcess *ionisation		= new G4hIonisation ();     

	    pm->AddProcess (ionisation);
	    pm->AddProcess (multipleScattering);    

	    pm->SetProcessOrdering (multipleScattering, idxAlongStep,1);
	    pm->SetProcessOrdering (ionisation,         idxAlongStep,2);

	    pm->SetProcessOrdering (multipleScattering, idxPostStep,1);
	    pm->SetProcessOrdering (ionisation,         idxPostStep,2);
	}
    }

    ////////////////////////////////////////////////////////////
    // Add Decay Process
    theParticleIterator->reset ();
    G4Decay *decay = new G4Decay ();
    while ((*theParticleIterator) ())
    {
	G4ParticleDefinition	*p = theParticleIterator->value ();
	G4ProcessManager	*pm = p->GetProcessManager ();

	if (decay->IsApplicable (*p))
	{
	    pm->AddProcess (decay);
	    pm->SetProcessOrdering (decay, idxPostStep);
	    pm->SetProcessOrdering (decay, idxAtRest);
	}
    }
}

void
VisG4ExamplePhysics::SetCuts (void)
{ SetCutsWithDefault (); }
