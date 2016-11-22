//<<<<<< INCLUDES                                                       >>>>>>

#include "IgTools/IgHook/interface/IgHook.h"
#include <iostream>
#include <cmath>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void myexit (int n);

static IgHook::TypedData<void (int)> hook
= { { 0, "exit", 0, 0, &myexit, 0, 0, 0 } };

void myexit (int n)
{
    std::cerr << "myexit(" << n << ")\n";
    (*hook.typed.chain) (n);
}

int main (int, char **)
{
    IgHook::hook (hook.raw);
    exit (0);
}
