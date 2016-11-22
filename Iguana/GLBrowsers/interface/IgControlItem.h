#ifndef IGUANA_GL_BROWSERS_IG_CONTROL_ITEM_H
# define IGUANA_GL_BROWSERS_IG_CONTROL_ITEM_H

# include "Iguana/GLBrowsers/interface/config.h"
# include <QObject>

class IGUANA_GL_BROWSERS_API IgControlItem : public QObject
{
    Q_OBJECT
public:
    IgControlItem(void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static QWidget *	makeSpacedBox	(QWidget *parent, QWidget *content);
    static QWidget *	makeBox       	(QWidget *parent, QWidget *content);

    virtual bool	isDirty (void) const;
    virtual bool	isEditable (void) const;

public slots:
    virtual void	setDirty (bool value);
    virtual void	setDirty (void);
    virtual void	setClean (void);
    virtual void	setEditable (bool value);

signals:
    void		dirty (void);
    void		clean (void);
    void		editable (bool value);

private:
    bool		m_editable;
    bool		m_dirty;
};

#endif // IGUANA_GL_BROWSERS_IG_CONTROL_ITEM_H
