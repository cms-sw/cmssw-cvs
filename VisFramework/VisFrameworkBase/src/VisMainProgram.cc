#include "VisFramework/VisFrameworkBase/interface/VisMainProgram.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "VisFramework/VisFrameworkBase/interface/VisExceptionService.h"
#include "Iguana/Framework/interface/IgArgsElement.h"
#include "Iguana/Studio/interface/IgQtAppLoopService.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include "Iguana/Studio/interface/IgQtAppWindowService.h"
#include <classlib/utils/Callback.h>
#include <QApplication>
#include <QMessageBox>
#include <unistd.h>
#include <iostream>

VisMainProgram::VisMainProgram (IgState *state)
  : m_state (state),
    m_mainWindow (IgQtAppWindowService::get (state)->mainWindow ())
{
  init ();

  // Auto-register a post-action to force a quit.
  IgQtAppLoopService::get (m_state)->post
    (lat::CreateCallback (this, &VisMainProgram::quit));
}

void
VisMainProgram::init (void) 
{
  VisEventProcessorService *procService 
    = VisEventProcessorService::get (m_state);

  // Make sure the event processor is initialised.
  if (! procService) 
  {
    procService = new VisEventProcessorService (m_state);

    procService->onProcessorStarted (
      lat::CreateCallback (this, 
			   &VisMainProgram::processorStartedCallback));

    procService->onProcessorStopped (
      lat::CreateCallback (this, 
			   &VisMainProgram::processorStoppedCallback));

    procService->onStartingProcessor (
      lat::CreateCallback (this,
			   &VisMainProgram::startingProcessorCallback));

    procService->onStoppingProcessor (
      lat::CreateCallback (this,
			   &VisMainProgram::stoppingProcessorCallback));

    procService->onException (
      lat::CreateCallback (this, 
			   &VisMainProgram::exceptionCallback));

    procService->onCmsException (
      lat::CreateCallback (this, 
			   &VisMainProgram::cmsExceptionCallback));

    procService->onFrameworkException (
      lat::CreateCallback (this, 
			   &VisMainProgram::frameworkExceptionCallback));

    procService->onUnhandledException (
      lat::CreateCallback (this, 
			   &VisMainProgram::unhandledExceptionCallback));

    procService->onConfigFailure (
      lat::CreateCallback (this, 
			   &VisMainProgram::configFailureCallback));

    procService->onStatusMessage (
      lat::CreateCallback (this, 
			   &VisMainProgram::statusMessageCallback));
	
    procService->initEventProcessor ();
  }

  VisExceptionService *exService = VisExceptionService::get (m_state);
  if (! exService)
  {
    exService = new VisExceptionService (m_state);
  }
}

void
VisMainProgram::quit (void)
{
  // Make sure the event processor is finalized.
  if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
  {
    procService->scheduleAbort ();
    delete procService;
  }

  qApp->quit ();
}

void
VisMainProgram::frameworkExceptionCallback (lat::Error *e)
{
  IgArgsElement *args = IgArgsElement::get (m_state);
    
  QApplication::restoreOverrideCursor ();

  // Make sure the event processor is finalized.
  if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
  {
    delete procService;
  }

  QMessageBox::critical (m_mainWindow, "IGUANA", 
			 QString ("<p>Exception caught in %1:</p>"
				  "<p>%2</p>")
			 .arg (QString::fromStdString (args->argv () [0]))
			 .arg (QString::fromStdString (e->explainSelf ())));
  qApp->quit ();    	
}

void
VisMainProgram::exceptionCallback (std::exception *e)
{
  IgArgsElement *args = IgArgsElement::get (m_state);

  QApplication::restoreOverrideCursor ();

  // Make sure the event processor is finalized.
  if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
  {
    delete procService;
  }

  QMessageBox::critical (m_mainWindow, "IGUANA", 
			 QString ("<p>Standard library exception caught in %1:</p>"
				  "<p>%2</p>")
			 .arg (QString::fromStdString (args->argv () [0]))
			 .arg (QString::fromStdString (e->what ())));
  qApp->quit ();
}

