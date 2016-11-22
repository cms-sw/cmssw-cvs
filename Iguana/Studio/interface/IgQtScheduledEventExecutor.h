#ifndef IGUANA_STUDIO_IG_QT_SCHEDULED_EVENT_EXECUTOR_H
# define IGUANA_STUDIO_IG_QT_SCHEDULED_EVENT_EXECUTOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include <qobject.h>
//Added by qt3to4:
#include <QCustomEvent>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QCustomEvent;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtScheduledEventExecutor : public QObject
{
public:
    IgQtScheduledEventExecutor (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
protected:
    void customEvent  (QCustomEvent *event);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_SCHEDULED_EVENT_EXECUTOR_H
