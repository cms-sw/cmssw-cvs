//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Studio/interface/IgQtScheduledEvent.h"
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

IgQtScheduledEvent::IgQtScheduledEvent (lat::Callback callback)
    : QCustomEvent (EVENT_ID),
      m_callback (callback)
{
}

lat::Callback
IgQtScheduledEvent::callback (void)
{
    return m_callback;    
}

