#ifndef VIS_G4_CORE_VIS_G4_UI_SHELL_H
# define VIS_G4_CORE_VIS_G4_UI_SHELL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Core/interface/config.h"
# include <G4UItcsh.hh>
# include <pthread.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** IGUANA override for the Geant4 command line shell.

    This class overrides the Geant4 command line shell to synchronise
    the GUI and Geant4 threads (the latter runs the command line among
    other things).  Consequently it also synchronises the exit so that
    exit from either triggers the other to quit.  */
class VIS_G4_CORE_API VisG4UIShell : public G4UItcsh
{
public:
    VisG4UIShell (const G4String &prompt, G4int maxhist);

    virtual G4String	GetCommandLine (const char *msg);
    virtual void	Quit (void);

private:
    G4String		MyGetCommandLine (const char *msg);
    G4String		MyReadLine (void);
    char		ReadChar (void);

    pthread_mutex_t	m_lock;
    bool		m_reading;
    bool		m_quit;
    int			m_oncancel [2];
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_CORE_VIS_G4_UI_SHELL_H
