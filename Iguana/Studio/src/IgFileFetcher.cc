#include "Iguana/Studio/interface/IgFileFetcher.h"
#include "Iguana/Studio/interface/IgFileProxyDB.h"
#include "Iguana/Studio/interface/IgFileProxy.h"
#include <QUrl>

IG_DEFINE_STATE_ELEMENT (IgFileFetcher, "FIXME/File Fetcher");

IgFileFetcher::IgFileFetcher (IgState *state)
    : m_state (state)
{ ASSERT (m_state); m_state->put (s_key, this); }

IgFileFetcher::~IgFileFetcher (void)
{
    for (ProxyTable::iterator i = m_proxies.begin (); i != m_proxies.end (); ++i)
	delete i->second;
    
    m_state->detach (s_key);
}

IgFileProxy *
IgFileFetcher::getProxy (const std::string& protocol)
{
    IgFileProxy *&proxy = m_proxies [protocol];
    if (! proxy)
        proxy = IgFileProxyDB::get ()->create (protocol);
    
    return proxy;
}

std::string
IgFileFetcher::fetch (const std::string &file)
{ 
    QUrl url(QString::fromStdString (file));
    QString protocol (url.protocol ());
    IgFileProxy *proxy = getProxy (protocol.toStdString ());
    
    ASSERT (proxy); // FIXME: throw exception if non-existent
    return proxy->fetch (file);
}
