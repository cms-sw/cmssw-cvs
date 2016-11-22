//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT
#include "VisFramework/VisFrameworkBase/interface/VisEventMenu.h"
#include "VisFramework/VisFrameworkBase/interface/VisExceptionService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "VisFramework/VisFrameworkBase/interface/VisL1FilterService.h"
#include "VisFramework/VisFrameworkBase/interface/VisL1TriggerFilterMenu.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventNavigationDialog.h"
#include "VisFramework/VisFrameworkBase/interface/VisProcessEventDialog.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/Provenance/interface/EventID.h"
#include "FWCore/Framework/interface/InputSource.h"
#include "FWCore/MessageLogger/interface/JobReport.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Iguana/Framework/interface/IgArgsElement.h"
#include "Iguana/Framework/interface/IgEnvsElement.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Studio/interface/IgDocument.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include "Iguana/Studio/interface/IgQtAppToolBarService.h"
#include "Iguana/Studio/interface/IgQtSite.h"
#include "Iguana/Studio/interface/IgQtSiteStack.h"
#include <QApplication>
#include <QMenu>
#include <QToolBar>
#include <q3buttongroup.h>
#include <q3filedialog.h>
#include <qicon.h>
#include <q3listview.h>
#include <qlineedit.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtimer.h>
#include <qtoolbutton.h>
#include <qvalidator.h>
#include <qworkspace.h>
#include <QPixmap>
#include <fstream>

IG_DEFINE_STATE_ELEMENT (VisEventMenu, "Services/Framework/GUI/EventMenu");

/** Initialise the visual controls for the event.  At the moment only
    registers a couple of menu items for walking the event collection.
    In future this should create a control panel for the event and
    event collection, the menu items and possibly tool bar buttons.  */
