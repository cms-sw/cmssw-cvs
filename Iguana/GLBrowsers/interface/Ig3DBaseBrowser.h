#ifndef IGUANA_GL_BROWSERS_IG3D_BROWSER_H
# define IGUANA_GL_BROWSERS_IG3D_BROWSER_H

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/Framework/interface/IgBrowser.h"
# include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
# include <QObject>
# include <Qt>
# include <QEvent>
# include <string>

class IgState;
class IgSite;
class IgSelectionMessage;
class IgQtObjectMenuMessage;

class Ig3DViewpoints;

class Ig3DBaseModel;
class Ig3DBaseModelEvent;
class Ig3DBaseRep;

class SoPickedPoint;
class SoPath;
class SoSelection;
class SoGroup;
class SoFieldSensor;

class QString;
class QEvent;
class QMdiSubWindow;

class IGUANA_GL_BROWSERS_API Ig3DBaseBrowser : public QObject,
					       public SoQtExaminerViewer,
					       public IgBrowser
{
  Q_OBJECT

public:


  static const int	MENU_3D_OPS		= 100;
  static const int	MENU_SEEKTO		= 101;
  static const int	MENU_SLICE		= 102;
  static const int	MENU_CLIP		= 103;


  Ig3DBaseBrowser (IgState *state, IgSite *site, Ig3DBaseModel *model);
  virtual ~Ig3DBaseBrowser (void);

  void setAutoClipping (SbBool enable);

  virtual void                 browse (IgRepresentable *object);
  virtual void                 select (Ig3DBaseRep *rep);

  virtual Ig3DBaseRep *        getCurrentPick (void) const;
  virtual Ig3DBaseRep *        getSelection (void) const;
  virtual const SoPickedPoint* getCurrentPickPoint (void) const;

  virtual Ig3DBaseModel *      model (void) const;
  virtual IgState *            state (void) const;

  virtual void   printBitmap (QString file, float ppi,
			      float dpi, QString format);
  virtual void   printVector (QString file, QString format, int level);

  virtual void   focusIn (void);
  virtual void   focusOut (void);

  Ig3DBaseRep*   menuRep (void);
  void           repMenu (IgQtObjectMenuMessage message);
  int            getGL2PSOptions (void);
  bool           isGridVisible (void);
  bool           isWhatsThisPicking (void);
    
  static bool    saveNode (SoNode *node, const QString& title,
			   QWidget *parent = 0, const char* file = 0);
  static bool    writeNode (SoNode *node, const QString& file, bool binary,
			    QWidget *parent = 0);
  static SoNode* openNode (const QString& nodeName, const QString& title,
			   QWidget *parent = 0, const char* file = 0);
  static SoNode* findGroup (SoNode *node, const char* name);

public slots:
  void     	zoomIn (void);
  void         	zoomOut (void);
  void         	zoom (const float diffvalue);
  void         	resetToHomePosition (void);
  void         	saveHomePosition (void);
  void         	viewAll (void);
  void         	seek (void);    
  void         	setFeedbackVisibility (bool enable);
  void 	 	setGridVisibility (bool enable);
  void         	view (void);
  void         	pick (void);
  void         	setWhatsThisPicking (bool enable = true);
  void         	setGL2PSOptions (int options);
  void         	changeWindowState (Qt::WindowStates oldState, Qt::WindowStates newState);
    
  virtual void 	autoPrint (void);
  virtual void 	autoPrint (const std::string text);
  virtual void 	viewPlaneX (void);
  virtual void 	viewPlaneY (void);
  virtual void 	viewPlaneZ (void);
  virtual void 	toggleCameraType (void);
  virtual void 	invertCamera (void);    
    
protected:
    
  virtual void     initWidget (IgSite *site);        
  virtual void     selectMessage (IgSelectionMessage message);
  virtual void     modelChanged (Ig3DBaseModelEvent event);
  virtual void     setCurrentPick (Ig3DBaseRep *rep);
  virtual void     setCurrentPickPoint (const SoPickedPoint *pick);
  virtual QWidget* buildLeftTrim (QWidget *parent);
  virtual QWidget* buildBottomTrim (QWidget *parent);
  virtual QWidget* buildRightTrim (QWidget *parent);

private slots:
  void save (void);
  void print (void);
  void repSeekTo (void);
  void leftWheelPressed (void);
  void leftWheelChanged (float by);
  void leftWheelReleased (void);

signals:
  void cameraToggled (void);
  void windowStateChanged (Qt::WindowStates oldState, Qt::WindowStates newState);
    
protected:
  QWidget* 	  initialise (IgState *state, IgSite *site);
  static SbBool   eventCallback (void *closure, QEvent *event);

private:
  static void    onStartPick (void *cb, SoQtViewer *viewer);
  static void    onFinishPick (void *cb, SoQtViewer *viewer);
  static SoPath* onPick (void *cb, const SoPickedPoint *pick);
  static void    onSelect (void *cb, SoPath *selection);
  static void    onDeselect (void *cb, SoPath *selection);
    
  //FIXME: remove these Callback method once the SoQt fixes the continuous
  //rendering problem
  static void    farDistanceSensorCB (void *me, SoSensor *sensor);
  static void    nearDistanceSensorCB (void *me, SoSensor *sensor);

  virtual void   drawGrid (const bool enable);

  IgState             *m_state;
  IgSite              *m_site;
  Ig3DBaseModel       *m_model;
  Ig3DBaseRep         *m_selection;
  Ig3DBaseRep         *m_menuRep;
  const SoPickedPoint *m_pick;
  bool                 m_first_time;
  bool                 m_grid;
  int                  m_gl2psOptions;
  Ig3DBaseRep         *m_currentPick;
  bool                 m_whatsThisPicking;
  bool                 m_oldView;
  bool                 m_oldSeek;
  int                  m_gl2psFBBufferSize;
  QMdiSubWindow       *m_mdiWindow;
  
  //FIXME: remove these two sensors once the SoQt fixes the continuous
  //rendering problem
  SoFieldSensor       *m_farDistanceSensor;
  SoFieldSensor       *m_nearDistanceSensor;
    
  // undefined semantics
  Ig3DBaseBrowser (const Ig3DBaseBrowser &);
  Ig3DBaseBrowser &operator= (const Ig3DBaseBrowser &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG3D_BROWSER_H
