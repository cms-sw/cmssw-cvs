#ifndef IGUANA_GL_BROWSERS_IG_3DUNIT_VEC_3F_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DUNIT_VEC_3F_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/Ig3DVec3fControl.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DUnitVec3fControl : public Ig3DVec3fControl
{
    Q_OBJECT
public:
    Ig3DUnitVec3fControl (IgControlCategory 	*pane,
    			  QString 		label,
			  Style			style = DirectionStyle);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    using Ig3DVec3fControl::setValue;
    using Ig3DVec3fControl::setValues;
    
public slots:
    virtual void	setValues (const float *values, unsigned int size);
    virtual void	setDirty (bool dirty);

private:
    void		normalize (float delta, float &v, float &a1,float &a2,
                                   float precision = 0.001);

    bool		m_internalUpdate;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DUNIT_VEC_3F_CONTROL_H
