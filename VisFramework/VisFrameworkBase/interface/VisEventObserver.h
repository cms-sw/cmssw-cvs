#ifndef VIS_FRAMEWORK_BASE_VIS_EVENT_OBSERVER_H
# define VIS_FRAMEWORK_BASE_VIS_EVENT_OBSERVER_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "sigc++/signal.h"
#include "sigc++/connection.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

namespace edm 
{
    class Event;
    class EventSetup;
}

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisEventObserver
{
public:
    VisEventObserver (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual ~VisEventObserver (void);

    virtual void	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup) = 0;

    void 	       	init (void);

private:
    IgState	       	*m_state;		//< State context.
    sigc::connection   	m_connection;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_EVENT_OBSERVER_H
