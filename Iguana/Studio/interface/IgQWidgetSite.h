#ifndef STUDIO_IG_QWIDGET_SITE_H
# define STUDIO_IG_QWIDGET_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/IgQtSite.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQWidgetSite : public IgQtSite
{
public:
    IgQWidgetSite (IgState *state, IgSite *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static const char *	catalogLabel (void);

protected:
    IgQWidgetSite (IgSite *parent, QWidget *widget);

    virtual QWidget *	self (void);
    virtual QWidget *	host (void);
    virtual void	host (QWidget *child);

private:
    QWidget		*m_widget;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // STUDIO_IG_QWIDGET_SITE_H
