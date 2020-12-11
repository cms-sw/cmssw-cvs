#ifndef VIS_RECO_VIS_PFTRACK_TWIG_H
#define VIS_RECO_VIS_PFTRACK_TWIG_H

#include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrackFwd.h"

#include <vector>
#include <utility>

//class SbVec3f; forward declaration doesn't work!?
#include <Inventor/SbVec3f.h>
#include <Inventor/SbColor.h>

/**\class VisPFRecTrackTwig
   \brief Twig class for PFRecTrack visualization

   \author T. McCauley
   \date   February 2008
*/

class VisPFRecTrackTwig : public VisQueuedTwig
{
public:
    VisPFRecTrackTwig(IgState* state, IgTwig* parent, 
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

    std::vector<reco::PFRecTrack> m_tracks;
    
    std::vector<std::pair<SbColor, std::vector<SbVec3f> > >  m_trajectoryPositions;
    std::vector<std::pair<SbColor, std::vector<SbVec3f> > >  m_trajectoryDirections;

    //std::vector<std::vector<SbVec3f> >  m_bremTrajectoryPositions;
    //std::vector<std::vector<SbVec3f> >  m_bremTrajectoryDirections;
};

#endif 
