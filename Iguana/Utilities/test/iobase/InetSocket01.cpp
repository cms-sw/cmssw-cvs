#include "test/Test.h" 
#include "classlib/iobase/InetSocket.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    InetSocket s;
    return 0;
}

