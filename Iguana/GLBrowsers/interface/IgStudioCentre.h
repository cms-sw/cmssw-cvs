#ifndef IGUANA_GL_BROWSERS_IG_STUDIO_CENTRE_H
# define IGUANA_GL_BROWSERS_IG_STUDIO_CENTRE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/Framework/interface/IgExtension.h"
# include "Iguana/Studio/interface/IgQtAppActiveService.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgControlCentre;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgStudioCentre : public IgExtension
{
    IG_DECLARE_STATE_ELEMENT (IgStudioCentre);
public:
    IgStudioCentre (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    IgControlCentre* controlCentre (void);
    void	     appActive (IgQtAppActiveMessage message);
    
private:
    IgState		*m_state;
    IgControlCentre	*m_centre;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_STUDIO_CENTRE_H
