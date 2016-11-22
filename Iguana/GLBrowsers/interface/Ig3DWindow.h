#ifndef IGUANA_GL_BROWSERS_IG_3D_WINDOW_H
# define IGUANA_GL_BROWSERS_IG_3D_WINDOW_H

# include "Iguana/GLBrowsers/interface/Ig3DBaseWindow.h"

class IgPage;
class Ig3DSlicersCategory;
class Ig3DClipsCategory;
class Ig3DLightsCategory;
class IgSoMaterialCategory;
class Ig3DAnimsCategory;
class Ig3DViewpointsCategory;
class Ig3DNodeCategory;
class IgQtObjectMenuMessage;

class Ig3DWindow : public Ig3DBaseWindow
{
  Q_OBJECT
public:
    
  Ig3DWindow (IgPage *page);
  ~Ig3DWindow (void);
    
  virtual void		initCategories (void);
  virtual void		initMenu (void);
  virtual void		initToolBar (void);
  virtual void 		focusIn (void);
  virtual void 		focusOut (void);
    
public slots:
  void			printViews(void);

private slots:
    
  void			showClips (void);
  void			showSlicers (void);
  void			showLights (void);
  void			showAnims (void);
  void			showViewpoints (void);
  void			showNodeManip (void);
  void			repSlice (void);
  void			repClip  (void);

private:
  void			repMenu (IgQtObjectMenuMessage message);

  Ig3DSlicersCategory		*m_slicersCategory;
  Ig3DClipsCategory		*m_clipsCategory;
  Ig3DLightsCategory		*m_lightsCategory;
  IgSoMaterialCategory	*m_materialsCategory;
  Ig3DAnimsCategory		*m_animsCategory;
  Ig3DViewpointsCategory	*m_viewpointsCategory;
  Ig3DNodeCategory		*m_nodeCategory;
};

#endif // IGUANA_GL_BROWSERS_IG_3D_WINDOW_H
