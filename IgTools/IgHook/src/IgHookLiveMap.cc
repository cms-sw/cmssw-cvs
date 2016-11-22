//<<<<<< INCLUDES                                                       >>>>>>

#include "IgTools/IgHook/interface/IgHookLiveMap.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

unsigned long
IgHookLiveMap::size (void)
{ return m_live.size (); }

IgHookLiveMap::Iterator
IgHookLiveMap::begin (void)
{ return m_live.begin (); }

IgHookLiveMap::Iterator
IgHookLiveMap::end (void)
{ return m_live.end (); }

IgHookLiveMap::Iterator
IgHookLiveMap::find (Resource x)
{ return m_live.find (x); }

IgHookTrace *
IgHookLiveMap::owner (Resource x)
{ Iterator pos = find (x); return pos == end () ? 0 : pos->second.first; }

void
IgHookLiveMap::insert (Resource x, Element item)
{ m_live [x] = item; }

void
IgHookLiveMap::insert (Resource x, IgHookTrace *owner, Resource extra /* = 0 */)
{ m_live [x] = Element (owner, extra); }

void
IgHookLiveMap::remove (Resource x)
{ m_live.erase (x); }

void
IgHookLiveMap::remove (Iterator pos)
{ m_live.erase (pos); }
