#ifndef IG_HOOK_IG_HOOK_LIVE_MAP_H
# define IG_HOOK_IG_HOOK_LIVE_MAP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "IgTools/IgHook/interface/IgHookAlloc.h"
# if __GNUC__ && __GNUC__ >= 3
#  include <ext/hash_map>
#  define IG_HOOK_MAP __gnu_cxx::hash_map
#  define IG_HOOK_MAP_COMP __gnu_cxx::hash<Resource>, std::equal_to<Resource>
# else
#  include <map>
#  define IG_HOOK_MAP std::map
#  define IG_HOOK_MAP_COMP std::less<Resource>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgHookTrace;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgHookLiveMap
{
public:
    // Stuff we track
    typedef unsigned long				Resource;
    typedef std::pair<IgHookTrace *, Resource>		Element;
    typedef std::pair<IgHookTrace * const, Resource>	MapElement;
    typedef IG_HOOK_MAP< Resource, Element,
    		         IG_HOOK_MAP_COMP,
		         IgHookAlloc<MapElement> >	Map;
    typedef Map::iterator				Iterator;

    // Class methods
    // implicit constructor
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    unsigned long	size (void);
    Iterator		begin (void);
    Iterator		end (void);
    Iterator		find (Resource x);
    IgHookTrace *	owner (Resource x);
    void		insert (Resource x, Element element);
    void		insert (Resource x, IgHookTrace *owner, Resource extra = 0);
    void		remove (Iterator pos);
    void		remove (Resource x);

private:
    Map			m_live;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_HOOK_IG_HOOK_LIVE_MAP_H
