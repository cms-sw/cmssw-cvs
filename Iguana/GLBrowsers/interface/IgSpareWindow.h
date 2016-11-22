#ifndef IGUANA_GL_BROWSERS_IG_SPARE_WINDOW_H
# define IGUANA_GL_BROWSERS_IG_SPARE_WINDOW_H

# include "Iguana/GLBrowsers/interface/Ig3DBaseWindow.h"

class IgPage;
class IgSpareViewPropertiesCategory;
class Ig3DLightsCategory;
class IgQtObjectMenuMessage;

/*!
 *
 * \class IgSpareWindow
 * \brief Implements a generic 3DBaseWindow modeled after IgRPhiWindow
 *
 *  This is intended to be used for specialist displays that need the utility
 *  of a IgRPhiWindow, but which would interfere with its normal use.
 *
 */

class IgSpareWindow : public Ig3DBaseWindow
{
  Q_OBJECT
public:
  IgSpareWindow (IgPage *page);
  ~IgSpareWindow (void);

  virtual void		initCategories (void);
  virtual void 		initMenu (void);
  virtual void 		initToolBar (void);    
  virtual void 		focusIn (void);
  virtual void 		focusOut (void);

private slots:

  void			showLights (void);
  void	 		showSpareViewProperties (void);    

private:
  void			repMenu (IgQtObjectMenuMessage message);

  IgSpareViewPropertiesCategory *m_spareViewPropertiesCategory;
  Ig3DLightsCategory	 	*m_lightsCategory;
};

#endif // IGUANA_GL_BROWSERS_IG_SPARE_WINDOW_H
