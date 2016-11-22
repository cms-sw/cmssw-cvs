//<<<<<< INCLUDES                                                       >>>>>>

#include "VisExamples/VisG4ExN02/src/VisG4ExampleField.h"
#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4ExampleField::VisG4ExampleField (G4ThreeVector fieldVector /* = G4ThreeVector () */)
    : G4UniformMagField (fieldVector)
{
    G4FieldManager *fm = G4TransportationManager::GetTransportationManager ()
			 ->GetFieldManager ();
    fm->SetDetectorField (this);
    fm->CreateChordFinder (this);
}
