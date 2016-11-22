#ifndef VIS_TRIGGER_VIS_L1GLOBAL_TRIGGER_READOUT_RECORD_TWIG_H
# define VIS_TRIGGER_VIS_L1GLOBAL_TRIGGER_READOUT_RECORD_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisL1GlobalTriggerReadoutRecordTwig : public VisQueuedTwig
{
public:
    VisL1GlobalTriggerReadoutRecordTwig (IgState *state, IgTwig *parent, 
					 const std::string &name,
					 const std::string &friendlyName = "",
					 const std::string &moduleLabel = "",
					 const std::string &instanceName = "",
					 const std::string &processName = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);
    using VisQueuedTwig::update;
    virtual void       	update (Ig3DRep *rep);
    virtual void       	update (IgRPhiRep *rep);
    virtual void       	update (IgRZRep *rep);
    virtual void       	update (IgTextRep *rep);

private:    
    IgState             *m_state;
    std::string	       	m_text;
    std::string	 	m_friendlyName;
    std::string	 	m_moduleLabel;
    std::string	 	m_instanceName;
    std::string	 	m_processName;
    std::vector<bool>	m_decision;
    std::vector<unsigned int> 	m_triggerMaskAlgoTrig;
    std::vector<unsigned int>	m_triggerMaskTechTrig;
    bool		m_passed;
    std::vector<std::pair<std::string, bool> > m_algos;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_TRIGGER_VIS_L1GLOBAL_TRIGGER_READOUT_RECORD_TWIG_H
