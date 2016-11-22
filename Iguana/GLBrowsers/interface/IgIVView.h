#ifndef GL_BROWSERS_IG_IV_VIEW_H
# define GL_BROWSERS_IG_IV_VIEW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/Studio/interface/IgView.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;
class IgState;
class IgBrowser;
class Ig3DBaseWindow;
class IgTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgIVView : public IgView
{
public:
    IgIVView (IgPage *page);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~IgIVView (void);

    void 	dump (unsigned level, IgTwig *twig);
    IgState*    state (void);

private:
    IgState                     *m_state;
    Ig3DBaseWindow              *m_browser;
    IgBrowser                   *m_bt;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
IgState*
IgIVView::state (void)
{ return m_state; }

#endif // GL_BROWSERS_IG_IV_VIEW_H
