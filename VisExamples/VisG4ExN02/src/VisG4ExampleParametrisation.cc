//<<<<<< INCLUDES                                                       >>>>>>

#include "VisExamples/VisG4ExN02/src/VisG4ExampleParametrisation.h"
#include <classlib/utils/DebugAids.h>
#include <G4VPhysicalVolume.hh>
#include <G4ThreeVector.hh>
#include <G4Box.hh>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4ExampleParametrisation::VisG4ExampleParametrisation (int nchambers, double zstart,
						  double zspacing, double chamberWidth,
						  double initialLen, double finalLen)
    : m_nchambers (nchambers),
      m_zstart    (zstart),
      m_zspacing  (zspacing),
      m_halfWidth (chamberWidth/2),
      m_halfFirst (initialLen/2),
      m_halfIncr  (nchambers > 0 ? (finalLen - initialLen)/(nchambers*2) : 0)
{ ASSERT (m_zspacing >= chamberWidth); }

void
VisG4ExampleParametrisation::ComputeTransformation (G4int copy, G4VPhysicalVolume *pv) const
{
    pv->SetTranslation (G4ThreeVector (0, 0, m_zstart + (copy+1) * m_zspacing));
    pv->SetRotation (0);
}

void
VisG4ExampleParametrisation::ComputeDimensions (G4Box &chamber, G4int copy,
					    const G4VPhysicalVolume *) const
{
    double halfLength = m_halfFirst + copy * m_halfIncr;
    chamber.SetXHalfLength (halfLength);
    chamber.SetYHalfLength (halfLength);
    chamber.SetZHalfLength (m_halfWidth);
}
