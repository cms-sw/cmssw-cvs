#ifndef IGUANA_GL_BROWSERS_IG_3DCAMERA_UTILS_H
# define IGUANA_GL_BROWSERS_IG_3DCAMERA_UTILS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoSFRotation;
class SoSFVec3f;
class SoSFFloat;
class SbVec3f;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DCameraUtils
{
public:
    Ig3DCameraUtils (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    void	setCamera (SoSFRotation *orientation,
		    	   SoSFVec3f *position,
			   SoSFFloat *focalDistance);

    SbVec3f	getFocalPoint (void) const;
    void	setFocalPoint (const SbVec3f &focalPoint);

private:
    SoSFRotation*m_orientation;
    SoSFVec3f	*m_position;
    SoSFFloat	*m_focalDistance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DCAMERA_UTILS_H
