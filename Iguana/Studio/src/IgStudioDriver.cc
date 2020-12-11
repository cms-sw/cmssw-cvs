#include "Iguana/Studio/interface/IgStudioDriver.h"
#include "Iguana/Studio/interface/IgStudioSetupExtension.h"
#include "Iguana/Studio/interface/IgStudioSetup.h"
#include "Iguana/Studio/interface/IgQtSite.h"
#include "Iguana/Studio/interface/IgQtAppExtensions.h"
#include "Iguana/Studio/interface/IgQtAppLoopService.h"
#include "Iguana/Studio/interface/IgQtAppActiveService.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/Studio/interface/IgQMainWindowSite.h"
#include "Iguana/Framework/interface/IgArgsElement.h"
#include "Iguana/Framework/interface/IgEnvsElement.h"
#include "Iguana/Framework/interface/IgPluginLoader.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgSelectionService.h"
#include "Iguana/Studio/interface/IgModelManager.h"
#include "Iguana/Studio/interface/IgDocumentDataManager.h"
#include <QApplication>
#include <QMainWindow>
#include <QWindowsStyle>
#include <QMessageBox>
#include <QSplashScreen>
#include <map>
#include <cstdlib>
#include <assert.h>

const char *
IgStudioDriver::catalogLabel (void)
{ return "IGUANA"; }

IgStudioDriver::IgStudioDriver (IgState *state)
    : m_state (state),
      m_setup (0),
      m_mainSite (0),
      m_mainWindow (0)
{
  QCoreApplication::setApplicationName("iguana");
  QCoreApplication::setApplicationVersion("CMSSW_3_3_x");
  QCoreApplication::setOrganizationDomain("iguana");
  QCoreApplication::setOrganizationName("iguana");
}

IgSite *
IgStudioDriver::mainSite (void) const
{ return m_mainSite; }

QMainWindow *
IgStudioDriver::mainWindow (void) const
{ return m_mainWindow; }

IgState *
IgStudioDriver::state (void) const
{ return m_state; }

int
IgStudioDriver::run (void)
{
    // Qt scales screen fonts by point size (1/72 inches), not pixel
    // size; the default is 12pt Helvetica.  To get pixel size font,
    // the point size is scaled by the real monitor resolution if it
    // is known.  Thus, on a 100 DPI screen (when correctly reported
    // by X) Qt will use 16-pixel fonts.  I don't know about you but
    // on my screen that just looks plain ugly, so we just tell Qt to
    // ignore to X11 on monitor resolution.
    //
    // You should also make sure you do not have ISO 10646 (Unicode)
    // fonts installed on your X11 server, at least not before any of
    // the normal fonts (at least as of Qt 3.0.1).  They tend to have
    // much larger vertical size (e.g. 20 vs. 15 pixels), presumably
    // to accommodate complex scripts like ideographs.  We don't use
    // any such characters in the GUI, so things look quite odd.
    // (FIXME: Alternatively tell Qt to not use the 10646 variants of
    // the fonts -- can we?)
#if !defined WIN32 && !defined QT_MACOSX_VERSION
    if (! ::getenv ("IGUANA_KEEP_X11_RESOLUTION"))
    {
	QPaintDevice::x11SetAppDpiX (72, 72);
	QPaintDevice::x11SetAppDpiY (72, 72);
    }
#endif //WIN32

    // Initialise Qt
    IgArgsElement	*args = IgArgsElement::get (m_state);
    IgEnvsElement	*envs = IgEnvsElement::get (m_state);
    QApplication	qapp (args->args (), args->argv ());
    QApplication::setOverrideCursor (Qt::waitCursor);
    
    // Show splash screen
    QSplashScreen *splash = new QSplashScreen(QPixmap(QString::fromUtf8(":/icons/IGUANA_SPLASH.xpm")));
    splash->show();

    // Load environment
    // and update message on the splash screen
    loadSettings ();
    splash->message ("Loaded settings");
    qApp->processEvents ();    

    // Setup main window
    // and update message on the splash screen
    setupMainWindow ();
    splash->message ("Set up main window");
    qApp->processEvents ();    

    // Delete splash screen and restore cursor   
    splash->clear ();
    QApplication::restoreOverrideCursor ();
    delete splash;

    QString sessionType ("");
    std::string ext ("");
    
    std::vector<std::string> arg;
    arg.push_back ("--iguana-session");
    arg.push_back ("-is");
    
    if (args->find (arg, ext)
	|| envs->getEnv ("IGUANA_SETUP", ext))
        sessionType = ext.c_str ();

    // Put CMSSW session as a default one if cfg or root file
    // is given as an argument
    if (sessionType.isEmpty () && (args->args () == 2))
    {
	std::string fileName (args->arg (1));
	if ((fileName.size () > 2 && fileName.substr (fileName.size () - 3) == ".py")
	    || (fileName.size () > 3 && fileName.substr (fileName.size () - 4) == ".cfg") 
	    || (fileName.size() > 4 && fileName.substr (fileName.size () - 5) == ".root"))
	{
	    sessionType = "CMSSW";
	}
    }
    
    
    // Pick a session driver once we run  
    if (! setupSession (sessionType))
	return EXIT_FAILURE;

    // Run the application.  (FIXME: always in separate thread?)
    IgQtAppLoopService::get (m_state)->run ();
    delete m_setup;
    return EXIT_SUCCESS;
}

void
IgStudioDriver::loadSettings (void)
{
    // FIXME: load parameters and environment settings, preferences
}

