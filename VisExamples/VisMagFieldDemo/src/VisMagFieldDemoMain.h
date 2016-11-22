#ifndef VIS_MAG_FIELD_DEMO_VIS_JET_DEMO_MAIN_H
# define VIS_MAG_FIELD_DEMO_VIS_JET_DEMO_MAIN_H

//<<<<<< INCLUDES                                                       >>>>>>

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

class VisMagFieldDemoMain : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (VisMagFieldDemoMain);
public:
    VisMagFieldDemoMain (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual bool	setup (IgStudioDriver *into);

private:
    IgState		*m_state;
    IgStudioDriver	*m_studio;
    IgDocument		*m_document;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MAG_FIELD_DEMO_VIS_JET_DEMO_MAIN_H
