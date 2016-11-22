//<<<<<< INCLUDES                                                       >>>>>>

#include "VisExamples/VisG4ExN02/src/VisG4ExampleHit.h"
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

G4Allocator<VisG4ExampleHit> VisG4ExampleHitAllocator;

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4ExampleHit::VisG4ExampleHit (void)
    : m_trackId (-1),
      m_chamber (-1),
      m_energy (0.),
      m_position (G4ThreeVector ())
{}

void
VisG4ExampleHit::Draw (void)
{
    if (G4VVisManager *pVVisManager = G4VVisManager::GetConcreteInstance ())
    {
	G4Circle circle (m_position);
	circle.SetScreenSize (0.04);
	circle.SetFillStyle  (G4Circle::filled);
	circle.SetVisAttributes (G4VisAttributes (G4Colour (1.,0.,0.)));

	pVVisManager->Draw (circle);
    }
}

void
VisG4ExampleHit::Print (void)
{
    std::cout << " track id: " << m_trackId
	      << " chamber: " << m_chamber
	      << " energy: " << m_energy
	      << " position: " << m_position;
}
