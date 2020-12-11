#ifndef VIS_MUON_VIS_TRACK_TWIG_H
# define VIS_MUON_VIS_TRACK_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisAnimate.h"
# include "VisFramework/VisFrameworkBase/interface/VisCounterMax.h"
# include "VisFramework/VisFrameworkBase/interface/VisCounterMin.h"
# include "VisFramework/VisFrameworkBase/interface/VisCounterFrequency.h"
# include "VisFramework/VisFrameworkBase/interface/VisBField.h"
# include "VisFramework/VisFrameworkBase/interface/VisRMax.h"
# include "VisFramework/VisFrameworkBase/interface/VisZMax.h"
# include "VisFramework/VisFrameworkBase/interface/VisMuonT0.h"
# include "VisFramework/VisFrameworkBase/interface/VisMuonDt.h"
# include "VisFramework/VisFrameworkBase/interface/VisMuonAnnotation.h"
# include "DataFormats/TrackReco/interface/Track.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisTrackTwig : public VisQueuedTwig
{
public:
    VisTrackTwig (IgState *state, IgTwig *parent, 
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
    virtual void        configChanged (void);

private:    
    std::string	      	m_text;
    const std::string	m_friendlyName;
    const std::string	m_moduleLabel;
    const std::string	m_instanceName;
    const std::string	m_processName;
    VisAnimate          m_animate;
    VisCounterMax       m_counterMax;
    VisCounterMin       m_counterMin;
    VisCounterFrequency m_frequency;
    VisBField           m_BField;
    VisRMax             m_RMax;
    VisZMax             m_ZMax;
    VisMuonT0           m_t0;
    VisMuonDt           m_dt;
    VisMuonAnnotation	m_annot;

    std::vector<reco::Track>	m_tracks;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MUON_VIS_TRACK_TWIG_H
