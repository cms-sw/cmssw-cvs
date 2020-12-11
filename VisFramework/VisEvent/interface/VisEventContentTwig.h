#ifndef VIS_EVENT_VIS_EVENT_CONTENT_TWIG_H
# define VIS_EVENT_VIS_EVENT_CONTENT_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

namespace edm 
{
    class Event;
    class EventID;    
    class EventSetup;
    class Timestamp;
}


//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisEventContentTwig : public IgCompoundTwig
{
public:
    VisEventContentTwig (IgState             *state,
			 IgTwig              *parent,
			 const std::string   name,
			 bool                traverse = false,
			 bool                visible = false,
			 bool                open = false);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual std::string	name (void) const;
    virtual void 	name (const std::string& value);
    virtual bool        expand (void);
    virtual bool 	selfTraverse (void) const;
    virtual void 	selfTraverse (bool value);
    virtual bool 	selfVisible (void) const;
    virtual void 	selfVisible (bool value);
    virtual bool 	cascade (void) const;
    virtual void	cascade (bool value);
    
    virtual void 	onPreEvent (const edm::EventID &eventID, const edm::Timestamp &timeStamp);
    virtual void       	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);

    IgState *		state (void);

private:
    static const unsigned int   TRAVERSE = 2;
    static const unsigned int   VISIBLE = 4;
    static const unsigned int   CASCADE = 8;
    
    IgState		*m_state;		//< State context.
    const std::string	m_name;
    std::string		m_text;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline IgState *
VisEventContentTwig::state (void)
{ return m_state; }

#endif // VIS_EVENT_VIS_EVENT_CONTENT_TWIG_H
