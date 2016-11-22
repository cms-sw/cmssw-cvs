#ifndef IGUANA_STUDIO_IG_Q_MAIN_WINDOW_SITE_H
# define IGUANA_STUDIO_IG_Q_MAIN_WINDOW_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/IgQtSite.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class QMainWindow;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQMainWindowSite : public IgQtSite
{
public:
    // FIXME: add capability to get the title from browser configuration set!
    IgQMainWindowSite (IgState *state, IgSite *parent);
    // IgQtMainWindow (QWidget *parent, QString title);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static const char *	catalogLabel (void);

protected:
    virtual QWidget *	self (void);
    virtual QWidget *	host (void);
    virtual void	host (QWidget *child);

private:
    QMainWindow		*m_window;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_Q_MAIN_WINDOW_SITE_H