VisEventMenu::VisEventMenu (IgState *state)
    : m_state(state),
      m_eventSelector(new VisProcessEventDialog),
      m_timer(new QTimer (this)),
      m_printTimer (new QTimer (this)),
      m_auto (false),
      m_print (false),
      m_autoPrint (state, lat::CreateCallback (this, &VisEventMenu::autoChanged)),
      m_autoStart (state, lat::CreateCallback (this, &VisEventMenu::autoChanged)),
      m_delay (state, lat::CreateCallback (this, &VisEventMenu::delayChanged)),
      m_printDelay (state, lat::CreateCallback (this, &VisEventMenu::delayChanged)),
      m_done (true)
{
  ASSERT (m_state);
  m_state->put (s_key, this);

  m_eventSelector->setModal (true);
  QRegExp rx ("\\d+");
  m_eventSelector->runNumberText->setValidator (new QRegExpValidator (rx, this));
  m_eventSelector->eventNumberText->setValidator (new QRegExpValidator (rx, this));

    // FIXME: Reuse the IgObjectMenu stuff!
    ASSERT (IgQtAppMenuService::get (m_state));

    QMenuBar *menubar = IgQtAppMenuService::get (m_state)->menuBar ();    
    m_menuEvent = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuEvent"));
    if (! m_menuEvent)
    {
	m_menuEvent = new QMenu (menubar);
	m_menuEvent->setObjectName (QString::fromUtf8 ("menuEvent"));
	m_menuEvent->setTearOffEnabled (true);

	QMenu *menuView = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuView"));
	if (menuView != 0)
	    menubar->insertMenu (menuView->menuAction (), m_menuEvent);
	else
	    menubar->addMenu (m_menuEvent);
	m_menuEvent->setTitle (QApplication::translate ("IgMainWindow", "Event", 0, QApplication::UnicodeUTF8));
    }
    
    ASSERT (IgQtAppToolBarService::get (m_state));
    IgQtAppToolBarService *tb = IgQtAppToolBarService::get (m_state);
    QToolBar *toolBar = tb->toolBar (1, "Event Navigation");
    ASSERT (toolBar);

    QIcon iconPrevious;
    iconPrevious.addPixmap(QPixmap(QString::fromUtf8(":/icons/previous.xpm")), QIcon::Normal, QIcon::Off);
    QIcon iconNext;
    iconNext.addPixmap(QPixmap(QString::fromUtf8(":/icons/next.xpm")), QIcon::Normal, QIcon::Off);
    QIcon iconAuto;
    iconAuto.addPixmap(QPixmap(QString::fromUtf8(":/icons/play.xpm")), QIcon::Normal, QIcon::Off);
    iconAuto.addPixmap(QPixmap(QString::fromUtf8(":/icons/pause.xpm")), QIcon::Normal, QIcon::On);

    m_actionPrevious = new QAction (m_menuEvent);
    m_actionPrevious->setObjectName (QString::fromUtf8 ("actionPrevious"));
    m_actionPrevious->setEnabled (false);
    m_actionPrevious->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    m_actionPrevious->setIcon (iconPrevious);

    QAction *actionNext = new QAction (m_menuEvent);
    actionNext->setObjectName (QString::fromUtf8 ("actionNext"));
    actionNext->setEnabled (true);
    actionNext->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    actionNext->setIcon (iconNext);

    m_actionAuto = new QAction (m_menuEvent);
    m_actionAuto->setObjectName (QString::fromUtf8 ("actionAuto"));
    m_actionAuto->setEnabled (true);
    m_actionAuto->setCheckable (true);    
    m_actionAuto->setChecked (m_autoStart.value ());
    m_actionAuto->setIcon (iconAuto);

    m_actionGotoEvent = new QAction (m_menuEvent);
    m_actionGotoEvent->setObjectName (QString::fromUtf8 ("actionGotoEvent"));
    m_actionGotoEvent->setEnabled (false);
    m_actionGotoEvent->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));

    m_menuEvent->addAction (m_actionPrevious);
    m_menuEvent->addAction (actionNext);
    m_menuEvent->addAction (m_actionAuto);
    m_menuEvent->addAction (m_actionGotoEvent);
    m_menuEvent->addSeparator ();

    toolBar->addAction (m_actionPrevious);
    toolBar->addAction (actionNext);
    toolBar->addAction (m_actionAuto);

    toolBar->show ();

    QObject::connect (m_actionPrevious, SIGNAL(triggered()), this, SLOT(previousEvent()));
    QObject::connect (actionNext, SIGNAL(triggered()), this, SLOT(nextEvent()));
    QObject::connect (m_actionAuto, SIGNAL(triggered()), this, SLOT(autoEvents()));
    QObject::connect (m_actionGotoEvent, SIGNAL(triggered()), this, SLOT(processEventDialog()));

    m_actionPrevious->setText (QApplication::translate ("IgMainWindow", "&Previous Event", 0, QApplication::UnicodeUTF8));
    actionNext->setText (QApplication::translate ("IgMainWindow", "&Next Event", 0, QApplication::UnicodeUTF8));
    m_actionAuto->setText (QApplication::translate ("IgMainWindow", "Auto Events", 0, QApplication::UnicodeUTF8));
    m_actionGotoEvent->setText (QApplication::translate ("IgMainWindow", "&Goto Event...", 0, QApplication::UnicodeUTF8));


//     event->insertItem ("&Rewind",
// 		       this,
// 		       SLOT(rewind ()), Qt::CTRL+Qt::Key_R,
// 		       MENU_EVENT_REWIND, IgQtAppMenuService::index
// 		       (event, MENU_EVENT_REWIND));

//     event->insertItem ("&Skip...",
// 		       this,
// 		       SLOT(skipEventDialog ()), 0,
// 		       MENU_EVENT_SKIP, IgQtAppMenuService::index
// 		       (event, MENU_EVENT_SKIP));

//     event->insertItem ("&L1 Filter...",
// 		       this,
// 		       SLOT(l1TriggerDialog ()), 0,
// 		       MENU_EVENT_L1TRIGGER, IgQtAppMenuService::index
// 		       (event, MENU_EVENT_L1TRIGGER));

//     event->insertSeparator ();

