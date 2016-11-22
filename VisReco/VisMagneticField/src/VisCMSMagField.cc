//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMagneticField/interface/VisCMSMagField.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCMSMagField::VisCMSMagField (void)
    : m_valid (false)
{
}

void
VisCMSMagField::setField (const edm::EventSetup& eventSetup)
{ 
    try 
    {
	eventSetup.get<IdealMagneticFieldRecord> ().get (m_pSetup);
    }
    catch (...) 
    {
    }
    m_pSetup.isValid () ? m_valid = true : m_valid = false;
}

void
VisCMSMagField::evaluate (const double point [3], double field [3]) const
{
    //point values are in meters and field is in cm, so convert it to cm first
    GlobalVector b = m_pSetup->inTesla (GlobalPoint (point [0]*(m/cm) , point [1]*(m/cm), point [2]*(m/cm)));
    
    field [0] = b.x ();
    field [1] = b.y ();
    field [2] = b.z ();
}

bool
VisCMSMagField::valid (void)
{
    return m_valid;
}
