#ifndef IGUANA_GL_BROWSERS_IG_2D_BROWSER_H
# define IGUANA_GL_BROWSERS_IG_2D_BROWSER_H

# include "Iguana/GLBrowsers/interface/Ig3DBaseBrowser.h"

class Ig2DModel;
class QEvent;

class Ig2DBrowser : public Ig3DBaseBrowser
{
  Q_OBJECT

public:
  Ig2DBrowser (IgState *state, IgSite *site, Ig2DModel *model = 0);

  static const char *		catalogLabel (void);

public slots:
  virtual void 		viewPlaneX (void);
  virtual void 		viewPlaneY (void);
  virtual void 		viewPlaneZ (void);

protected:
  static SbBool 		eventCallback (void *closure, QEvent *event);
    
  virtual void buildDecoration (QWidget *parent);    
  virtual QWidget *buildLeftTrim (QWidget *parent);
  virtual QWidget *buildBottomTrim (QWidget *parent);
  virtual QWidget *buildRightTrim (QWidget *parent);
};

#endif // IGUANA_GL_BROWSERS_IG_2D_BROWSER_H
