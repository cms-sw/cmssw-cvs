#ifndef IGUANA_GL_BROWSERS_IG_CONTROL_BASE_H
# define IGUANA_GL_BROWSERS_IG_CONTROL_BASE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgControlItem.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgControlCategory;
class QWidget;
class QLayout;
class QLayoutItem;
class QString;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgControlBase : public IgControlItem
{
    Q_OBJECT
public:
    IgControlBase (IgControlCategory *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    QWidget *		makeIndentedLabel (QString label, QWidget *area);
    bool		isShown ();
    IgControlCategory * parentCategory(void) const
    { return m_parent; }

public slots:
    virtual void	attach (void);
    virtual void	detach (void);
    virtual void	update (void);
    virtual void	apply (void);

    virtual void	setShown (bool show);
    virtual void	show (void);
    virtual void	hide (void);

    virtual void	track (bool doit);

protected:
    virtual bool	hasValue (void) const;
    virtual void	updateValue (void) = 0;
    virtual void	applyValue (void) = 0;

    void		connectSignal (void);
    void		disconnectSignal (void);

    virtual int		gridRows (void) const;
    virtual void	addGridWidget (QWidget *widget, int row, int column);
    virtual void	addGridLayout (QLayout *layout, int row, int column);

private:
    static void		showLayout (QLayoutItem *item, bool show);

    IgControlCategory	*m_parent;
    bool		m_tracking;
    QObjectList		m_objects;
    bool		m_shown;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_CONTROL_BASE_H
