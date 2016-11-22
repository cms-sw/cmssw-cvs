//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Studio/interface/IgQtScheduledEventExecutor.h"
#include "Iguana/Studio/interface/IgQtScheduledEvent.h"
#include <classlib/utils/DebugAids.h>
#include <qevent.h>
#include <qobject.h>
//Added by qt3to4:
#include <QCustomEvent>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtScheduledEventExecutor::IgQtScheduledEventExecutor (void)
    : QObject ()
{
}

void
IgQtScheduledEventExecutor::customEvent (QCustomEvent *event)
{
    IgQtScheduledEvent * scheduledEvent 
	= dynamic_cast <IgQtScheduledEvent *> (event);
    ASSERT (scheduledEvent);    
    (scheduledEvent->callback ()) ();
}
