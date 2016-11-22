#ifndef IGUANA_GL_BROWSERS_IG_3DROTATIONF_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DROTATIONF_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/Ig3DFieldControl.h"
# include <Inventor/SbLinear.h>
# include <Inventor/fields/SoSFRotation.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DVectorControl;
class Ig3DFloatControl;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DRotationFControl : public Ig3DFieldControl
{
    Q_OBJECT
public:
    Ig3DRotationFControl (IgControlCategory	*pane,
		          QString		label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    // Field and field value access
    SoSFRotation *	field (void) const;
    void		setField (SoSFRotation *field);
    SbRotation		fieldValue (void) const;

    // Control value and change access
    SbVec3f		axis (void) const;
    float		angle (void) const;
    SbRotation		value (void) const;


    Ig3DVectorControl *	axisControl (void) const;
    Ig3DFloatControl *  angleControl (void) const;
    
    QString		getSettingString (void) const;
    void		setSettingString (QString &setting);
public slots:
    virtual void	setValue (const SbVec3f &axis, float angle);
    virtual void	setValue (const SbRotation &newval);
    virtual void	setAxis (const SbVec3f &value);
    virtual void	setAngle (float value);
    virtual void	emitSettingsChanged (void);

protected:
    virtual void	updateValue (void);
    virtual void	applyValue (void);

private:
    Ig3DVectorControl	*m_axis;
    Ig3DFloatControl	*m_angle;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DROTATIONF_CONTROL_H
