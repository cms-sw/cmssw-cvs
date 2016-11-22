#ifndef VIS_G4_FIELDS_VIS_G4_MAG_FIELD_H
# define VIS_G4_FIELDS_VIS_G4_MAG_FIELD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Fields/interface/config.h"
# include "Iguana/Inventor/interface/IgSbField.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class G4Field;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Geant4 magnetic field proxy. */
class VIS_G4_FIELDS_API VisG4MagField : public IgSbField
{
public:
    VisG4MagField (const G4Field *field = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void setField (const G4Field *field);
    virtual void evaluate (const double point [3], double field []) const;

private:
    const G4Field *m_field;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_FIELDS_VIS_G4_MAG_FIELD_H
