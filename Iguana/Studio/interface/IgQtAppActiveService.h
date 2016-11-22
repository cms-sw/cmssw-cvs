#ifndef IGUANA_STUDIO_IG_APP_ACTIVE_SERVICE_H
# define IGUANA_STUDIO_IG_APP_ACTIVE_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgDispatcher.h"
# include "Iguana/Framework/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IGUANA_STUDIO_API IgQtAppActiveMessage
{
public:
    IgQtAppActiveMessage (QWidget *widget, bool active)
      : m_widget (widget),
        m_active  (active)
      { }
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    QWidget*		widget (void) const
    { return m_widget; }
    
    bool		active (void) const
    { return m_active; }

private:
    QWidget *		m_widget;
    bool		m_active;
};

class IGUANA_STUDIO_API IgQtAppActiveService
    : public IgStateElement, public IgDispatcher<IgQtAppActiveMessage>
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppActiveService);
public:
    IgQtAppActiveService (IgState *state);

private:
    IgState		*m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_APP_ACTIVE_SERVICE_H
