#ifndef IGUANA_STUDIO_IG_QT_SCHEDULED_EVENT_H
# define IGUANA_STUDIO_IG_QT_SCHEDULED_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include <classlib/utils/Callback.h>
# include <qevent.h>
//Added by qt3to4:
#include <QCustomEvent>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtScheduledEvent : public QCustomEvent
{
public:
    static const int EVENT_ID = QEvent::User + 1;
    IgQtScheduledEvent (lat::Callback action);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    lat::Callback callback (void);
private:
    lat::Callback 	m_callback;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_SCHEDULED_EVENT_H