void
VisMainProgram::cmsExceptionCallback (cms::Exception *e)
{
  IgArgsElement *args = IgArgsElement::get (m_state);

  QApplication::restoreOverrideCursor ();

  // Make sure the event processor is finalized.
  if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
  {
    delete procService;
  }

  QMessageBox::critical (m_mainWindow, "IGUANA", 
			 QString ("<p>CMS Exception caught in %1:</p>"
				  "<p>%2</p>")
			 .arg (QString::fromStdString (args->argv () [0]))
			 .arg (QString::fromStdString (e->what ())));
  qApp->quit ();
}

void
VisMainProgram::unhandledExceptionCallback (void)
{
  IgArgsElement *args = IgArgsElement::get (m_state);

  QApplication::restoreOverrideCursor ();

  // Make sure the event processor is finalized.
  if (VisEventProcessorService *procService = VisEventProcessorService::get (m_state)) 
  {
    delete procService;
  }

  QMessageBox::critical (m_mainWindow, "IGUANA", 
			 QString ("<p>Unknown exception caught in %1</p>")
			 .arg (QString::fromStdString (args->argv () [0])));
  qApp->quit ();
}

void
VisMainProgram::startingProcessorCallback (void)
{
  QApplication::setOverrideCursor (Qt::waitCursor);
  if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
    sbar->setMessage (QString ("Start Event Processor initialization..."));
}

void
VisMainProgram::stoppingProcessorCallback (void)
{
  QApplication::setOverrideCursor (Qt::waitCursor);
  if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
    sbar->setMessage (QString ("Stopping running Event Processor..."));
}

void
VisMainProgram::processorStoppedCallback (void)
{
  QApplication::setOverrideCursor (Qt::waitCursor);
  if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
    sbar->setMessage (QString ("Event Processor stopped..."));
}

void
VisMainProgram::processorStartedCallback (void)
{
  QApplication::restoreOverrideCursor ();
  if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
    sbar->setMessage (QString ("Event Processor is initialized."));    
}

void
VisMainProgram::configFailureCallback (cms::Exception *e)
{
  IgArgsElement *args = IgArgsElement::get (m_state);
  std::vector <std::string> parameterSetOpts;
  parameterSetOpts.push_back ("--parameter-set");
  parameterSetOpts.push_back ("-p");
  std::string parameterSetFilename;
  args->find (parameterSetOpts, parameterSetFilename, false);

  if (parameterSetFilename.empty () && (args->args () > 1)) 
  {
    std::string fileName (args->arg (1));
    parameterSetFilename = fileName;
  }

  QApplication::restoreOverrideCursor ();

  switch (QMessageBox::critical (m_mainWindow, "IGUANA", QString ("<p><b>Attention:</b> Please, check that \"%1\" does exist and "
								  "its Python syntax is valid.</p>"
								  "<p>Use <i>python -i yourFileName_cfg.py</i> to check the syntax "
								  "(Ctrl+D to exit Python shell).</p>"
								  "Use iguana as follows:<br> <i>> iguana yourFileName_cfg.py</i>"
								  "<p>%2 will load a default configuration with an EmptySource.</p>"
								  "<p><b>CMS Exception:</b></p>"
								  "<p>%3</p>")
				 .arg (QString::fromStdString (parameterSetFilename))
				 .arg (QString::fromStdString (args->argv () [0]))
				 .arg (QString::fromStdString (e->what ())), QMessageBox::Ok, QMessageBox::Cancel)) 
  {
  case QMessageBox::Ok:
    break;
  case QMessageBox::Cancel:
    std::cout << "Quit" << std::endl;
	
    this->quit ();
    break;
  }
}

void
VisMainProgram::statusMessageCallback (const std::string& message)
{
  if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (m_state))
    sbar->setMessage (message);    
}
