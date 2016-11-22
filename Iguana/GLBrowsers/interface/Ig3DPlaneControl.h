#ifndef IGUANA_GL_BROWSERS_IG_3DPLANE_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DPLANE_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/Ig3DFieldControl.h"
# include "Iguana/GLBrowsers/interface/Ig3DAxisRotationControl.h"
# include "Iguana/GLBrowsers/interface/IgPlaneDirection.h"
# include <Inventor/SbLinear.h>
# include <Inventor/fields/SoSFPlane.h>


//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoSFPlane;
class Ig3DAxisRotationControl;
class Ig3DVectorControl;
class Ig3DFloatControl;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DPlaneControl : public Ig3DFieldControl
{
    Q_OBJECT
public:
    Ig3DPlaneControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    SoSFPlane*	field (void) const;
    void		setField (SoSFPlane *field);
    SbPlane		fieldValue (void) const;

    SbPlane		value (void) const;
    SbVec3f		direction (void) const;
    float		distance (void) const;

	Ig3DAxisRotationControl*	axisRotationControl  (void) const;
    Ig3DFloatControl*			distanceControl  (void) const;

    QString		getSettingString (void) const;
    void		setSettingString (QString &setting);

public slots:
    virtual void	setValue (const SbPlane &value);
    virtual void	setDirection (const SbVec3f &value);
    virtual void	setDistance (float value);
    virtual void	emitSettingsChanged (void);

    virtual void	show (void);
    virtual void	hide (void);

protected:
    virtual void	applyValue (void);
    virtual void	updateValue (void);

private:
	Ig3DAxisRotationControl*	m_rotation;
    IgPlaneDirection*			m_direction;
    Ig3DFloatControl*			m_distance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DPLANE_CONTROL_H
