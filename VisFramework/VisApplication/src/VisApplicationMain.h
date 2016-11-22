#ifndef VIS_APPLICATION_VIS_APPLICATION_MAIN_H
# define VIS_APPLICATION_VIS_APPLICATION_MAIN_H

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

class VisApplicationMain : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (VisApplicationMain);
public:
    VisApplicationMain (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual bool        setup (IgStudioDriver *into);

private:
    IgState		*m_state;
    IgStudioDriver	*m_studio;
    IgDocument		*m_document;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_APPLICATION_VIS_APPLICATION_MAIN_H
