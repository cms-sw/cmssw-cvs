#ifndef STUDIO_IG_FILE_FETCHER_H
# define STUDIO_IG_FILE_FETCHER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <string>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgFileProxy;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgFileFetcher: public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgFileFetcher);
public:
    IgFileFetcher (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~IgFileFetcher (void);
    
    std::string         fetch (const std::string &file);

private:
    typedef std::map<std::string, IgFileProxy *> ProxyTable;
    IgFileProxy *       getProxy (const std::string& protocol);
    
    IgState             *m_state;
    ProxyTable          m_proxies;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // STUDIO_IG_FILE_FETCHER_H
