//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Transients/interface/VisSoG4Trajectories.h"
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLLazyElement.h>
//#include <Inventor/actions/SoRayPickAction.h>
//#include <Inventor/actions/SoCallbackAction.h>

#include <G4RunManager.hh>
#include <G4Trajectory.hh>

#ifdef WIN32
# include <windows.h>
#endif
#ifdef __APPLE__
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>                                                                        
#endif

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)
#define DRAW_TRAJ_POINT(traj,i)					\
   do {								\
       G4TrajectoryPoint *tp = (G4TrajectoryPoint*) ((traj)->GetPoint (i));\
       G4ThreeVector     pos = tp->GetPosition ();		\
       glVertex3f (pos.x () / m, pos.y () / m, pos.z () / m);	\
   } while (0)

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_NODE_SOURCE (VisSoG4Trajectories);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
VisSoG4Trajectories::initClass (void)
{
    SO_NODE_INIT_CLASS (VisSoG4Trajectories, SoShape, "Shape");
}

VisSoG4Trajectories::VisSoG4Trajectories (void)
{
    SO_NODE_CONSTRUCTOR (VisSoG4Trajectories);
    SO_NODE_ADD_FIELD (on, (FALSE));
    SO_NODE_ADD_FIELD (flush, (NONE));

    SO_NODE_DEFINE_ENUM_VALUE (Flush, NONE);
    SO_NODE_DEFINE_ENUM_VALUE (Flush, STEP);
    SO_NODE_DEFINE_ENUM_VALUE (Flush, TRAJECTORY);
    SO_NODE_DEFINE_ENUM_VALUE (Flush, ALL);

    SO_NODE_SET_SF_ENUM_TYPE (flush,Flush);
}

VisSoG4Trajectories::~VisSoG4Trajectories (void)
{}

void
VisSoG4Trajectories::computeBBox (SoAction *, SbBox3f &box, SbVec3f &center)
{
    if (G4RunManager *runManager = G4RunManager::GetRunManager ())
	if (const G4Event *event = runManager->GetCurrentEvent ())
	    if (G4TrajectoryContainer *trajectories
		= event->GetTrajectoryContainer ())
	    {
		for (int count = 0; count < trajectories->entries (); count++)
		{
		    G4Trajectory *t = static_cast<G4Trajectory *>
				      ((*trajectories) [count]);
		    for (int i = 0; i < t->GetPointEntries (); i++)
		    {
			G4ThreeVector pos =
			    ((G4TrajectoryPoint *) t->GetPoint (i))
			    ->GetPosition ();

			box.extendBy (SbVec3f (pos.x () / m,
					       pos.y () / m,
					       pos.z () / m));
		    }
		    center = box.getCenter ();
		}
	    }
}

void
VisSoG4Trajectories::touch (void)
{ on.touch(); }

void
VisSoG4Trajectories::GLRender (SoGLRenderAction *action)
{
    if (on.getValue () == FALSE || ! shouldGLRender (action))
	return;

    SoState *state = action->getState ();
    state->push ();
    SoLazyElement::setColorMaterial (state, true);
    SoGLLazyElement::sendNoMaterial (state);

    if (G4RunManager *runManager = G4RunManager::GetRunManager ())
	if (const G4Event *event = runManager->GetCurrentEvent ())
	    if (G4TrajectoryContainer *trajectories
		= event->GetTrajectoryContainer ())
	    {
		glDisable (GL_LIGHTING);
		glColor3f (1,0,0);

		Flush flushing = (Flush) flush.getValue();

		for (int count = 0; count < trajectories->entries (); count++)
		{
		    G4Trajectory *t = static_cast<G4Trajectory *>
				      ((*trajectories) [count]);

		    if (t->GetPointEntries () == 1)
		    {
			glBegin (GL_POINTS);
			DRAW_TRAJ_POINT (t, 0);
			glEnd ();
			glFlush ();
		    }
		    else if (flushing == STEP)
		    {
			for (int i = 0; i < t->GetPointEntries ()-1; i++)
			{
			    glBegin (GL_LINES);
			    DRAW_TRAJ_POINT (t, i);
			    DRAW_TRAJ_POINT (t, i+1);
			    glEnd ();
			    glFlush ();
			}
		    }
		    else
		    {
			glBegin (GL_LINES);
			for (int i = 0; i < t->GetPointEntries ()-1; i++)
			{
			    DRAW_TRAJ_POINT (t, i);
			    DRAW_TRAJ_POINT (t, i+1);
			}
			glEnd ();

			if (flushing == TRAJECTORY)
			    glFlush ();
		    }

		}

		if (flushing == ALL)
		    glFlush ();

		glEnable (GL_LIGHTING);
	    }

    ((SoGLLazyElement *) SoLazyElement::getInstance (state))
	->reset (state, SoLazyElement::DIFFUSE_MASK
		 | SoLazyElement::LIGHT_MODEL_MASK);
    state->pop ();
}

void
VisSoG4Trajectories::generatePrimitives (SoAction *)
{
    if (on.getValue () == FALSE)
	return;

    // FIXME
}
