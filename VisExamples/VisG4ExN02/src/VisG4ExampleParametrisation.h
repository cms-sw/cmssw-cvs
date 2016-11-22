#ifndef IG_G4_DEMO_IG_G4DEMO_PARAMETRISATION_H
# define IG_G4_DEMO_IG_G4DEMO_PARAMETRISATION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisExamples/VisG4ExN02/interface/config.h"
# include <G4VPVParameterisation.hh>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_G4_DEMO_API VisG4ExampleParametrisation : public G4VPVParameterisation
{
public:
    VisG4ExampleParametrisation (int nchambers, double zstart,
			     double zspacing, double chamberWidth,
			     double initialLen, double finalLen);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void ComputeTransformation (G4int copy, G4VPhysicalVolume *pv) const;
    virtual void ComputeDimensions     (G4Box &chamber, G4int copy,
					const G4VPhysicalVolume *pv) const;

private:
    int			m_nchambers;
    double		m_zstart;
    double		m_zspacing;
    double		m_halfWidth;
    double		m_halfFirst;
    double		m_halfIncr;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_G4_DEMO_IG_G4DEMO_PARAMETRISATION_H
