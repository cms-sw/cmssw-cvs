#ifndef VIS_FRAMEWORK_BASE_VIS_CONFIGURABLE_H
# define VIS_FRAMEWORK_BASE_VIS_CONFIGURABLE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisConfigurable
{
public:
    VisConfigurable (IgState *state) { m_state = state; }
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual ~VisConfigurable (void);

    virtual std::string get (void) const = 0;
    virtual void set (const std::string &input) = 0;

protected:
    void baseInit (const std::string &iname);

private: 
    VisConfigurable & operator= (const VisConfigurable &rh);
    IgState		*m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_CONFIGURABLE_H
