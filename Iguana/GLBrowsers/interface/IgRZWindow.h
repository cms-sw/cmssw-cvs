#ifndef IGUANA_GL_BROWSERS_IG_RZ_WINDOW_H
# define IGUANA_GL_BROWSERS_IG_RZ_WINDOW_H

# include "Iguana/GLBrowsers/interface/Ig3DBaseWindow.h"

class IgPage;
class IgRZViewPropertiesCategory;
class Ig3DLightsCategory;
class IgQtObjectMenuMessage;

class IgRZWindow : public Ig3DBaseWindow
{
  Q_OBJECT
public:
  IgRZWindow (IgPage *page);
  ~IgRZWindow (void);

  virtual void		initCategories (void);
  virtual void 		initMenu (void);
  virtual void 		focusIn (void);
  virtual void 		focusOut (void);

private slots:

  void			showLights (void);
  void	 		showRZViewProperties (void);    

private:
  void			repMenu (IgQtObjectMenuMessage message);

  IgRZViewPropertiesCategory *m_rzViewPropertiesCategory;
  Ig3DLightsCategory	 *m_lightsCategory;
};

#endif // IGUANA_GL_BROWSERS_IG_RZ_WINDOW_H
