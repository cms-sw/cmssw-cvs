#ifndef VIS_RECO_VIS_PFCLUSTER_TWIG_H
#define VIS_RECO_VIS_PFCLUSTER_TWIG_H

#include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include <vector>

/**\class VisPFClusterTwig
   \brief Twig class for PFCluster visualization
   
   \todo Support for PS?
   \todo HB: same problem as w/ PFRecHit; HCAL det id?
   \todo It appears that I have to cache the PFRecHitFraction information (something with copy constructor?)

   \author T. McCauley
   \date   February 2008
*/


class PFCluster;

class VisPFClusterTwig : public VisQueuedTwig
{
public:
    VisPFClusterTwig(IgState* state, IgTwig* parent, 
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

    struct RecHit
    {
	double fraction;
	double energy;
	unsigned detId;
    };
    
    std::string m_text;

    const std::string m_name;
    const std::string m_friendlyName;
    const std::string m_moduleLabel;
    const std::string m_instanceName;
    const std::string m_processName;

    std::vector<reco::PFCluster> m_EBclusters;
    std::vector<reco::PFCluster> m_HBclusters;
    std::vector<reco::PFCluster> m_EEclusters;

    std::vector<RecHit> m_EBrechits;
    std::vector<RecHit> m_HBrechits;
    std::vector<RecHit> m_EErechits;
    
    edm::ESHandle<CaloGeometry> m_pDD;

};	


#endif
