#ifndef VIS_FRAMEWORK_BASE_VIS_EVENT_MENU_H
# define VIS_FRAMEWORK_BASE_VIS_EVENT_MENU_H

# include "Iguana/Framework/interface/IgExtension.h"
# include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h" // This one has to be here due to conflicts of sigc signals 
// with qobject signals
# include "VisFramework/VisFrameworkBase/interface/VisAutoStart.h"
# include "VisFramework/VisFrameworkBase/interface/VisAutoPrint.h"
# include "VisFramework/VisFrameworkBase/interface/VisEventTimer.h"
# include "VisFramework/VisFrameworkBase/interface/VisPrintTimer.h"
# include <QObject>

class QAction;
class QMenu;
class QToolButton;
class QTimer;
class VisProcessEventDialog;

/** Menu actions for the current event.  */
class VisEventMenu : public QObject, public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisEventMenu);
  Q_OBJECT
public:

  VisEventMenu (IgState *state);
  ~VisEventMenu (void);
    
public slots:
  void		nextEvent (void);
  void		previousEvent (void);
  void		autoEvents (void);
  void		processOneEvent (void);
  void		processEvent (unsigned int runNum, unsigned int eventNum);
  void		processEventDialog (void);
  void		rewind (void);
  void		skipEventDialog (void);
  void 		l1TriggerDialog (void);
  void		skipEvent (long num);
  void		autoPrint (void);
  void		print (void);

private:
  void		finalizeTimer (void);
  virtual void 	autoChanged (void);
  virtual void	delayChanged (void);

  IgState	*m_state;
  VisProcessEventDialog *m_eventSelector;
  QMenu 	*m_menuEvent;
  QAction 	*m_actionAuto;
  QAction 	*m_actionPrint;
  QAction 	*m_actionGotoEvent;
  QAction	*m_actionPrevious;
  QAction	*m_actionRewind;
  QTimer	*m_timer;
  QTimer	*m_printTimer;
  bool		 m_auto;
  bool		 m_print;
  VisAutoPrint	 m_autoPrint;    
  VisAutoStart	 m_autoStart;
  VisEventTimer	 m_delay;
  VisPrintTimer	 m_printDelay;
  bool		 m_done;
};

#endif // VIS_FRAMEWORK_BASE_VIS_EVENT_MENU_H
