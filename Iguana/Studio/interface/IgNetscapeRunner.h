#ifndef IG_NETSCAPE_RUNNER_H
#define IG_NETSCAPE_RUNNER_H

#include "Iguana/Studio/interface/config.h"
#include <qobject.h>
#include <stdlib.h>

class QString;

class IGUANA_STUDIO_API IgNetscapeRunner : public QObject
{
    Q_OBJECT

public:

    IgNetscapeRunner( QObject* parent = 0, const char* name = 0 )
	: QObject( parent, name ) {};

public slots:

    void netscape( const QString & url );
//  	{
//          system( "netscape -remote 'openURL(http://iguana.web.cern.ch/iguana/)'"
//  	        " || netscape 'http://iguana.web.cern.ch/iguana/'"); }
};
#endif //IG_NETSCAPE_RUNNER_H
