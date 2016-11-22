#ifndef IGUANA_GL_BROWSERS_IG_LEGO_WINDOW_H
# define IGUANA_GL_BROWSERS_IG_LEGO_WINDOW_H

# include "Iguana/GLBrowsers/interface/Ig3DBaseWindow.h"

class IgPage;
class IgLegoViewPropertiesCategory;
class Ig3DLightsCategory;
class IgQtObjectMenuMessage;

class IgLegoWindow : public Ig3DBaseWindow
{
  Q_OBJECT
public:
  IgLegoWindow (IgPage *page);
  ~IgLegoWindow (void);

  virtual void		initCategories (void);
  virtual void 		initMenu (void);
  virtual void 		initToolBar (void);    
  virtual void 		focusIn (void);
  virtual void 		focusOut (void);

private slots:

  void			showLights (void);
  void	 		showLegoViewProperties (void);    

private:
  void			repMenu (IgQtObjectMenuMessage message);

  IgLegoViewPropertiesCategory *m_legoViewPropertiesCategory;
  Ig3DLightsCategory	 *m_lightsCategory;
};

#endif // IGUANA_GL_BROWSERS_IG_LEGO_WINDOW_H
