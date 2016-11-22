#ifndef VIS_G4_TRANSIENTS_IG_SO_G4_TRAJECTORIES_H
# define VIS_G4_TRANSIENTS_IG_SO_G4_TRAJECTORIES_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Transients/interface/config.h"
# include <Inventor/nodes/SoShape.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoSFEnum.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A specialised OpenInventor node that renders Geant4 trajectories
    directly from the current trajectory container.  A proof of
    concept implementation not suitable for long-term usage.  */
class VIS_G4_TRANSIENTS_API VisSoG4Trajectories : public SoShape
{
    SO_NODE_HEADER (VisSoG4Trajectories);
public:
    enum Flush
    {
	NONE,
	STEP,
	TRAJECTORY,
	ALL
    };

    SoSFBool	on;
    SoSFEnum	flush;

    VisSoG4Trajectories (void);
    // implicit copy constructor
    // implicit assignment operator

    static void		initClass (void);

    void		touch (void);

protected:
    virtual ~VisSoG4Trajectories (void);

    virtual void	computeBBox(SoAction *, SbBox3f &box, SbVec3f &center);
    virtual void	GLRender (SoGLRenderAction *renderAction);
    virtual void	generatePrimitives (SoAction *action);
    //virtual void	rayPick (SoRayPickAction *);
    //virtual void	callback (SoCallbackAction *);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_TRANSIENTS_IG_SO_G4_TRAJECTORIES_H
