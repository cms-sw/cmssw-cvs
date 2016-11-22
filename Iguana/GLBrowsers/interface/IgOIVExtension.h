#ifndef GL_BROWSERS_IG_OIV_EXTENSION_H
# define GL_BROWSERS_IG_OIV_EXTENSION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/Studio/interface/IgStudioSetupExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgStudioDriver;
class IgIVView;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgOIVExtension : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (IgOIVExtension);
public:
    IgOIVExtension (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~IgOIVExtension (void);

    virtual bool        setup (IgStudioDriver *into);

private:
    IgState             *m_state;
    IgIVView            *m_view;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // GL_BROWSERS_IG_OIV_EXTENSION_H
