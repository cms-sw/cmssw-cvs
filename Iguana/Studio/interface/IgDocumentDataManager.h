#ifndef IGUANA_STUDIO_IG_DOCUMENT_DATA_MANAGER_H
# define IGUANA_STUDIO_IG_DOCUMENT_DATA_MANAGER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <map>
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgDocumentDataManager : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgDocumentDataManager);
public:
    IgDocumentDataManager (IgState *m_state);
    ~IgDocumentDataManager (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    typedef std::map<std::string, IgState*> DocDataMap;
    
    IgState*			state (void);
    IgState*			lookup (IgState* state = 0,
					std::string root = "");
    std::vector<std::string> 	roots (void);

private:
    IgState	*m_state;
    DocDataMap	m_docData;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_DOCUMENT_DATA_MANAGER_H
