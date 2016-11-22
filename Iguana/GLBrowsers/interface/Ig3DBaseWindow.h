#ifndef IGUANA_GL_BROWSERS_IG_3D_BASE_WINDOW_H
# define IGUANA_GL_BROWSERS_IG_3D_BASE_WINDOW_H

#define QT_NO_DEBUG_OUTPUT

# include "Iguana/Studio/interface/IgView.h"
# include "Iguana/Framework/interface/IgBrowser.h"
# include <QObject>
# include <QWidget>

class IgState;
class IgSite;
class IgPage;
class Ig3DBaseBrowser;
class Ig3DBaseModel;
class IgCommonViewPropertiesCategory;
class IgControlCentre;
class QAction;
class QToolBar;

class Ig3DBaseWindow : public QWidget,
		       public IgView,
		       public IgBrowser
{
  Q_OBJECT
public:
  Ig3DBaseWindow (IgPage *page);
  ~Ig3DBaseWindow (void);

  virtual void 		initCategories (void);
  virtual void 		initMenu (void);
  virtual void		initToolBar (void);    
  virtual void		watchBrowser (Ig3DBaseBrowser *browser);
  virtual void		browse (IgRepresentable *object);
  virtual void		show (void);
  virtual void 		hide (void);
  virtual void 		focusIn (void);
  virtual void 		focusOut (void);
  virtual IgState*	state (void);
  IgControlCentre	*centre (void);
    	
  Ig3DBaseBrowser*	browser (void);
  Ig3DBaseModel*	model (void);
  QAction		*actionInvertCamera;
  QAction 		*actionYX;
  QAction 		*actionXZ;
  QAction 		*actionYZ;
  QAction		*actionShowAxis;
  QAction 		*actionCameraToggle;
  QToolBar		*toolBar;

private slots:
  void			showViewProperties (void);
  void 			toolbarVisibilityChanged(bool visibility);
  void			pick (void);
  void			view (void);
  void			print (const QString &text);
  void 			changeWindowState (Qt::WindowStates, Qt::WindowStates);
  
private:
  IgState		*m_state;
  IgSite		*m_site;
  Ig3DBaseModel		*m_model;
  Ig3DBaseBrowser	*m_browser;
  IgControlCentre	*m_centre;
  IgCommonViewPropertiesCategory 	*m_viewPropertiesCategory;
};

#endif // IGUANA_GL_BROWSERS_IG_3D_BASE_WINDOW_H
