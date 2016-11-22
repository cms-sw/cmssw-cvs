#ifndef IGUANA_GL_BROWSERS_IG_3D_CLIPS_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_3D_CLIPS_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/GLBrowsers/interface/Ig3DObjectCategory.h"
# include "Iguana/Inventor/interface/IgSoClipPlane.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <qstring.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DBaseBrowser;
class Ig3DBaseRep;
class SbVec3f;
class SoGroup;
class SoSwitch;
class Ig3DPlaneControl;
class Ig3DBoolControl;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DClipsCategory
    : public Ig3DObjectCategory<IgSoClipPlane>,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (Ig3DClipsCategory);    
public:
    Ig3DClipsCategory (IgState *state,
		       Ig3DBaseBrowser *source);
    // implicit destructor

    bool		makeOne 	(Ig3DBaseRep *rep);
    void		reconstruct 	(void);
    void		reconstruct 	(SoSwitch *from);
    void		browserChanged  (Ig3DBaseBrowser *browser);
    
    virtual void	attach (void);
    virtual void	detach (void);
    virtual void	update (void);
    virtual void	apply (void);
    virtual void	registerBrowser (IgState *state,
                                         Ig3DBaseBrowser *browser);    

protected:
    virtual void	createNewItem (void);
    virtual void	deleteItem (void);
    virtual void	renameItem (const QString &newName);

private slots:
    virtual bool	makeOne 	(void);
    virtual void 	deleteOne 	(void);
    
private:
    void		reconstruct 	(SoGroup *from);
    void		buildDefaults 	(void);
    SoSwitch *		repClips	(SoGroup	*magic,
					 bool		make,
					 SoGroup	**objects,
					 SoGroup	**active);
    SoGroup *		repActiveClips	(Ig3DBaseRep *rep, 	bool make);
    SoGroup *		repAllClips	(Ig3DBaseRep *rep, 	bool make);
    IgSoClipPlane *	makeClip	(QString name, 
                                	 bool on,
					 SbVec3f center);
    Ig3DBaseBrowser	*m_source;
    
    Ig3DPlaneControl	*m_plane;
    Ig3DBoolControl	*m_on;
    Ig3DBoolControl	*m_manip;

    static const char	*s_group;
    static const char	*s_all;
    static const char	*s_active;

    // undefined semantics
    Ig3DClipsCategory (const Ig3DClipsCategory &);
    Ig3DClipsCategory &operator= (const Ig3DClipsCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3D_CLIPS_CATEGORY_H
