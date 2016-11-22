#ifndef VIS_RECO_JETS_VIS_JET_TAG_TWIG_H
# define VIS_RECO_JETS_VIS_JET_TAG_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "DataFormats/TrackReco/interface/TrackFwd.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

namespace reco 
{
    class CaloJet;
}

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisJetTagTwig : public VisQueuedTwig
{
public:
    VisJetTagTwig (IgState *state, IgTwig *parent, const std::string &name = "",
		   const std::string &friendlyName = "",
		   const std::string &moduleLabel = "",
		   const std::string &instanceName = "",
		   const std::string &processName = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void	       	onNewEvent (const edm::Event &event,
					    const edm::EventSetup &eventSetup);

    using VisQueuedTwig::update;
    virtual void        	update (IgTextRep *rep);
    virtual void       		update (Ig3DRep *rep);
    virtual void       		update (IgLegoRep *rep);
    virtual void       		update (IgRPhiRep *rep);
    virtual void       		update (IgRZRep *rep);

private:
    std::string			m_text;
    const std::string		m_friendlyName;
    const std::string		m_moduleLabel;
    const std::string		m_instanceName;
    const std::string		m_processName;
    std::vector<reco::CaloJet>  m_jetTags;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_RECO_JETS_VIS_JET_TAG_TWIG_H