//     event->insertItem ("Auto Print",
// 		       this,
// 		       SLOT(autoPrint ()), 0,
// 		       MENU_EVENT_AUTO_PRINT, IgQtAppMenuService::index
// 		       (event, MENU_EVENT_AUTO_PRINT));

//     menu->insertItem ("&Event", event, MENU_ID_EVENT,
// 		      IgQtAppMenuService::index (event, MENU_ID_EVENT));
				       
    // Disable the auto-events as it isn't implemented yet.  Note that
    // this object gets created in the event processing thread so we
    // more or less know that we have, or are about to have, an event.
    // So there is little point in disabling the menu item.  (FIXME:
    // Still need to disable it between events!)
    // 
    // FIXME: Re-initialization of the event processor sort of works:
    // we can use it for dynamic twigs. The ones which were loaded
    // as the data proxies still have problems: connection with the 
    // signal is lost somehow.
//     menu->setItemEnabled (MENU_INIT_APP, false);
//     menu->setItemEnabled (MENU_RE_INIT_APP, false);
//     menu->setItemEnabled (MENU_EVENT_NEXT, true);

    // FIXME: Previous event can be retrieved from the PoolSource
    // and yet to be tested with other sources.
    // For online we may want to write our own source to keep a few 
    // events for that perpose.
//     menu->setItemEnabled (MENU_EVENT_PREVIOUS, true);
//     menu->setItemEnabled (MENU_EVENT_AUTO, true);
//     menu->setItemChecked (MENU_EVENT_AUTO, m_autoStart.value ());
//     menu->setItemEnabled (MENU_EVENT_NUM, true);
//     menu->setItemEnabled (MENU_EVENT_REWIND, true);	
//     menu->setItemChecked (MENU_EVENT_AUTO_PRINT, m_autoPrint.value ());

    if (m_autoStart.value ())
    {
	m_auto = true;
	m_actionAuto->setChecked (m_auto);
	connect (m_timer, SIGNAL (timeout ()), SLOT (processOneEvent ()));
	m_timer->start (m_delay.value (), false); 
    }
    if (m_autoPrint.value ())
    {
	m_print = true;
	connect (m_printTimer, SIGNAL (timeout ()), SLOT (print ()));
	m_printTimer->start (m_printDelay.value (), false);
    }
}

void
VisEventMenu::autoChanged (void)
{}

void
VisEventMenu::delayChanged (void) 
{}

