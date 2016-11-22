#ifndef VIS_FRAMEWORK_BASE_VIS_L1FILTER_SERVICE_H
# define VIS_FRAMEWORK_BASE_VIS_L1FILTER_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

typedef std::map <std::string, std::string> VisL1TriggerMap;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisL1FilterService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (VisL1FilterService);
public:
    VisL1FilterService (IgState *state);
    ~VisL1FilterService (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    bool	filter (void) { return m_passed; }
    void	filter (bool key) { m_passed = key; }
    bool 	initialized (void) { return m_init; }
    void	initialized (bool flag) { m_init = flag; }
    bool 	registerAlgo (std::string name, std::string key);
    bool	unregisterAlgo (const std::string &name);
    void 	algorithm (std::string name, std::string key);
    
    const VisL1TriggerMap& algoMap (void) const { return m_algos; }

private:    
    IgState    *m_state;
    bool 	m_init;
    bool	m_passed;
    VisL1TriggerMap m_algos;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_L1FILTER_SERVICE_H
