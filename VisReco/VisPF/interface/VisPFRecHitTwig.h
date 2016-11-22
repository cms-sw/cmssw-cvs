#ifndef VIS_RECO_VIS_PFRECHIT_TWIG_H
#define VIS_RECO_VIS_PFRECHIT_TWIG_H

#include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include <vector>


/**\class VisPFRecHitTwig
   \brief Twig class for PFRecHit visualization
   
   \todo Fix for HB in 3D (something wrong with Layer enum or detId?)

   \author T. McCauley
   \date   February 2008
*/

class VisPFRecHitTwig : public VisQueuedTwig
{
public:
    VisPFRecHitTwig(IgState* state, IgTwig* parent, 
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
 
    std::vector<reco::PFRecHit> m_EBrechits;
    std::vector<reco::PFRecHit> m_HBrechits;
    std::vector<reco::PFRecHit> m_EErechits;

    edm::ESHandle<CaloGeometry> m_pDD;
};

#endif 
