#ifndef IGUANA_GL_BROWSERS_IG_SCENE_MANAGER_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_SCENE_MANAGER_CATEGORY_H

# include "Iguana/GLBrowsers/interface/IgCommonViewCategory.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <Inventor/SbColor.h>

class IgState;
class Ig3DBaseBrowser;
class QFrame;
class QCheckBox;

class IGUANA_GL_BROWSERS_API IgSceneManagerCategory
    : public IgCommonViewCategory,
      public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgSceneManagerCategory);    
    Q_OBJECT
public:
    IgSceneManagerCategory (IgState *state);
    ~IgSceneManagerCategory ();

    virtual void		browserChanged 	(Ig3DBaseBrowser *browser);    
  
private slots:
    virtual void		setBackgroundColor	(void);
    virtual void		setBackgroundColor	(const SbColor& color);
    virtual void		setRGBMode		(bool option);
    virtual void		setRealTimeUpdate	(bool option);
    
private:
    void			addSceneManagerTab 	(void);
    
    QFrame			*m_background;    
    QCheckBox			*m_rgbMode;
    QCheckBox			*m_realTimeUpdate;
};

#endif // IGUANA_GL_BROWSERS_IG_SCENE_MANAGER_CATEGORY_H
