#ifndef IG_G4_DEMO_IG_G4_DEMO_SETUP_H
# define IG_G4_DEMO_IG_G4_DEMO_SETUP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisExamples/VisG4ExN02/interface/config.h"
# include "Iguana/Studio/interface/IgStudioSetupExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgStudioDriver;
class IgDocument;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_G4_DEMO_API VisG4ExampleSetup : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (VisG4ExampleSetup);
public:
    VisG4ExampleSetup (IgState *state);
    ~VisG4ExampleSetup (void);
    // implicit copy constructor
    // implicit assignment operator

    virtual bool	setup (IgStudioDriver *into);

private:
    IgState		*m_state;
    IgStudioDriver	*m_studio;
    IgDocument		*m_document;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_G4_DEMO_IG_G4_DEMO_SETUP_H
