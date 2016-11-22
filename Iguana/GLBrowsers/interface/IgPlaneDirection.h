#ifndef IGUANA_GL_BROWSERS_IG_PLANEDIRECTION_H
# define IGUANA_GL_BROWSERS_IG_PLANEDIRECTION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include <Inventor/SbLinear.h>
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgPlaneDirection : public QObject
{
    Q_OBJECT
public:	
	IgPlaneDirection ();
	IgPlaneDirection (const SbVec3f &vec);
	IgPlaneDirection (const float x, const float y, const float z);
	
	bool    isDirty ();
	void    setValue (float x, float y, float z);
	void    setValue (const SbVec3f &vec);
	SbVec3f value ();
	
signals:
	void dirty();

private:
	SbVec3f m_vector;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_PLANEDIRECTION_H
