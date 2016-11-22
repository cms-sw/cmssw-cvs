#ifndef VIS_FRAMEWORK_BASE_VIS_EVENT_SELECTOR_H
# define VIS_FRAMEWORK_BASE_VIS_EVENT_SELECTOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "FWCore/Framework/interface/SelectorBase.h"
# include "DataFormats/Provenance/interface/ConstBranchDescription.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisEventSelector : public edm::SelectorBase
{
public:
    VisEventSelector (const std::string &friendlyName, 
		      const std::string &moduleLabel,
		      const std::string &instanceName,
		      const std::string &processName) 
	: m_friendlyName (friendlyName),
	  m_moduleLabel (moduleLabel),
	  m_instanceName (instanceName),
	  m_processName (processName) {}
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual bool doMatch (const edm::ConstBranchDescription &p) const 
	{
	    return ((p.friendlyClassName() == m_friendlyName) && 
		    (p.moduleLabel() == m_moduleLabel) &&
		    (p.productInstanceName() == m_instanceName) &&
		    (p.processName () == m_processName));
	}
    
    virtual edm::SelectorBase *clone() const {return new VisEventSelector(*this);}

private:
    std::string 	m_friendlyName;
    std::string		m_moduleLabel;
    std::string		m_instanceName;
    std::string		m_processName;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_EVENT_SELECTOR_H
