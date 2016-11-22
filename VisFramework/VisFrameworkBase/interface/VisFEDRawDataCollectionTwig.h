#ifndef VIS_FRAMEWORK_BASE_VIS_FEDRAW_DATA_COLLECTION_TWIG_H
# define VIS_FRAMEWORK_BASE_VIS_FEDRAW_DATA_COLLECTION_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisFEDRawDataCollectionTwig : public VisQueuedTwig
{
public:
    VisFEDRawDataCollectionTwig (IgState *state, IgTwig *parent, 
				 const std::string &name,
				 const std::string &friendlyName = "",
				 const std::string &moduleLabel = "",
				 const std::string &instanceName = "",
				 const std::string &processName = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);
    using VisQueuedTwig::update;
    virtual void       	update (Ig3DRep *rep);
    virtual void       	update (IgLegoRep *rep);
    virtual void       	update (IgTextRep *rep);
    virtual void       	update (IgRPhiRep *rep);
    virtual void       	update (IgRZRep *rep);

private:    
    IgState             *m_state;
    std::string	       	m_text;
    std::string	 	m_friendlyName;
    std::string	 	m_moduleLabel;
    std::string	 	m_instanceName;
    std::string	 	m_processName;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_FEDRAW_DATA_COLLECTION_TWIG_H
