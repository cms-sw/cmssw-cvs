#ifndef IGUANA_GL_BROWSERS_IG_3D_STRING_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3D_STRING_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/Ig3DFieldControl.h"
# include <Inventor/SbLinear.h>
# include <Inventor/fields/SoSFString.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgQtLineEdit;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DStringControl : public Ig3DFieldControl
{
    Q_OBJECT

public:
    Ig3DStringControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    SoSFString *	field (void) const;
    void		setField (SoSFString *field);
    SbString		fieldValue (void) const;

    SbString		value (void) const;
    IgQtLineEdit *	widget (void) const;

public slots:
    void		setValue (const SbString& newval); 

protected:
    virtual void	updateValue (void);
    virtual void	applyValue (void);

private slots:
    void		textChanged (const QString& newval); 

private:
    IgQtLineEdit	*m_value;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3D_STRING_CONTROL_H
