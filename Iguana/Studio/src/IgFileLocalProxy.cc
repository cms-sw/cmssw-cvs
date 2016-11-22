#include "Iguana/Studio/interface/IgFileLocalProxy.h"
#include <QUrl>
#include <classlib/iobase/Filename.h>

const char *
IgFileLocalProxy::catalogLabel (void)
{ return "file"; }

std::string
IgFileLocalProxy::fetch (const std::string &file)
{
    std::string filename (QUrl (QString::fromStdString (file)).path ().latin1 ());
    if (lat::Filename (filename).exists ())
        return filename;
    else
        // FIXME: throw an exception
	;
    return "";    
}