/** Destruct and deregister the state element.  */
VisEventMenu::~VisEventMenu (void)
{
    // FIXME: Clear the menu?  Or did Qt already take care of that?
    // What if we get deleted before Qt GUI dies?  Do our slots get
    // deregistered automatically?
    ASSERT (m_state);
    m_state->detach (s_key);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Handle the "Next Event" action.  Schedules an event-thread action
    for #VisQueueProcessor to return for advance to the next event --
    once all already-registered actions have executed.  */
void
VisEventMenu::nextEvent (void)
{
  IgArgsElement *args = IgArgsElement::get (m_state);

  try 
  {
    processOneEvent ();
  }
  catch (lat::Error& e)
  {
    QMessageBox::critical (0, "IGUANA", QString ("<p>Exception caught in %1:</p>"
						 "<p>%2</p>")
			   .arg (QString::fromStdString (args->argv () [0]))
			   .arg (QString::fromStdString (e.explainSelf ())));
  }
  catch (std::exception& e)
  {
    QMessageBox::critical (0, "IGUANA", QString ("<p>Standard library exception caught in %1:</p>"
						 "<p>%2</p>")
			   .arg (QString::fromStdString (args->argv () [0]))
			   .arg (QString::fromStdString (e.what ())));
  }
  catch (...)
  {
    QMessageBox::critical (0, "IGUANA", QString ("<p>Unknown exception caught in %1</p>")
			   .arg (QString::fromStdString (args->argv () [0])));
  }
  m_actionGotoEvent->setEnabled(true);
  m_actionPrevious->setEnabled(true);
}

void
VisEventMenu::previousEvent (void)
{
  IgArgsElement *args = IgArgsElement::get (m_state);

  try 
  {
    if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
    {
      (*procService)->skip (-2);
      (*procService)->run (1);
    }
  }
  catch (lat::Error& e)
  {
    QMessageBox::critical (0, "IGUANA", QString ("<p>Exception caught in %1:</p>"
						 "<p>%2</p>")
			   .arg (QString::fromStdString (args->argv () [0]))
			   .arg (QString::fromStdString (e.explainSelf ())));
  }
  catch (std::exception& e)
  {
    QMessageBox::critical (0, "IGUANA", QString ("<p>Standard library exception caught in %1:</p>"
						 "<p>%2</p>")
			   .arg (QString::fromStdString (args->argv () [0]))
			   .arg (QString::fromStdString (e.what ())));
  }
  catch (...)
  {
    QMessageBox::critical (0, "IGUANA", QString ("<p>Unknown exception caught in %1</p>")
			   .arg (QString::fromStdString (args->argv () [0])));
  }
}

/** Handle the "Auto Events" action. See old
    IgVis implementation on what this should do: tick/untick the menu
    item.  If ticked, schedule a QTimer to automatically do a next
    event every second or so.  If unticked, kill the timer.  */
void
VisEventMenu::autoEvents (void)
{
    // FIXME: remodel this without knowing about animators -- some
    //  central event trigger mechanism and a custom animator that
    //  knows about that?  don't want to have 3d browser know about
    //  events or us to know about 3d -- custom animator sounds best
    //  but need to teach 3d browser about extensions like that...
    //  on creation of this object should somehow register it, but
    //  again without dependence on inventor -- really need to sort
    //  out the requirements here!
    //
    //   -- flip the menu item tick on autoevents
    //   -- set autoevent handling based on the new value
    //       -- on:
    //           -- get current animator
    //           -- connect its "ready for new event" to "next event"
    //           -- connect its "axis reset" to "print by axis"
    //           -- activate the animator
    //       -- off:
    //           -- get the current animator
    //           -- turn it off
    //           -- disconnect the "ready for new event" and "axis reset"

    // FIXME: similar auto-print menu -- need somehow automatic actions
    //   for both: automatically print event once it has been received
    //   *and* all reps have updated themselves (so the scene is right!)
    //
    //   thusly, need a) notification scheme on events (and possibly other
    //   types of occurances?) that b) can be hooked up with automatic
    //   printer, animator and next-event-stepper -- all in synchronised
    //   fashion so that they execute in a sensible order!

    IgArgsElement *args = IgArgsElement::get (m_state);

    try 
    {
	m_auto ? m_auto = false : m_auto = true;
	m_actionAuto->setChecked (m_auto);

	std::string timeString;
     
	if (m_auto)
	{
	    connect (m_timer, SIGNAL (timeout ()), SLOT (processOneEvent ()));
	    m_timer->start (m_delay.value (), false); // set 5 seconds apart
 	}
	else
	{
	    m_timer->stop ();
	    m_timer->disconnect ();
	}
    }
    catch (lat::Error& e)
    {
	QMessageBox::critical (0, "IGUANA", QString ("<p>Exception caught in %1:</p>"
						     "<p>%2</p>")
			       .arg (QString::fromStdString (args->argv () [0]))
			       .arg (QString::fromStdString (e.explainSelf ())));
    }
    catch (std::exception& e)
    {
	QMessageBox::critical (0, "IGUANA", QString ("<p>Standard library exception caught in %1:</p>"
						     "<p>%2</p>")
			       .arg (QString::fromStdString (args->argv () [0]))
			       .arg (QString::fromStdString (e.what ())));
    }
    catch (...)
    {
	QMessageBox::critical (0, "IGUANA", QString ("<p>Unknown exception caught in %1</p>")
			       .arg (QString::fromStdString (args->argv () [0])));
    }
}

void
VisEventMenu::processOneEvent (void) 
{
    int rc = -1;
    
    if (m_done)
    {
	m_done = false;
	
	if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
	{
	    try 
	    {    
		(*procService)->run (1);
	    }
	    catch (cms::Exception& e) 
	    {
		finalizeTimer ();	
		std::string shortDesc ("CMSException");
		std::ostringstream longDesc;
		longDesc << "cms::Exception caught in "
			 << "iguana -s CMSSW"
			 << "\n"
			 << e.explainSelf();
		rc = 8001;
		procService->jobReport ()->reportError (shortDesc, longDesc.str (), rc);
		edm::LogSystem (shortDesc) << longDesc.str () << "\n";
	    }
	    catch (std::exception& e) 
	    {
		finalizeTimer ();
		std::string shortDesc ("StdLibException");
		std::ostringstream longDesc;
		longDesc << "Standard library exception caught in "
			 << "iguana -s CMSSW"
			 << "\n"
			 << e.what();
		rc = 8002;
		procService->jobReport ()->reportError (shortDesc, longDesc.str (), rc);
		edm::LogSystem (shortDesc) << longDesc.str () << "\n";
	    }
	    catch (...) 
	    {
		finalizeTimer ();
		std::string shortDesc ("UnknownException");
		std::ostringstream longDesc;
		longDesc << "Unknown exception caught in "
			 << "iguana -s CMSSW"
			 << "\n";
		rc = 8003;
		procService->jobReport ()->reportError (shortDesc, longDesc.str (), rc);
		edm::LogSystem (shortDesc) << longDesc.str () << "\n";
	    }
	}
	m_done = true;
    }
    else 
    {	
	IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state);
	ASSERT (sbar);
	sbar->setMessage (QString ("Waiting for an event..."));
    }	
}

