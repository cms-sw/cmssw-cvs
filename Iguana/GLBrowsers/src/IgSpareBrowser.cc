#include "Iguana/GLBrowsers/interface/IgSpareBrowser.h"
#include "Iguana/GLModels/interface/Ig3DBaseRep.h"
#include "Iguana/GLModels/interface/IgSpareModel.h"
#include "Iguana/Inventor/interface/IgXYGrid.h"
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/SbLinear.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <iostream>
#include <QEvent>
#include <QMouseEvent>
#include <QWidget>
#include <cmath>

const char *
IgSpareBrowser::catalogLabel (void)
{ return "Spare"; }

IgSpareBrowser::IgSpareBrowser (IgState *state,
			      IgSite *site,
			      IgSpareModel *model)
    : Ig3DBaseBrowser (state,
		       site,
		       model ? model : new IgSpareModel (state)),
      m_grid (false)
{
    init ();

    SoNode *node = model->attachPoint ()->findMagic (
                         Ig3DBaseModel::encode ("Default Grid Group"));
    if (node && dynamic_cast<SoGroup*>(node)->getNumChildren ())
        m_grid = true;
}

void
IgSpareBrowser::init (void)
{
    SoQtExaminerViewer::setCameraType (SoOrthographicCamera::getClassTypeId ());
    SoOrthographicCamera * const camera = dynamic_cast <SoOrthographicCamera *> (this->getCamera ());
    if (! camera) return; // probably a scene-less viewer

    if (SoQtViewer::isHeadlight ()) SoQtViewer::setHeadlight (false);

//     camera->position = SbVec3f (0, 0, 1) * camera->position.getValue ().length ();
//     camera->orientation = SbRotation::identity ();

    camera->position = SbVec3f (-2.3816285e-07, -5.9540713e-07, 14.8);
    camera->orientation = SbRotation (SbVec3f (0, 1, 0),  0);
    camera->aspectRatio = 1;
    camera->focalDistance = 14.815891;
    camera->height = 12.273887;
    camera->nearDistance = 0.1;
    camera->farDistance = 32767;
   
    setEventCallback (eventCallback, this); 
    setBackgroundColor (SbColor (1.0, 1.0, 1.0));
}

SbBool
IgSpareBrowser::eventCallback (void *closure, QEvent *event)
{
    static bool firstEvent = true;    
    static float oldX = 0.;
    static float oldY = 0.;
    IgSpareBrowser *browser = static_cast<IgSpareBrowser *> (closure);
    if (! browser->isViewing ())
        return false;
    if (! browser->getParentWidget ()->hasMouse () && firstEvent)
        return false;
    if (! Ig3DBaseBrowser::eventCallback (closure, event))
    {      
        if (QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *> (event))
        {
	    if (mouseEvent->type () == QEvent::MouseMove
	        && mouseEvent->stateAfter () == Qt::NoButton)
	    {
	        if (! firstEvent)
	        {
	            firstEvent = true;
	            return true;
	        }
	    }
	    else if (mouseEvent->state () == Qt::LeftButton)
	    {
	        if (mouseEvent->stateAfter () == Qt::NoButton)
  	            firstEvent = true;
	        else if (mouseEvent->type () == QEvent::MouseMove)
	        {
	            if (firstEvent)
	            {
	                oldX = mouseEvent->x ();
	                oldY = mouseEvent->y ();
	                firstEvent = false;
	            }
	            else
	            {
	                float deltaX = mouseEvent->x () - oldX;
	                float deltaY = oldY - mouseEvent->y ();
	                SoCamera *camera = browser->getCamera ();
	                SbVec3f oldPosition = camera->position.getValue ();
	                camera->position = oldPosition - 
					   SbVec3f (deltaX/10, deltaY/10, 0);
	                oldX = mouseEvent->x ();
	                oldY = mouseEvent->y ();
	            }
	            return true;
	        }
	    }
	    return false;
        }
        return false;
    }
    return true;
}

QWidget *
IgSpareBrowser::buildLeftTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedWidth (0);
    return result;    
}

QWidget *
IgSpareBrowser::buildBottomTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedHeight (0);
    return result;    
}

QWidget *
IgSpareBrowser::buildRightTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedWidth (0);
    return result;    
}

void
IgSpareBrowser::buildDecoration (QWidget *parent)
{
    Ig3DBaseBrowser::buildDecoration (parent);    
    return;    
}

void
IgSpareBrowser::toggleCameraType (void)
{
    SoQtExaminerViewer::toggleCameraType ();
    Ig3DBaseBrowser::toggleCameraType ();    
}

void
IgSpareBrowser::invertCamera (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer

    camera->position = camera->position.getValue () * -1.0F;    
}

void
IgSpareBrowser::setGridVisibility (bool enable)
{
    IgSpareBrowser::drawGrid (enable);
}

void
IgSpareBrowser::drawGrid (bool enable)
{
    SoGroup *group = 0;
    SoNode *node = model ()->attachPoint ()->findMagic (
			Ig3DBaseModel::encode ("Default Grid Group"));
    if (!node)
    {
	group = new SoGroup;
 	group->setName (Ig3DBaseModel::encode ("Default Grid Group"));
	model ()->attachPoint ()->magic ()->insertChild (group, 0);
    }
    else
    {
	group = dynamic_cast<SoGroup *> (node);
    }

    if (! enable)
    {
	if (group->getNumChildren () > 0)
	    group->removeChild (0);
    }
    else if (group->getNumChildren () == 0)
    {
	SoNode *xyGrid;
	SoInput in;
	in.putBack (IV_GRID);
	SoDB::read (&in, xyGrid);

	xyGrid->setName (Ig3DBaseModel::encode ("XY Grid"));
	group->addChild (xyGrid);
    }
    m_grid = enable;
}

bool
IgSpareBrowser::isGridVisible (void)
{
    return m_grid;
}

void
IgSpareBrowser::viewPlaneX (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
    
    camera->position = SbVec3f (-1, 0, 0) * camera->position.getValue ().length ();
    camera->orientation = SbRotation (SbVec3f (0, 1, 0), -M_PI / 2.f);
}

void
IgSpareBrowser::viewPlaneY (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer

    SbVec3f norient = SbVec3f (0, -1, 0);
    camera->position = -norient * camera->position.getValue().length();
    camera->orientation = SbRotation (SbVec3f (0, 0, 1), -M_PI / 2.f) * 
                          SbRotation (SbVec3f (0, 0, -1), norient);
}

void
IgSpareBrowser::viewPlaneZ (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
  
    camera->position = SbVec3f (0, 0, 1) * camera->position.getValue ().length ();
    camera->orientation = SbRotation::identity ();
}
