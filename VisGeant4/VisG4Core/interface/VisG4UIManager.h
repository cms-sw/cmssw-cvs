#ifndef VIS_G4_CORE_VIS_G4_UI_MANAGER_H
# define VIS_G4_CORE_VIS_G4_UI_MANAGER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Core/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <pthread.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisG4UIShell;
class G4UImanager;
class G4UIterminal;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

extern "C" void *VisG4UIManagerCmdLoop (void *);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Geant4 command-line management service for IGUANA.  */
class VIS_G4_CORE_API VisG4UIManager : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (VisG4UIManager);
public:
    VisG4UIManager (IgState *state);

    virtual void		start (void);

private:
    friend void *		VisG4UIManagerCmdLoop (void *);
    void			cmdLoop (void);
    void			onQuit (void);

    IgState			*m_state;
    G4UImanager			*m_ui;
    VisG4UIShell			*m_shell;
    G4UIterminal		*m_terminal;
    pthread_t			m_cmdline;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_CORE_VIS_G4_UI_MANAGER_H
