#ifndef VIS_RECO_VIS_TRACKINGPARTICLE_TWIG_H
#define VIS_RECO_VIS_TRACKINGPARTICLE_TWIG_H

#include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"

#include <utility>
#include <Inventor/SbVec3f.h>

/**\class VisTrackingParticle
   \brief Twig class for TrackingParticle visualization

   \author T. McCauley
   \date   February 2008
*/

class VisTrackingParticleTwig : public VisQueuedTwig
{
public:
    VisTrackingParticleTwig(IgState* state, IgTwig* parent, 
			    const std::string& name = "",
			    const std::string& friendlyName = "",
			    const std::string& moduleLabel = "",
			    const std::string& instanceName = "",
			    const std::string& processName = "");
    
    virtual void onNewEvent(const edm::Event& event,
			    const edm::EventSetup& eventSetup);

    using VisQueuedTwig::update;
    virtual void update(Ig3DRep* rep);
    virtual void update(IgLegoRep* rep);
    virtual void update(IgTextRep* rep);
    virtual void update(IgRPhiRep* rep);
    virtual void update(IgRZRep* rep);

private:
    std::string m_text;

    const std::string m_name;
    const std::string m_friendlyName;
    const std::string m_moduleLabel;
    const std::string m_instanceName;
    const std::string m_processName;

    std::vector<std::pair<TrackingParticle,SbVec3f> > m_tracks;
};

#endif 
