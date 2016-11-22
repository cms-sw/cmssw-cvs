#ifndef IGUANA_GL_BROWSERS_IG_3DAXISROTATION_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DAXISROTATION_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Iguana/GLBrowsers/interface/IgControlBase.h"
# include "Iguana/GLBrowsers/interface/Ig3DPlaneControl.h"
# include "Iguana/GLBrowsers/interface/IgQtAxisAlignment.h"
# include "Iguana/Studio/interface/IgSoQtThumbWheel.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
class Ig3DPlaneControl;
class SbVec3f;
//class IgSoQtThumbWheel;

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
static const int X_Axis = 0;
static const int Y_Axis = 1;
static const int Z_Axis = 2;

//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DAxisRotationControl : public IgControlBase
{
    Q_OBJECT
public:	
    Ig3DAxisRotationControl (IgControlCategory *pane, QString label, Ig3DPlaneControl* planeControl);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

	IgSoQtThumbWheel* xWidget () const;
	IgSoQtThumbWheel* yWidget () const;
	IgSoQtThumbWheel* zWidget () const;

protected:
    virtual void	applyValue (void);
    virtual void	updateValue (void);

private slots:
	void 		setXAxisDirty (float change);
	void 		setYAxisDirty (float change);
	void 		setZAxisDirty (float change);
	void		alignToAxis ();

private:
	void 		makeWheelControl (IgControlCategory* pane, QWidget* parent, int axis, bool horizontal);
	SbVec3f		rotateAboutAxis (float x, float y, float z, float angle);
	
	Ig3DPlaneControl*	m_planeControl;
	float				m_oldAngle[3];
	IgQtAxisAlignment*	m_alignment;
	IgSoQtThumbWheel*	m_thumbWheels[3];
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DAXISROTATION_CONTROL_H
