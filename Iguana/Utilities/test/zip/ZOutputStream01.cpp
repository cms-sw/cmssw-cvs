#include "test/Test.h" 
#include "classlib/zip/ZOutputStream.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/iotools/StorageOutputStream.h"
#include "classlib/iotools/BufferOutputStream.h"
#include "classlib/utils/Signal.h"

using namespace lat;
int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    Filename		inputFile (Filename ("$SEAL/src/Foundation/classlib/zip/tests/BuildFile")
		        	   .substitute (ShellEnvironment ()));
    File		input (inputFile);
    File		output ("test.z", IOFlags::OpenWrite
				| IOFlags::OpenCreate | IOFlags::OpenTruncate );
    StorageOutputStream	soutput (&output);
    BufferOutputStream	boutput (&soutput);
    ZOutputStream	t (&boutput);
    char		buf [1024];
    IOSize		n;

    while ((n = input.read (buf, sizeof (buf))))
	t.xwrite (buf, n);

    t.close ();
    return 0;
}
