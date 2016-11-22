#ifndef VIS_TRACKER_VIS_TRACKER_PICLUSTER_TWIG_H
# define VIS_TRACKER_VIS_TRACKER_PICLUSTER_TWIG_H

# include "Iguana/Framework/interface/IgTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
# include <vector>

class SiPixelCluster;
class DetId;

class VisTrackerPiClusterTwig : public VisQueuedTwig
{
public:
  VisTrackerPiClusterTwig (IgState *state, IgTwig *parent, 
			   const std::string &name = "",
			   const std::string &friendlyName = "",
			   const std::string &moduleLabel = "",
			   const std::string &instanceName = "",
			   const std::string &processName = "");
  
  virtual void	onNewEvent (const edm::Event &event,
			    const edm::EventSetup &eventSetup);
  using VisQueuedTwig::update;
  virtual void	update (Ig3DRep *rep);
  virtual void  update (IgLegoRep *rep);
  virtual void  update (IgTextRep *rep);
  virtual void  update (IgRPhiRep *rep);
  virtual void  update (IgRZRep *rep);
  
private:
  std::string 	     m_text;
  const std::string  m_friendlyName;
  const std::string  m_moduleLabel;
  const std::string  m_instanceName;
  const std::string  m_processName;
  const edm::ParameterSet *m_pset;
  double 	     m_clusterPointSize;
    
  std::vector<std::pair<DetId, SiPixelCluster> > p_cluster;
};

#endif 
