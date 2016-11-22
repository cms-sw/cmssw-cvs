#ifndef VIS_G4_CORE_VIS_G4_PATH_H
# define VIS_G4_CORE_VIS_G4_PATH_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Core/interface/config.h"
# include <classlib/utils/DebugAids.h>
# include <vector>
# include <utility>
# include <iosfwd>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisG4Path;
class logstream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

std::ostream &operator<< (std::ostream &out, const VisG4Path &path);
logstream   &operator<< (logstream &out, const VisG4Path &path);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Describe a Geant4 volume path including replica information.  */
class VIS_G4_CORE_API VisG4Path
{
    class Node
    {
    public:
	Node (void);
	Node (Node *parent, short daughter, short replica);
	~Node (void);

	void		ref (void);
	void		unref (void);

	short		daughter (void) const;
	short		replica (void) const;

	Node *		parent (void) const;
	Node *		get (short daughter, short replica);
	static Node *	root (void);

    private:
	Node		*m_parent;
	Node		*m_sibling;
	Node		*m_children;
	unsigned	m_refs;
	short		m_daughter;
	short		m_replica;

	static Node	*s_root;
    };

public:
    VisG4Path (void);
    VisG4Path (short daughter, short replica);
    VisG4Path (const VisG4Path &prefix, short daughter, short replica);
    VisG4Path (const VisG4Path &x);
    VisG4Path &operator= (const VisG4Path &x);
    ~VisG4Path (void);

    VisG4Path		prefix (void) const;
    bool		empty (void) const;
    short		daughter (void) const;
    short		replica (void) const;

    bool		operator== (const VisG4Path &path) const;
    bool		operator!= (const VisG4Path &path) const;
    bool		operator< (const VisG4Path &path) const;
    bool		operator<= (const VisG4Path &path) const;
    bool		operator> (const VisG4Path &path) const;
    bool		operator>= (const VisG4Path &path) const;

private:
    VisG4Path (Node *node);

    Node		*m_node;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
VisG4Path::Node::Node (void)
    : m_parent (this),
      m_sibling (0),
      m_children (0),
      m_refs (1),
      m_daughter (-1),
      m_replica (-1)
{}

inline
VisG4Path::Node::Node (Node *parent, short daughter, short replica)
    : m_parent (parent),
      m_sibling (parent->m_children),
      m_children (0),
      m_refs (0),
      m_daughter (daughter),
      m_replica (replica)
{ m_parent->m_refs++; m_parent->m_children = this; }

inline
VisG4Path::Node::~Node (void)
{
    if (m_parent->m_children == this)
	m_parent->m_children = m_sibling;
    else
    {
	Node *prev = m_parent->m_children;
	while (prev->m_sibling != this)
	    prev = prev->m_sibling;

	prev->m_sibling = m_sibling;
    }

    if (--m_parent->m_refs == 0)
	delete m_parent;
}

inline void
VisG4Path::Node::ref (void)
{ m_refs++; }

inline void
VisG4Path::Node::unref (void)
{
    if (--m_refs == 0)
	delete this;
}

inline short
VisG4Path::Node::daughter (void) const
{ return m_daughter; }

inline short
VisG4Path::Node::replica (void) const
{ return m_replica; }

inline VisG4Path::Node *
VisG4Path::Node::parent (void) const
{ return m_parent; }

inline VisG4Path::Node *
VisG4Path::Node::get (short daughter, short replica)
{
    Node *n = m_children;
    while (n && ! (n->m_daughter == daughter && n->m_replica == replica))
	n = n->m_sibling;

    if (! n)
	n = new Node (this, daughter, replica);

    return n;
}

inline VisG4Path::Node *
VisG4Path::Node::root (void)
{ return s_root; }

//////////////////////////////////////////////////////////////////////
/// Construct an empty path.
inline
VisG4Path::VisG4Path (void)
    : m_node (Node::root ())
{ m_node->ref (); }

/// Construct a path from a suffix.
inline
VisG4Path::VisG4Path (short daughter, short replica)
    : m_node (Node::root ()->get (daughter, replica))
{ m_node->ref (); }

/// Construct a path from a prefix and a suffix.
inline
VisG4Path::VisG4Path (const VisG4Path &prefix, short daughter, short replica)
    : m_node (prefix.m_node->get (daughter, replica))
{ m_node->ref (); }

/// Construct a path from a specific node.
inline
VisG4Path::VisG4Path (Node *node)
    : m_node (node)
{ m_node->ref (); }

/// Copy a path.
inline
VisG4Path::VisG4Path (const VisG4Path &x)
    : m_node (x.m_node)
{ m_node->ref (); }

/// Copy a path.
inline VisG4Path &
VisG4Path::operator= (const VisG4Path &x)
{ x.m_node->ref (); m_node->unref (); m_node = x.m_node; return *this; }

/// Destruct the path.
inline
VisG4Path::~VisG4Path (void)
{ m_node->unref (); }

/// Return the path prefix of this one: all but the last daughter,
/// replica information.
inline VisG4Path
VisG4Path::prefix (void) const
{ return VisG4Path (m_node->parent ()); }

/// Check if this an empty path.
inline bool
VisG4Path::empty (void) const
{ return m_node == Node::root (); }

/// Return the daughter of the last element of this path.
inline short
VisG4Path::daughter (void) const
{ return m_node->daughter (); }

/// Return the replica of the last element of this path.
inline short
VisG4Path::replica (void) const
{ return m_node->replica (); }

/// Compare two paths for equality.
inline bool
VisG4Path::operator== (const VisG4Path &path) const
{ return m_node == path.m_node; }

/// Compare two paths for inequality.
inline bool
VisG4Path::operator!= (const VisG4Path &path) const
{ return m_node != path.m_node; }

/** Compare two paths for relative order.  The comparison is arbitrary
    but very efficient -- you cannot assume anything about the path
    attributes from their relative order, but can use this method to
    store the paths in a map or set for instance.  */
inline bool
VisG4Path::operator< (const VisG4Path &path) const
{ return m_node < path.m_node; }

/** Compare two paths for relative order.  The comparison is arbitrary
    but very efficient -- you cannot assume anything about the path
    attributes from their relative order, but can use this method to
    store the paths in a map or set for instance.  */
inline bool
VisG4Path::operator<= (const VisG4Path &path) const
{ return m_node <= path.m_node; }

/** Compare two paths for relative order.  The comparison is arbitrary
    but very efficient -- you cannot assume anything about the path
    attributes from their relative order, but can use this method to
    store the paths in a map or set for instance.  */
inline bool
VisG4Path::operator> (const VisG4Path &path) const
{ return m_node > path.m_node; }

/** Compare two paths for relative order.  The comparison is arbitrary
    but very efficient -- you cannot assume anything about the path
    attributes from their relative order, but can use this method to
    store the paths in a map or set for instance.  */
inline bool
VisG4Path::operator>= (const VisG4Path &path) const
{ return m_node >= path.m_node; }

#endif // VIS_G4_CORE_VIS_G4_PATH_H
