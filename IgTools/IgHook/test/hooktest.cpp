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

double myreplacement (double x);

static IgHook::TypedData<double (double)> hook
= { { 0, "myfunc", 0, 0, &myreplacement, 0, 0, 0 } };

extern "C"  double myfunc (double x)
{
    return sin (x);
}

double myreplacement (double x)
{
    double result = (*hook.typed.chain) (x);
    std::cerr << "myreplacement(" << x << ") = " << result << "\n";
    return result;
}

double testit (void)
{
    double d = 0.;
    for (int i = 0; i < 360; ++i)
	d += myfunc (i / M_PI);
    
    return d;
}

int main (int, char **)
{
    IgHook::hook (hook.raw);
    testit ();
    return 0;
}
