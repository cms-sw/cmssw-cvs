#ifndef IGUANA_MODELS_IG_TEXT_MODEL_EVENT_H
# define IGUANA_MODELS_IG_TEXT_MODEL_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Models/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_MODELS_API IgTextModelEvent
{
public:
    IgTextModelEvent (IgTextModel *source);
    // default copy constructor
    // default assignment operator
    // default destructor

    IgTextModel *		source (void) const;

private:
    IgTextModel *		m_source;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
IgTextModelEvent::IgTextModelEvent (IgTextModel *source)
    : m_source (source)
{}

inline IgTextModel *
IgTextModelEvent::source (void) const
{
    return m_source;
}

#endif // IGUANA_MODELS_IG_TEXT_MODEL_EVENT_H
