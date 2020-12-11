#ifndef VIS_MAGNETIC_FIELD_VIS_CMSMAG_FIELD_TWIG_H
# define VIS_MAGNETIC_FIELD_VIS_CMSMAG_FIELD_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgTwig.h"
# include "FWCore/Framework/interface/Event.h"
# include "FWCore/Framework/interface/EventSetup.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "MagneticField/VolumeBasedEngine/interface/VolumeBasedMagneticField.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgSbField;
class VisCMSMagField;
class IgSoFieldPlane;
class SbBox3f;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCMSMagFieldTwig : public IgSimpleTwig
{
public:
    VisCMSMagFieldTwig (IgTwig *parent, IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    void		onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);

    IgSoFieldPlane*     getFieldPlane ();
    bool                fieldProxy (IgSbField *&field, SbBox3f &worldbox);

private:
    edm::ESHandle<VolumeBasedMagneticField> m_pSetup;
    VisCMSMagField      *m_fieldProxy;
    IgSoFieldPlane      *m_fieldPlane;
    bool		m_done;
    bool		m_fieldDone;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MAGNETIC_FIELD_VIS_CMSMAG_FIELD_TWIG_H
