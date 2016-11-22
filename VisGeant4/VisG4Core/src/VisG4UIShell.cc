//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Core/interface/VisG4UIShell.h"
#include <classlib/utils/DebugAids.h>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4UIShell::VisG4UIShell (const G4String &prompt, G4int maxhist)
    : G4UItcsh (prompt, maxhist),
      m_reading (false),
      m_quit (false)
{
    pthread_mutex_init (&m_lock, 0);
    VERIFY (pipe (m_oncancel) != -1);
}

G4String
VisG4UIShell::GetCommandLine (const char *msg)
{
    G4String result;
    pthread_mutex_lock (&m_lock);
    if (! m_quit)
    {
	m_reading = true;
	pthread_mutex_unlock (&m_lock);

	result = MyGetCommandLine (msg);

	pthread_mutex_lock (&m_lock);
	m_reading = false;
    }

    // FIXME: What if we are in pause?  We'll have to answer continue!
    // Can we use G4StateManager to tell which answer we should give?
    if (m_quit)
	result = "exit";
    pthread_mutex_unlock (&m_lock);

    return result;
}

void
VisG4UIShell::Quit (void)
{
    // Set a flag to indicate that next time the command line reads
    // something, it should just exit.  If we are currently reading
    // something, interrupt and discard whatever the user was typing.
    pthread_mutex_lock (&m_lock);
    m_quit = true;
    if (m_reading)
	write (m_oncancel [1], "\n", 1);
    pthread_mutex_unlock (&m_lock);
}

char
VisG4UIShell::ReadChar (void)
{
    // Wait till we can read a character but let us be cancelled.
    fd_set r;
    FD_ZERO (&r);
    FD_SET (0, &r);
    FD_SET (m_oncancel[0], &r);
    while (select (m_oncancel[0]+1, &r, 0, 0, 0) == -1
	   && (0
#ifdef EAGAIN
	       || errno == EAGAIN
#endif
#ifdef EINTR
	       || errno == EINTR
#endif
#ifdef EWOULDBLOCK
	       || errno == EWOULDBLOCK
#endif
	       || 0))
	;

    if (FD_ISSET (m_oncancel [0], &r))
	// Someone wanted us to quit so get out of the loop.  In case
	// last line was continued, return something that will get us
	// out of the command line loop.
	return '\n';

    // OK to read from standard input.
    char cc;
    G4cin.get(cc);
    return cc;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Copied from G4UItcsh.cc.  Sigh.
G4String
VisG4UIShell::MyReadLine (void)
{
    static const char AsciiCtrA = '\001';
    static const char AsciiCtrB = '\002';
    static const char AsciiCtrC = '\003';
    static const char AsciiCtrD = '\004';
    static const char AsciiCtrE = '\005';
    static const char AsciiCtrF = '\006';
    static const char AsciiCtrK = '\013';
    static const char AsciiCtrL = '\014';
    static const char AsciiCtrN = '\016';
    static const char AsciiCtrP = '\020';
    static const char AsciiCtrQ = '\021';
    static const char AsciiCtrS = '\023';
    static const char AsciiCtrZ = '\032';
    static const char AsciiTAB  = '\011';
    static const char AsciiBS   = '\010';
    static const char AsciiDEL  = '\177';
    static const char AsciiESC  = '\033';

    InitializeCommandLine();

    char cc;
    do {
	// treatment for special character
	switch(cc = ReadChar ()){
	case AsciiCtrA:       // ... move cursor to the top
	    MoveCursorTop();
	    break;
	case AsciiCtrB:       // ... backward cursor
	    BackwardCursor();
	    break;
	case AsciiCtrD:       // ... delete/exit/show matched list
	    if(commandLine.length()!=0 && IsCursorLast()) ListMatchedCommand();
	    else if (commandLine.empty()) {
		G4cout << G4endl;
		exit(0);
	    } else DeleteCharacter();
	    break;
	case AsciiCtrE:       // ... move cursor to the end
	    MoveCursorEnd();
	    break;
	case AsciiCtrF:       // ... forward cursor
	    ForwardCursor();
	    break;
	case AsciiCtrK:       // ... clear after the cursor
	    ClearAfterCursor();
	    break;
	case AsciiCtrL:       // ... clear screen
	    // ClearScreen();
	    break;
	case AsciiCtrN:	// ... next command
	    NextCommand();
	    break;
	case AsciiCtrP:	// ... previous command
	    PreviousCommand();
	    break;
	case AsciiTAB:         // ... command completion
	    if( (!commandLine.empty()) && IsCursorLast()) CompleteCommand();
	    break;
	case AsciiDEL:         // ... backspace
	    BackspaceCharacter();
	    break;
	case AsciiBS:          // ... backspace
	    BackspaceCharacter();
	    break;
	case AsciiCtrC:       // ... kill prompt
	    break;
	case AsciiCtrQ:       // ... restarts suspeded output
	    break;
	case AsciiCtrS:       // ... suspend output
	    break;
	case AsciiCtrZ:       // ... suspend
	    break;
	default:
	    break;
	}

	// treatment for ESC. character
	if( cc == AsciiESC) { // ESC
	    G4cin.get(cc);
	    if (cc == '[') {
		G4cin.get(cc);
		switch(cc) {
		case 'A': // [UP]
		    cc = 'P' - '@';
		    PreviousCommand();  // ... show previous commad
		    break;
		case 'B': // [DOWN]
		    cc = 'N' - '@';
		    NextCommand();  // ... show next commad
		    break;
		case 'C': // [RIGHT]
		    cc = 'F' - '@';
		    ForwardCursor();   // ... forward cursor
		    break;
		case 'D': // [LEFT]
		    cc = 'B' - '@';
		    BackwardCursor();      // ... backward cursor
		    break;
		default:  // who knows !?
		    cc = 0;
		    break;
		}
	    }
	}

	// insert character to command line and display
	InsertCharacter(cc);
  
    } while( cc != '\n');

    return commandLine;
}

G4String
VisG4UIShell::MyGetCommandLine (const char *msg)
{
    SetTermToInputMode();

    MakePrompt(msg); // update
    relativeHistoryIndex= 0;

    G4cout << promptString << std::flush;

    G4String newCommand= MyReadLine();  // read line...
    // multi-line
    while( (newCommand.length() > 0) &&
	   ( newCommand[newCommand.length()-1] == '_') ) {
	newCommand.remove(newCommand.length()-1);
	G4cout << G4endl;
	promptString= "? ";
	G4cout << promptString << std::flush;
	G4String newLine= MyReadLine();
	newCommand.append(newLine);
    }

    // update history...
    G4bool isMeaningfull= FALSE; // check NULL command
    for (size_t i=0; i<newCommand.length(); i++) {
	if(newCommand[i] != ' ') {
	    isMeaningfull= TRUE;
	    break;
	}
    }
    if( !newCommand.empty() && isMeaningfull) StoreHistory(newCommand);

    // reset terminal
    RestoreTerm();

    G4cout << G4endl;
    return newCommand;
}
