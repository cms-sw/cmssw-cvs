#ifndef IGUANA_GL_BROWSERS_IG_3DENUM_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DENUM_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/Ig3DFieldControl.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoSFEnum;
class QComboBox;
class QWidegt;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DEnumControl : public Ig3DFieldControl
{
    Q_OBJECT
public:
    Ig3DEnumControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    SoSFEnum *		field (void) const;
    void		setField (SoSFEnum *field);
    int			fieldValue (void) const;
    int			value (void) const;
    void		addValidValue (int value, const QString &name);
    QWidget *		widget () const;

public slots:
    void		setValue (int newval);

protected:
    virtual void	updateValue (void);
    virtual void	applyValue (void);

private slots:
    virtual void	valueChanged (int);

private:
    int			findIndex (int value);

    QComboBox		*m_value;
    std::vector<int>	m_enums;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DENUM_CONTROL_H
