//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Core/interface/VisG4UIManager.h"
#include "VisGeant4/VisG4Core/interface/VisG4UIShell.h"
#include "Iguana/Studio/interface/IgQtAppLoopService.h"
#include <classlib/utils/DebugAids.h>
#include <G4UImanager.hh>
#include <G4UIterminal.hh>
#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (VisG4UIManager, "Services/Geant4/UI Manager");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4UIManager::VisG4UIManager (IgState *state)
    : m_state (state),
      m_ui (G4UImanager::GetUIpointer ()),
      m_shell (new VisG4UIShell ("Idle> ", 10)),
      m_terminal (new G4UIterminal (m_shell)),
      m_cmdline (0)
{ ASSERT (m_state); m_state->put (s_key, this); }

extern "C" void *VisG4UIManagerCmdLoop (void *context)
{ ((VisG4UIManager *) context)->cmdLoop (); return 0; }

void
VisG4UIManager::start (void)
{
    // Start the command line in another thread
    ASSERT (! m_cmdline);
    pthread_create (&m_cmdline, 0, VisG4UIManagerCmdLoop, this);

    // Install an application loop post-action to synchronise
    // the exit with the command line.
    ASSERT (IgQtAppLoopService::get (m_state));
    IgQtAppLoopService::get (m_state)->post
	(lat::CreateCallback (this, &VisG4UIManager::onQuit));
}

void
VisG4UIManager::onQuit (void)
{
    // GUI has exited.  Tell command line to exit the next time it
    // reads input (or right now if it is currently reading input).
    // This operation is safe even if the command line has already
    // exited -- the shell is still around and simply remembers that
    // it needs to quit the next time around.
    m_shell->Quit ();

    pthread_join (m_cmdline, 0);
    m_cmdline = 0;

    delete m_terminal;
    m_terminal = 0;
    m_shell = 0;
}

void
VisG4UIManager::cmdLoop (void)
{
    // Execute custom startup script.
    // m_ui->ApplyCommand ("/control/execute startup.mac");

    // Run the command line until exit.
    ASSERT (m_terminal);
    m_terminal->SessionStart ();

    // Signal and wait for the GUI to exit.  We do not really have to
    // synchronise.  Right now (Qt 2.2.4) calling quit() is harmless
    // even if we do it too late (whether already exiting, or exited).
    // Qt processes quitting mostly unlocked so locking does us no
    // extra good, but we do that anyway for an extra measure of
    // safety.  The wake-up from unlock is also harmless if the GUI
    // has already quit.
    qApp->lock ();
    qApp->quit ();
    qApp->unlock ();
}
