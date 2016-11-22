//<<<<<< INCLUDES                                                       >>>>>>

#include "IgTools/IgHook/interface/IgHookTrace.h"
#include "IgTools/IgHook/interface/IgHook.h"
#include <iostream>
#include <cstdlib>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

#define TRACE_TEST(fun,next) void fun (void) { if (rand () % 2) traceme (); next (); }

IgHookTrace *root = 0;
IgHookTrace::Counter ticks = { "ticks" };

void dumptrace (IgHookTrace *node, int depth)
{
    if (node->address ())
    { 
	for (int i = 0; i < depth-1; ++i)
	    std::cout << ' ';
    
	const char	*sym;
	const char	*lib;
	int		offset;
	int		liboffset;
	bool		nonheap = node->symbol (sym, lib, offset, liboffset);

	std::cout << '(' << node->address () << "+" << offset << ", "
		  << sym << " [" << (lib ? lib : "<unknown>") << " + " << liboffset << "]): "
		  << ticks.m_name << "=" << node->counter (&ticks)->value () << "\n";

	if (! nonheap) delete [] sym;
    }

    for (IgHookTrace *kid = node->children (); kid; kid = kid->next ())
	dumptrace (kid, depth+1);
}

void traceme (void)
{
    void *addresses [128];
    int depth = IgHookTrace::stacktrace (addresses, 128);
    int drop = 2; // one for stacktrace, one for me

    if (! root)
	root = new IgHookTrace;

    IgHookTrace *node = root;
    for (int i = depth-2; i >= drop; --i)
	node = node->child (IgHookTrace::tosymbol (addresses [i]));

    node->counter (&ticks)->tick ();
}

TRACE_TEST(a,traceme)
TRACE_TEST(b,a)
TRACE_TEST(c,b)
TRACE_TEST(d,c)
TRACE_TEST(e,d)
TRACE_TEST(f,e)

int main (int, char **)
{
    traceme ();
    for (int i = 0; i < 10; ++i)
	f ();

    dumptrace (root, 0);
    return 0;
}
