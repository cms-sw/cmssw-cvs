#ifndef IGUANA_STUDIO_IG_DOCUMENT_DATA_H
# define IGUANA_STUDIO_IG_DOCUMENT_DATA_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgDocument;
class IgSimpleTwig;
class IgTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgDocumentData : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgDocumentData);
public:
    IgDocumentData (IgState *state, IgDocument *owner);
    ~IgDocumentData (void);
    // implicit copy constructor
    // implicit assignment operator

    IgDocument *	document (void);
    IgSimpleTwig *	root (const std::string &name = "", 
			      bool create = false);
    void		root (IgSimpleTwig *twig, 
			     const std::string &rootName = "");
    IgSimpleTwig *	add (const std::string &name, 
			     const std::string &rootName = "");
    IgTwig *		find (const std::string &name,
			      const std::string &rootName = "");

    typedef std::map<std::string, IgSimpleTwig *> TwigMap;
    TwigMap::const_iterator 	begin (void);
    TwigMap::const_iterator	end (void);
    
private:
    IgState		*m_state;
    IgDocument		*m_document;
    TwigMap m_rootMap;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_DOCUMENT_DATA_H
