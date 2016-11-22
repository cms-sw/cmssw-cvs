#ifndef VIS_G4_FIELDS_VIS_G4_MAG_FIELD_TWIG_H
# define VIS_G4_FIELDS_VIS_G4_MAG_FIELD_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Fields/interface/config.h"
# include "Iguana/Framework/interface/IgTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgSbField;
class VisG4MagField;
class SbBox3f;
class IgSoFieldPlane;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Geant4 magnetic field proxy. */
class VIS_G4_FIELDS_API VisG4MagFieldTwig : public IgSimpleTwig
{
public:
    VisG4MagFieldTwig (IgTwig *parent, IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    IgSoFieldPlane*     getFieldPlane ();
    bool		fieldProxy (IgSbField *&field, SbBox3f &worldbox);

private:
    VisG4MagField	*m_fieldProxy;
    IgSoFieldPlane	*m_fieldPlane;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_FIELDS_VIS_G4_MAG_FIELD_TWIG_H