void
IgStudioDriver::setupMainWindow (void)
{
  m_mainSite = new IgQMainWindowSite (m_state, 0);
  m_mainWindow = dynamic_cast <QMainWindow *>(IgQtSite::selfFrom (m_mainSite));
    qApp->setMainWidget (m_mainWindow);
#ifndef QT_MACOSX_VERSION // not on mac
    qApp->setStyle (new QWindowsStyle);
#endif

    IgQtAppExtensions::install (m_state, m_mainWindow);
    assert (IgQtAppLoopService::get (m_state));

    // FIXME: Need a better way to setup state
    new IgSelectionService (m_state);
    new IgQtObjectMenuService (m_state);
    new IgQtAppActiveService (m_state);
    new IgDocumentDataManager (m_state);
    new IgModelManager (m_state);

    std::string caption("IGUANA");
    m_mainWindow->setCaption (caption.c_str());
    
    unsigned int x = 1000, y=760;
    m_mainWindow->resize (x, y);

    // FIXME: add standard menu items like quit, ...
    // FIXME: add standard tool bar buttons

    // Now auto-load all other GUI extensions.
    assert (IgPluginLoader::get (m_state));
    IgPluginLoader::get (m_state)->load (m_state, "Extensions/Qt GUI/", true);

    // Make it visible
    m_mainWindow->show ();
}

bool
IgStudioDriver::setupSession (QString sessionType)
{
    // FIXME: Use own plug-in category
    assert (! m_setup);

    std::string	prefix ("Services/Studio/Session Types/");
    std::string	preferred;
    
    // Build list of available session drivers (FIXME: remember last?)
    IgExtensionDB::Iterator ext = IgExtensionDB::get ()->begin ();
    IgExtensionDB::Iterator last = IgExtensionDB::get ()->end ();

    if (! sessionType.isEmpty ()) 
    {
	bool found = false;
	for ( ; (ext != last) && (!found); ++ext)
	{
	  std::string fullname = (*ext)->name();
	  std::vector<std::string> aliases = getAliases (fullname, prefix);
	  
	  for (unsigned int i = 0; i < aliases.size (); i++)
	  {
	    if (aliases [i] == sessionType.latin1() )
	    {
	      preferred = fullname;
	      found = true;
	      break;
	    }
	  }
	}
	  
	if (!found)
	{
	    QMessageBox::information (0, "IGUANA Setup", QString ("<p>IGUANA_SETUP environment variable"
								  " is set to <b>\"%1\"</b>. This session type"
								  " is not available.</p>"
								  "<p>Please select one from the next setup"
								  " page.").arg (sessionType));
	    sessionType = "";
	}
    }

    if (sessionType.isEmpty ()) 
    {
	// Create the setup window
	IgStudioSetup *setup = new IgStudioSetup (qApp->mainWidget ());
	QListWidget *configs = setup->m_configs;

	ext = IgExtensionDB::get ()->begin ();
	int select = 0;

        std::map <std::string , std::string> aliasesMap;
	for ( ; ext != last; ++ext)
	{
	  std::string fullname = (*ext)->name();
	  std::vector<std::string> aliases = getAliases (fullname, prefix);
	  
	  for(unsigned int i = 0; i < aliases.size (); i++)
	    aliasesMap [aliases [i]] = fullname;
	  
	  if (aliases.size ())
	  {
	    configs->addItem(new QListWidgetItem(QString::fromStdString(aliases [0])));
	    if (! preferred.empty () && fullname == preferred)
	      select = configs->count ()-1;
	  }
	}

	if (! configs->count ())
	{
	    QMessageBox::critical (0, "IGUANA", "<p>You do not seem to have any"
				   " IGUANA session configurations available in"
				   " your system at the moment (cf. ``iguana"
				   " --list'', $IGUANA_PLUGINS).  I cannot"
				   " continue without one.</p>"
				   "<p>Please correct the above problem and try"
				   " again.");
	    delete setup;
	    return false;
	}

	configs->setCurrentItem(configs->item(0));
	preferred = "";

	// Let the user select the driver
	if (setup->exec () != QDialog::Accepted || configs->currentItem () == 0)
	{
	    delete setup;
	    return false;
	}

	preferred = aliasesMap[ configs->currentItem()->text().toStdString() ];
	delete setup;
    }
    
    // Instantiate the selected session driver
    assert (! preferred.empty ());
    IgStudioSetupExtension	*sext = 0;
    IgExtension			*newext = 0;

    try
    {
	// Try to instantiate it
	QApplication::setOverrideCursor (Qt::waitCursor);
	newext = IgExtensionDB::get ()->create (preferred, m_state);

	// Make sure we got what we expect
	assert (newext);
	assert (dynamic_cast<IgStudioSetupExtension *> (newext));
	sext = static_cast<IgStudioSetupExtension *> (newext);
	QApplication::restoreOverrideCursor ();

	// Try to set it up; if fails, take short cut out
	if (! sext->setup (this))
	    throw 1;
    }
    // FIXME: too broad...
    catch (...)
    {
	QApplication::restoreOverrideCursor ();
	delete newext;
	return false;
    }

    m_setup = sext;
    return true;
}

std::vector<std::string>
IgStudioDriver::getAliases (const std::string& name, const std::string& type)
{
  std::vector<std::string> aliases;
  if (name.substr (0, type.length ()) != type){ return aliases; }
  QString substr (&name[type.length ()]);
  QRegExp search (";\\s+");
  while (!substr.isEmpty ())
  {
    int index = substr.find(search);
    if (index == 0)
    { substr = substr.mid (index+search.matchedLength ()); }
    else if (index > 0)
    {
      aliases.push_back (substr.left (index).latin1());
      substr = substr.mid (index+search.matchedLength ());
    }
    else
    {
      aliases.push_back (substr.latin1());
      substr="";
    }
  }
  return aliases;
}