void
VisEventMenu::finalizeTimer (void) 
{
    if (m_timer->isActive ())
    {
	m_auto ? m_auto = false : m_auto = true;
 	m_actionAuto->setChecked(m_auto);
	m_timer->stop ();
	m_timer->disconnect ();
    }
}

void
VisEventMenu::processEvent (unsigned int runNum, unsigned int eventNum) 
{
    if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
    {
	VisExceptionService *exService = VisExceptionService::get (m_state);
	if (! exService)
	{
	    exService = new VisExceptionService (m_state);
	}
	try 
	{    
	    (*procService)->run(edm::EventID(runNum, 0U, eventNum));
	}
	catch (cms::Exception& e)
	{
	    exService->cmsExceptionCallback (&e);
	}
	catch (lat::Error& e)
	{
	    exService->errorCallback (&e);
	}
	catch (std::exception& e)
	{
	    exService->exceptionCallback (&e);
	}
	catch (...)
	{
	    exService->unhandledExceptionCallback ();
	}
    }
}

void
VisEventMenu::processEventDialog (void)
{
  m_eventSelector->show();

  if (m_eventSelector->exec () == QDialog::Accepted
      && ! m_eventSelector->runNumberText->text ().isEmpty()
      && ! m_eventSelector->eventNumberText->text ().isEmpty())
  {
    processEvent (m_eventSelector->runNumberText->text ().toUInt (), m_eventSelector->eventNumberText->text ().toUInt ());
  }
}

void
VisEventMenu::rewind (void) 
{
    if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
    {
	VisExceptionService *exService = VisExceptionService::get (m_state);
	if (! exService)
	{
	    exService = new VisExceptionService (m_state);
	}
	try 
	{
	    (*procService)->rewind ();
	    (*procService)->run (1);
	}
	catch (cms::Exception& e)
	{
	    exService->cmsExceptionCallback (&e);
	}
	catch (lat::Error& e)
	{
	    exService->errorCallback (&e);
	}
	catch (std::exception& e)
	{
	    exService->exceptionCallback (&e);
	}
	catch (...)
	{
	    exService->unhandledExceptionCallback ();
	}
    }
}

