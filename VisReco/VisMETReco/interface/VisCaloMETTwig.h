#ifndef VIS_MET_RECO_VIS_CALO_MET_TWIG_H
# define VIS_MET_RECO_VIS_CALO_MET_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "DataFormats/Common/interface/Handle.h"
# include "DataFormats/METReco/interface/CaloMETCollection.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCaloMETTwig : public VisQueuedTwig
{
public:
    VisCaloMETTwig (IgState *state, IgTwig *parent,
		    const std::string &name = "",
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
    virtual void	update (Ig3DRep *rep);
    virtual void       	update (IgLegoRep *rep);
    virtual void       	update (IgTextRep *rep);
    virtual void       	update (IgRPhiRep *rep);
    virtual void       	update (IgRZRep *rep);

private:
    std::string         	m_name;
    const std::string		m_friendlyName;
    const std::string		m_moduleLabel;
    const std::string		m_instanceName;
    const std::string		m_processName;
    std::string			m_text;
    std::vector<edm::Handle<reco::CaloMETCollection> > m_handles;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MET_RECO_VIS_CALO_MET_TWIG_H
