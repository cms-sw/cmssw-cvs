#ifndef VIS_FRAMEWORK_BASE_VIS_CONFIGURATION_H
# define VIS_FRAMEWORK_BASE_VIS_CONFIGURATION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgStateElement.h"
# include <map>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisConfigEditor;
class VisConfigurable;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisConfiguration : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (VisConfiguration);
public:
    static const int	MENU_ID_CONFIG	      = 8900;
    static const int	MENU_ID_CONFIG_CMSSW  = 8901;
    static const int	MENU_ID_CONFIG_SEP    = 8902;
    static const int	MENU_ID_CONFIG_SAVE   = 8903;

    typedef std::map<std::string, std::string, std::less<std::string> > VisState;
    typedef std::multimap<std::string, VisConfigurable*, std::less<std::string> > VisConfigMap;
    typedef VisConfigMap::const_iterator VisConfigMapConItr;
    typedef VisConfigMap::iterator VisConfigMapItr;
    typedef std::pair<VisConfigMapItr,VisConfigMapItr> SRANGE;

    VisConfiguration (IgState *state);
    virtual ~VisConfiguration (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
 
    void dumpConfig (VisState& state) const; 
    void insert (const std::string &iname, VisConfigurable *ihim);
    void erase (const VisConfigurable *ihim);
    void modify (const std::string & iname, const std::string & ivalue);
    void set (const std::string & iname, const std::string & ivalue);
    
protected:
    void clear (void);
    VisConfigMap::iterator find (const VisConfigurable *ihim, VisConfigMapItr p);

private: 
    IgState		       *m_state; 
    VisConfigEditor	       *m_editor;
    VisConfigMap 		m_registry;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_CONFIGURATION_H
