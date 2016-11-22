#ifndef VIS_SIM_APP_VIS_SIM_MAIN_H
# define VIS_SIM_APP_VIS_SIM_MAIN_H

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

class VisSimMain : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (VisSimMain);
public:
    VisSimMain (IgState *state);
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

#endif // VIS_SIM_APP_VIS_SIM_MAIN_H
