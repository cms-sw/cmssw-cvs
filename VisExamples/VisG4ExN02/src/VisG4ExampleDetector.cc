//<<<<<< INCLUDES                                                       >>>>>>

#include "VisExamples/VisG4ExN02/src/VisG4ExampleDetector.h"
#include "VisExamples/VisG4ExN02/src/VisG4ExampleParametrisation.h"
#include "VisExamples/VisG4ExN02/src/VisG4ExampleField.h"
#include "VisExamples/VisG4ExN02/src/VisG4ExampleSD.h"
#include <G4Material.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVParameterised.hh>
#include <G4SDManager.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const int	VisG4ExampleDetector::N_CHAMBERS		= 5;
const double	VisG4ExampleDetector::CHAMBER_SIZE		= 20 * cm;
const double	VisG4ExampleDetector::CHAMBER_SPACING	= 80 * cm;
const double	VisG4ExampleDetector::TRACKER_SIZE		= (N_CHAMBERS+1) * CHAMBER_SPACING;
const double	VisG4ExampleDetector::TARGET_SIZE		= 5 * cm;
const double	VisG4ExampleDetector::WORLD_SIZE		= 1.2 * (TARGET_SIZE + TRACKER_SIZE);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4ExampleDetector::VisG4ExampleDetector (void)
    : m_worldSol (0), m_worldLV (0), m_worldPV (0),
      m_targetSol (0), m_targetLV (0), m_targetPV (0),
      m_trackerSol (0), m_trackerLV (0), m_trackerPV (0),
      m_chamberSol (0), m_chamberLV (0), m_chamberPV (0),
      m_matPb (0), m_matXenon (0),
      m_field (new VisG4ExampleField (G4ThreeVector (0, 0, 1.*tesla)))
{}

VisG4ExampleDetector::~VisG4ExampleDetector (void)
{ delete m_field; }

G4VPhysicalVolume *
VisG4ExampleDetector::Construct (void)
{
    // MATERIAL: Air, Pb, Xenon gas
    G4Material	*air = new G4Material 	("Air", 1.29*mg/cm3, 2);
    air->AddElement (new G4Element ("Nitrogen", "N", 7., 14.01*g/mole), .7);
    air->AddElement (new G4Element ("Oxygen",   "O", 8., 16.00*g/mole), .3);

    m_matPb    = new G4Material ("Pb", 82., 207.19*g/mole, 11.35*g/cm3);
    m_matXenon = new G4Material ("XenonGas", 54., 131.29*g/mole, 5.458*mg/cm3,
					 kStateGas, 293.15*kelvin, 1*atmosphere);

    // SOLIDS, LVs and PVs
    // - World
    m_worldSol = new G4Box ("world", WORLD_SIZE/2, WORLD_SIZE/2, WORLD_SIZE/2);
    m_worldLV  = new G4LogicalVolume (m_worldSol, air, "World", 0, 0, 0);
    m_worldPV  = new G4PVPlacement (0, G4ThreeVector (), "World", m_worldLV, 0, false, 0);

    // - Target
    G4ThreeVector posTarget (0, 0, -(TARGET_SIZE + TRACKER_SIZE)/2);
    m_targetSol = new G4Box ("target", TARGET_SIZE/2, TARGET_SIZE/2, TARGET_SIZE/2);
    m_targetLV 	= new G4LogicalVolume (m_targetSol, m_matPb, "Target",0,0,0);
    m_targetPV 	= new G4PVPlacement (0, posTarget, "Target", m_targetLV, m_worldPV, false, 0);

    // - Tracker
    G4ThreeVector posTracker = G4ThreeVector (0,0,0);
    m_trackerSol = new G4Box ("tracker", TRACKER_SIZE/2, TRACKER_SIZE/2, TRACKER_SIZE/2);
    m_trackerLV  = new G4LogicalVolume (m_trackerSol, air, "Tracker",0,0,0);  
    m_trackerPV  = new G4PVPlacement (0, posTracker, "Tracker", m_trackerLV, m_worldPV, false, 0);

    // - Tracker segments: An example of Parameterised volumes; dummy
    //   values for G4Box -- modified by parameterised volume
    m_chamberSol = new G4Box ("chamber", 100*cm, 100*cm, 10*cm);
    m_chamberLV  = new G4LogicalVolume (m_chamberSol, m_matXenon, "Chamber",0,0,0);
    m_chamberPV  = new G4PVParameterised ("Chamber", m_chamberLV, m_trackerPV,
					  kZAxis, N_CHAMBERS, new VisG4ExampleParametrisation
					  (N_CHAMBERS, -TRACKER_SIZE/2 + CHAMBER_SIZE/2,
					   CHAMBER_SPACING, CHAMBER_SIZE,
					   TRACKER_SIZE/10, TRACKER_SIZE));

    // - Sensitive detectors
    G4SDManager	*sdm = G4SDManager::GetSDMpointer ();
    VisG4ExampleSD	*sd = new VisG4ExampleSD ("TrackerChamberSD");
    sdm->AddNewDetector (sd);
    m_chamberLV->SetSensitiveDetector (sd);

    // - Visualization attributes
    G4VisAttributes *white= new G4VisAttributes (G4Colour (1.0,	1.0, 1.0));
    G4VisAttributes *yellow = new G4VisAttributes (G4Colour (1.0, 1.0, 0.0));

    m_worldLV->SetVisAttributes (white);
    m_targetLV->SetVisAttributes (white);
    m_trackerLV->SetVisAttributes (white);
    m_chamberLV->SetVisAttributes (yellow);
  
    return m_worldPV;
}
