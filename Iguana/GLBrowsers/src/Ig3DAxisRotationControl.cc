#include "Iguana/GLBrowsers/interface/IgControlCategory.h"
#include "Iguana/GLBrowsers/interface/Ig3DAxisRotationControl.h"
#include "Iguana/GLBrowsers/interface/IgQtRangeControlCommon.h"

/** Create a new axis rotation controller with three thumbwheels.  */
Ig3DAxisRotationControl::Ig3DAxisRotationControl (IgControlCategory *pane,
					  QString label, Ig3DPlaneControl* planeControl)
    : IgControlBase (pane), m_planeControl(planeControl)
{
	assert (pane);
	assert (pane->bodyArea ());
	assert (pane->bodyLayout ());

	std::vector<QString> labels;
	QString tmpLabels ("X:Y:Z");
	
	for(unsigned int i = 0; i < 3; i++)
	{
		labels.push_back (IgQtRangeControlCommon::getSettingItem (tmpLabels));
	}

	QWidget     *area = pane->bodyArea ();
	
	// Add label to the grid
	QWidget     *vlab = makeIndentedLabel (label, area);
	addGridWidget (vlab, gridRows (), 0);
	pane->pushIndent ();

	// create three thumbWheel controls for rotating around the axes
	for (int axis = X_Axis; axis <= Z_Axis; axis++)
	{
		makeWheelControl(pane, vlab, axis, true);// und so weiter...
	}
	
	// Add alignment button group if requested
	m_alignment = new IgQtAxisAlignment (area);
	addGridWidget (makeSpacedBox (area, m_alignment->widget ()), gridRows (), 1);
	
	connect (m_alignment, SIGNAL (dirty ()), this, SLOT (alignToAxis ()));
	
	pane->popIndent ();
}

void
Ig3DAxisRotationControl::setXAxisDirty (float angle)
{
	// calculate the difference between the old and the new angle
	// and remember the new angle for the next call
	float change = angle - m_oldAngle[0];
	m_oldAngle[0] = angle;
	
	// get the plane's normal direction and rotate it by 'change' 
	// degrees (radians) around the X axis. Finally set the plane's 
	// new calculated direction.
	SbVec3f old_dir (m_planeControl->direction ());
	SbVec3f new_dir = rotateAboutAxis (old_dir[1], old_dir[2], old_dir[0], change);
	m_planeControl->setDirection (SbVec3f(new_dir[2], new_dir[0], new_dir[1]));
}

void
Ig3DAxisRotationControl::setYAxisDirty (float angle)
{
	float change = angle - m_oldAngle[1];
	m_oldAngle[1] = angle;
	
	SbVec3f old_dir (m_planeControl->direction ());
	SbVec3f new_dir = rotateAboutAxis (old_dir[2], old_dir[0], old_dir[1], change);
	m_planeControl->setDirection (SbVec3f(new_dir[1], new_dir[2], new_dir[0]));
}

void
Ig3DAxisRotationControl::setZAxisDirty (float angle)
{
	float change = angle - m_oldAngle[2];
	m_oldAngle[2] = angle;
	
	SbVec3f old_dir (m_planeControl->direction ());
	SbVec3f new_dir = rotateAboutAxis (old_dir[0], old_dir[1], old_dir[2], change);
	m_planeControl->setDirection (new_dir);
}

void
Ig3DAxisRotationControl::alignToAxis ()
{
	// gets the alignment vector on a signal event and sets the cutting plane 
	// accordingly. After that, the alignment buttons have to be cleaned again
	// to be able to use them again.
	m_planeControl->setDirection (m_alignment->value ().getValue ());
	m_alignment->setClean();
}

void
Ig3DAxisRotationControl::makeWheelControl (IgControlCategory* pane, QWidget* parent, int axis, bool horizontal)
{
	// get index of the last row
	int row = gridRows ();
	
	QWidget *area = pane->bodyArea ();
	
	// determine the orientation based on the passed boolean
	Qt::Orientation orientation;
	orientation = (horizontal ? Qt::Horizontal : Qt::Vertical);
		
	// create the thumbwheel and its label
	QString label;
	switch (axis)
    {
    	case X_Axis:
			label = "X Axis";
			break;
		case Y_Axis:
			label = "Y Axis";
			break;
		default:
			label = "Z Axis";
			break;
    }
	m_thumbWheels[axis] = new IgSoQtThumbWheel (orientation, parent, label);
		
	// create a label inside an area
	QWidget* vlab = makeIndentedLabel (label, area);
	
	// add label and thumbwheel control to the gridded widget
	addGridWidget (vlab, row, 0);
	addGridWidget (makeSpacedBox (area, m_thumbWheels[axis]), row, 1);
	
	// connect wheels	
	switch (axis)
    {
    	case X_Axis:
			connect (m_thumbWheels[axis], SIGNAL( wheelMoved (float) ), this, SLOT( setXAxisDirty (float) ));
			break;
		case Y_Axis:
			connect (m_thumbWheels[axis], SIGNAL( wheelMoved (float) ), this, SLOT( setYAxisDirty (float) ));
			break;
		default:
			connect (m_thumbWheels[axis], SIGNAL( wheelMoved (float) ), this, SLOT( setZAxisDirty (float) ));
			break;
	}
}

IgSoQtThumbWheel*
Ig3DAxisRotationControl::xWidget () const
{
	return m_thumbWheels[X_Axis];
}

IgSoQtThumbWheel* 
Ig3DAxisRotationControl::yWidget () const
{
	return m_thumbWheels[Y_Axis];
}

IgSoQtThumbWheel* 
Ig3DAxisRotationControl::zWidget () const
{
	return m_thumbWheels[Z_Axis];
}

/** Rotates a 3D vector given by its components and the angle. 
	To rotate about the x-Axis pass the vector components as follows: 
	rotateAboutAxis(y,z,x,angle)
	About y-Axis: rotateAboutAxis(z,x,y,angle)
	About z-Axis: rotateAboutAxis(x,y,z,angle)*/
SbVec3f
Ig3DAxisRotationControl::rotateAboutAxis (float x, float y, float z, float angle)
{
	float new_x = x*cos(angle) - y*sin(angle);
	float new_y = x*sin(angle) + y*cos(angle);
	return SbVec3f(new_x, new_y, z);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DAxisRotationControl::updateValue (void)
{
    // We don't have a field for this control, so this method does
	// nothing.
}

void
Ig3DAxisRotationControl::applyValue (void)
{    
    // We don't have a field for this control, so this method does
	// nothing.
}
