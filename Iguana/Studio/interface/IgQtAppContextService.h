#ifndef IGUANA_STUDIO_IG_QT_APP_CONTEXT_SERVICE_H
# define IGUANA_STUDIO_IG_QT_APP_CONTEXT_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include "classlib/utils/Callback.h"
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtAppContextService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppContextService);
public:
    typedef void (*ConnectionCallback) (void *data, bool focus);    

    IgQtAppContextService (IgState *state, QWidget *mainWindow);
    ~IgQtAppContextService (void);

    void add (ConnectionCallback connection, 
	      void *data, bool enable = true);

    void addOnFocusIn (lat::CallbackRep *callback);
    void addOnFocusOut (lat::CallbackRep *callback);
            
    void focusIn (void);
    void focusOut (void);
    
    // FIXME: methods to manage/switch current user attention
    // context; use in pre-menu-popups to enable/disable items,
    // to switch context menu contents, and to determine what
    // window/context menu items like "Print" should apply to.

private:
    IgState			*m_state;
    QWidget			*m_mainWindow;
    
    typedef std::list<lat::Callback> CallbackList;

    CallbackList		m_focusInCallbacks;
    CallbackList		m_focusOutCallbacks;    
    
    // undefined semantics
    IgQtAppContextService (const IgQtAppContextService &);
    IgQtAppContextService &operator= (const IgQtAppContextService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_APP_CONTEXT_SERVICE_H
