#ifndef VIS_FRAMEWORK_BASE_VIS_TWIG_FACTROY_SERVICE_H
# define VIS_FRAMEWORK_BASE_VIS_TWIG_FACTROY_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisQueuedTwig;
class IgTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisTwigFactroyService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (VisTwigFactroyService);
public:
    typedef VisQueuedTwig * (*CreateTwigCallback)(IgState *state, IgTwig *parent,
						  const std::string &name,
						  const std::string &friendlyName,
						  const std::string &moduleLabel,
						  const std::string &instanceName,
						  const std::string &processName);    

    VisTwigFactroyService (IgState *state);
    ~VisTwigFactroyService (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    // Returns 'true' if registration was successful
    bool		registerTwig (const std::string &name, CreateTwigCallback createFn);
    
    // Returns 'true' if the Twig name was registered before
    bool 		unregisterTwig (const std::string &name);
    VisQueuedTwig *	createTwig (const std::string &name, 
				    IgState *state, IgTwig *parent,
				    const std::string &friendlyName,
				    const std::string &moduleLabel,
				    const std::string &instanceName,
				    const std::string &processName);
    
private:
    typedef std::map<const std::string, CreateTwigCallback> CallbackMap;

    IgState			*m_state;
    CallbackMap			m_callback;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_TWIG_FACTROY_SERVICE_H
