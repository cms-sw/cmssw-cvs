#ifndef IGUANA_STUDIO_IG_Q_SPLITTER_SITE_H
# define IGUANA_STUDIO_IG_Q_SPLITTER_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/IgQtSite.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class QSplitter;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQSplitterSite : public IgQtSite
{
public:
    // FIXME: get orientation from browser configuration!
    IgQSplitterSite (IgState *state, IgSite *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static const char *	catalogLabel (void);

protected:
    IgQSplitterSite (IgSite *parent, QSplitter *widget);

    virtual QWidget *	self (void);
    virtual QWidget *	host (void);
    virtual void	host (QWidget *child);

private:
    QSplitter		*m_widget;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_Q_SPLITTER_SITE_H
