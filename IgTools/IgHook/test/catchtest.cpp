//<<<<<< INCLUDES                                                       >>>>>>

#include "IgTools/IgHook/interface/IgHook.h"
#include "IgTools/IgHook/interface/IgHookTrace.h"
#include <iostream>
#include <typeinfo>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

static void throwhook (void *exception, std::type_info *tinfo, void (*dest) (void *));

static IgHook::TypedData<void (void *, std::type_info *, void (*) (void *))> hook
= { { 0, "__cxa_throw", 0, 0, &throwhook, 0, 0, 0 } };

static void throwhook (void *exception, std::type_info *tinfo, void (*dest) (void *))
{
    void *trace [128];
    int levels = IgHookTrace::stacktrace (trace, 128);

    // Removing the __extension__ gives a warning which
    // is acknowledged as a language problem in the C++ Standard Core 
    // Language Defect Report
    //
    // http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#195
    //
    // since the suggested decision seems to be that the syntax should
    // actually be "Conditionally-Supported Behavior" in some 
    // future C++ standard I simply silence the warning.
    std::cerr << "trapped throw ("
	      << "" << exception
	      << ", " << tinfo->name ()
	      << ", " << __extension__ (void *) dest << ")\n";

    for (int i = 0; i < levels; ++i)
    {
	const char	*sym = 0;
	const char	*lib = 0;
	int		offset = 0;
	int		liboffset = 0;
	bool		nonheap = IgHookTrace::symbol (trace [i], sym, lib, offset, liboffset);

	std::cerr << "  " << trace [i] << " " << sym << " + " << offset
		  << " [" << (lib ? lib : "<unknown>") << " + " << liboffset << "]\n";

	if (! nonheap) delete [] sym;
    }

    hook.typed.chain (exception, tinfo, dest);
}

void testit (int n)
{
    if (n)
	testit (n-1);
    else
	throw std::string ("foo!");
}

int main (int, char **)
{
    IgHook::hook (hook.raw);
    try {
        testit (10);
    } catch (std::string x) {
	std::cerr << "caught " << x << "\n";
    }
    return 0;
}
