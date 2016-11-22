//<<<<<< INCLUDES                                                       >>>>>>

#include "VisExamples/VisG4ExN02/src/VisG4ExampleGenerator.h"
#include "VisExamples/VisG4ExN02/src/VisG4ExampleDetector.h"
#include <G4ParticleTable.hh>
#include <G4ParticleGun.hh>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4ExampleGenerator::VisG4ExampleGenerator (VisG4ExampleDetector *detector)
    : m_detector (detector),
      m_gun (new G4ParticleGun (1))
{
    G4ParticleTable		*pt = G4ParticleTable::GetParticleTable ();
    G4ParticleDefinition	*p = pt->FindParticle ("proton");

    m_gun->SetParticleDefinition (p);
    m_gun->SetParticleMomentumDirection (G4ThreeVector (0, 0, 1));
    m_gun->SetParticleEnergy (3. * GeV);
}

VisG4ExampleGenerator::~VisG4ExampleGenerator (void)
{ delete m_gun; }

void
VisG4ExampleGenerator::GeneratePrimaries (G4Event *event)
{
    m_gun->SetParticlePosition
	(G4ThreeVector (0, 0, -m_detector->fullLength ()/2));
    m_gun->GeneratePrimaryVertex (event);
}
