#ifndef VIS_FRAMEWORK_BASE_VIS_ACTIVE_CONFIGURABLE_H
# define VIS_FRAMEWORK_BASE_VIS_ACTIVE_CONFIGURABLE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisConfigurable.h"
# include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
# include "VisFramework/VisFrameworkBase/interface/debug.h"
# include "classlib/utils/Callback.h"
# include <boost/lexical_cast.hpp>
# include <string>
# include <sstream>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

template<class T>
class VisActiveConfigurable : VisConfigurable
{
public:
    typedef VisActiveConfigurable<T> self;

    VisActiveConfigurable (IgState *state, lat::Callback notify, const T &t, const std::string &name);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    T&		value () { return m_value;}
    const T& 	value () const { return m_value;}

protected:
    virtual void 	set (const std::string &input);
    virtual void 	set (const double value);
    virtual std::string get (void) const;
    
private:
    VisActiveConfigurable (void) {}

    T 			m_value;
    lat::Callback 	m_notify;
};

/// constructor from default value, name and configuration
template <class T>
inline
VisActiveConfigurable<T>::VisActiveConfigurable (IgState *state, lat::Callback notify, const T &t, const std::string &name)
    : VisConfigurable (state),
      m_value (t),
      m_notify (notify)
{
    LOG (0, trace, LFfwvis, "VisActiveConfigurable[" << name
       << "]\n");

    baseInit (name);

    const edm::ParameterSet *pset = VisConfigurationService::pSet ();
    if (pset)
    {
        T value = pset->getUntrackedParameter<T> (name, m_value);
	set (value);
    }
}

template <class T>
inline void
VisActiveConfigurable<T>::set (const std::string &input)
{ 
    std::istringstream in (input.c_str ());
    in >> m_value; 
    m_notify (); 
}

template <class T>
inline void
VisActiveConfigurable<T>::set (const double value)
{ 
    const std::string s = boost::lexical_cast<std::string>(value);
    set (s);
}

template<class T>
inline std::string VisActiveConfigurable<T>::get () const
{
    std::ostringstream out;
    out << m_value;
    return out.str ();
}

template<>
inline void VisActiveConfigurable<std::string>::set (const std::string & input) 
{
    m_value = input;
}

template<>
inline std::string VisActiveConfigurable<std::string>::get () const 
{
    return m_value;
}

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_ACTIVE_CONFIGURABLE_H
