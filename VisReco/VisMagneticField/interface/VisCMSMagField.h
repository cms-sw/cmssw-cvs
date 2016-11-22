#ifndef VIS_MAGNETIC_FIELD_VIS_CMSMAG_FIELD_H
# define VIS_MAGNETIC_FIELD_VIS_CMSMAG_FIELD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSbField.h"
# include "FWCore/Framework/interface/EventSetup.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "MagneticField/Engine/interface/MagneticField.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCMSMagField : public IgSbField
{
public:
    VisCMSMagField (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void setField (const edm::EventSetup& eventSetup);
    virtual void evaluate (const double point [3], double field []) const;
    bool	 valid (void);
    
private:
    edm::ESHandle<MagneticField> m_pSetup;
    bool	m_valid;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MAGNETIC_FIELD_VIS_CMSMAG_FIELD_H
