#ifndef IGUANA_STUDIO_IG_QT_APP_LOOP_SERVICE_H
# define IGUANA_STUDIO_IG_QT_APP_LOOP_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <classlib/utils/Callback.h>
# include <list>
# include <setjmp.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgQtScheduledEventExecutor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtAppLoopService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppLoopService);
public:
    IgQtAppLoopService (IgState *state);
    ~IgQtAppLoopService (void);

    virtual void		prepare (lat::Callback action);
    virtual void		post (lat::Callback action);
    virtual void		schedule (lat::Callback action);    
    virtual void		recover (lat::Callback action);
    virtual void		override (lat::Callback action);

    virtual void		run (void);
    virtual bool		running (void);

    // crash recovery
    virtual void		resume (void);

protected:
    virtual void		doPrepare (void);
    virtual void		doRun (void);
    virtual void		doResume (void);
    virtual void		doPost (void);

private:
    static void			defaultRunAction (void);

    IgState			*m_state;
    std::list<lat::Callback>	m_preparations;
    std::list<lat::Callback>	m_recoveries;
    std::list<lat::Callback>	m_post;
    jmp_buf			m_return;
    lat::Callback		m_run;
    bool			m_running;
    IgQtScheduledEventExecutor *m_executor;
    
    // undefined semantics
    IgQtAppLoopService (const IgQtAppLoopService &);
    IgQtAppLoopService &operator= (const IgQtAppLoopService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_APP_LOOP_SERVICE_H
