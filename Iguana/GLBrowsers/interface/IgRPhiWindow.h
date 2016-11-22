#ifndef IGUANA_GL_BROWSERS_IG_RPHI_WINDOW_H
# define IGUANA_GL_BROWSERS_IG_RPHI_WINDOW_H

# include "Iguana/GLBrowsers/interface/Ig3DBaseWindow.h"

class IgPage;
class IgRPhiViewPropertiesCategory;
class Ig3DLightsCategory;
class IgQtObjectMenuMessage;

class IgRPhiWindow : public Ig3DBaseWindow
{
  Q_OBJECT
public:
  IgRPhiWindow (IgPage *page);
  ~IgRPhiWindow (void);

  virtual void		initCategories (void);
  virtual void 		initMenu (void);
  virtual void 		focusIn (void);
  virtual void 		focusOut (void);

private slots:

  void			showLights (void);
  void	 		showRPhiViewProperties (void);    

private:
  void			repMenu (IgQtObjectMenuMessage message);

  IgRPhiViewPropertiesCategory *m_rphiViewPropertiesCategory;
  Ig3DLightsCategory	 	*m_lightsCategory;
};

#endif // IGUANA_GL_BROWSERS_IG_RPHI_WINDOW_H
