#ifndef IGUANA_STUDIO_IG_QT_LINE_EDIT_H
# define IGUANA_STUDIO_IG_QT_LINE_EDIT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include <qlineedit.h>
//Added by qt3to4:
#include <QFocusEvent>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    IgQtLineEdit (QWidget *parent, const char *name = 0);
    IgQtLineEdit (const QString &contents,
		  QWidget	*parent,
		  const char	*name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    virtual void	changed (void);
    virtual void	changed (const QString &text);
    virtual void	setChanged (bool option = true);

signals:
    void		valueChanged (const QString &text);

protected:
    void		focusOutEvent (QFocusEvent *event);

private:
    bool		m_textChanged;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_LINE_EDIT_H
