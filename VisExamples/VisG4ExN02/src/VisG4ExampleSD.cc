//<<<<<< INCLUDES                                                       >>>>>>

#include "VisExamples/VisG4ExN02/src/VisG4ExampleSD.h"
#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4ThreeVector.hh>
#include <G4SDManager.hh>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4ExampleSD::VisG4ExampleSD (const std::string &name)
    : G4VSensitiveDetector (name)
{ collectionName.insert ("trackerCollection"); }

void
VisG4ExampleSD::Initialize (G4HCofThisEvent *HCE)
{
    static int HCID = G4SDManager::GetSDMpointer ()->GetCollectionID (collectionName [0]);
    m_hits = new VisG4ExampleHitsCollection (SensitiveDetectorName, collectionName [0]);
    HCE->AddHitsCollection (HCID, m_hits);
}

bool
VisG4ExampleSD::ProcessHits (G4Step *step, G4TouchableHistory *)
{
    if (! step->GetTotalEnergyDeposit ())
	return false;

    VisG4ExampleHit	*hit = new VisG4ExampleHit;
    hit->setTrackId  (step->GetTrack ()->GetTrackID ());
    hit->setChamber  (step->GetPreStepPoint ()->GetTouchable ()
		      ->GetReplicaNumber ());
    hit->setEnergy   (step->GetTotalEnergyDeposit ());
    hit->setPosition (step->GetPostStepPoint ()->GetPosition ());

    m_hits->insert (hit);
    return true;
}
