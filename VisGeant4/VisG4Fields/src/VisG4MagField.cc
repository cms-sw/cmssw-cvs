//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Fields/interface/VisG4MagField.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "G4Field.hh"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4MagField::VisG4MagField (const G4Field *field /* = 0 */)
    : m_field (field)
{}

void
VisG4MagField::setField (const G4Field *field)
{ m_field = field; }

void
VisG4MagField::evaluate (const double point [3], double field [3]) const
{
    double pt [3] = { point [0] * m, point [1] * m, point [2] * m };
    m_field->GetFieldValue (pt, field);
    field [0] /= tesla;
    field [1] /= tesla;
    field [2] /= tesla;
}
