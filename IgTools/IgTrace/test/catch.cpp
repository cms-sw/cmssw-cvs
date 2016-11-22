//<<<<<< INCLUDES                                                       >>>>>>

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

void testit (int n)
{
    if (n)
	testit (n-1);
    else
	throw std::string ("foo!");
}

int main (int argc, char **argv)
{
    int level = 10;
    if (argc > 1)
	level = atoi (argv[1]);

    try {
        testit (level);
    } catch (std::string x) {
	std::cerr << "caught " << x << "\n";
    }
    return 0;
}
