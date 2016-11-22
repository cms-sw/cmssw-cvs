#ifndef VIS_SIM_BASE_VIS_SIM_QT_ACTIONS_H
# define VIS_SIM_BASE_VIS_SIM_QT_ACTIONS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisSimQtActions : public IgExtension
{
    IG_DECLARE_STATE_ELEMENT (VisSimQtActions);
public:
    VisSimQtActions (IgState *state);
    ~VisSimQtActions (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

private:
    void		init (void);
    IgState *		m_state;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_SIM_BASE_VIS_SIM_QT_ACTIONS_H