void
VisEventMenu::skipEventDialog (void) 
{
    VisEventNavigationDialog *eventSelector = new VisEventNavigationDialog ();
    eventSelector->setModal (true);
    QRegExp rx ("\\d+");
    eventSelector->skipNumberLineEdit->setValidator (new QRegExpValidator (rx, this));
    eventSelector->show ();

    if (eventSelector->exec () == QDialog::Accepted)
    {
	eventSelector->buttonGroup->selectedId () == 0 ? skipEvent (eventSelector->skipNumberLineEdit->text ().toULong ()) :
						         skipEvent (-eventSelector->skipNumberLineEdit->text ().toULong ());
    }
    delete eventSelector;
}

void
VisEventMenu::skipEvent (long num) 
{
    IgArgsElement *args = IgArgsElement::get (m_state);

    if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
    {
	try
	{
	    (*procService)->skip (num - 1);
	    (*procService)->run (1);
	}
	catch (lat::Error& e)
	{
	    QMessageBox::critical (0, "IGUANA", QString ("<p>Exception caught in %1:</p>"
							 "<p>%2</p>")
				   .arg (QString::fromStdString (args->argv () [0]))
				   .arg (QString::fromStdString (e.explainSelf ())));
	}
	catch (std::exception& e)
	{
	    QMessageBox::critical (0, "IGUANA", QString ("<p>Standard library exception caught in %1:</p>"
							 "<p>%2</p>")
				   .arg (QString::fromStdString (args->argv () [0]))
				   .arg (QString::fromStdString (e.what ())));
	}
	catch (...)
	{
	    QMessageBox::critical (0, "IGUANA", QString ("<p>Unknown exception caught in %1</p>")
				   .arg (QString::fromStdString (args->argv () [0])));
	}
    }
}

void
VisEventMenu::l1TriggerDialog (void)
{
    VisL1TriggerMenu *dialog = new VisL1TriggerMenu ();
    
    if (VisL1FilterService *trgService = VisL1FilterService::get (m_state))
    {
	const VisL1TriggerMap amap = trgService->algoMap ();
// 	QCheckListItem *topItem =  new QCheckListItem (dialog->listView, "Name", 
// 						       "Bit");
	for (VisL1TriggerMap::const_iterator mit = amap.begin(), mitEnd = amap.end(); mit != mitEnd; ++mit)
	{
	    std::string name = mit->first;
	    std::string key = mit->second;
	    QString s = (QString::fromStdString (name));	    
	    QStringList snames = QStringList::split (": ", s);

// 	    QCheckListItem *listItem =  new QCheckListItem (dialog->listView, QString (name).section (": ", 1, 1), 
// 							    QString (name).left (QString (name).find (":")));

// 	    std::cout << "name: " << name << ", key: " << key << std::endl;
// 	    std::cout << "Bit: " << snames [0] << ", Name: " << snames [1] << std::endl;
	    
	    Q3ListViewItem *listItem =  new Q3ListViewItem (dialog->listView, snames [1], 
							    snames [0], QString::fromStdString (key));
	    listItem->renameEnabled (2);
	}
    }
    
    dialog->setModal (true);
    dialog->show ();
    
    int nEvents = 0;
    int maxEvents = 100;
    
    if (dialog->exec () == QDialog::Accepted)
    {
	if (VisL1FilterService *trgService = VisL1FilterService::get (m_state))
	{
	    while (! (trgService->filter ()) || nEvents++ < maxEvents)
	    {
		processOneEvent ();
	    }
	}
    }
    
    delete dialog;
}

void
VisEventMenu::autoPrint (void)
{ 
  m_print ? m_print = false : m_print = true;
  m_actionPrint->setChecked(m_print);

  if (m_print)
  {
    connect (m_printTimer, SIGNAL (timeout ()), SLOT (print ()));
    m_printTimer->start (m_printDelay.value (), false);
  }
  else
  {
    m_printTimer->stop ();
    m_printTimer->disconnect ();
  }
}

void
VisEventMenu::print (void)
{ 
  IgPage *currentPage = IgDocumentData::get (m_state)->document ()->currentPage ();
  QString text ("No information available.");  
  currentPage->printViews (text);
}

