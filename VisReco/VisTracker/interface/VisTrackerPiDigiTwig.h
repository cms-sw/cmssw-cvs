#ifndef VIS_TRACKER_VIS_TRACKER_PIDIGI_TWIG_H
# define VIS_TRACKER_VIS_TRACKER_PIDIGI_TWIG_H

# include "Iguana/Framework/interface/IgTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include <vector>

class PixelDigi;
class DetId;

class VisTrackerPiDigiTwig : public VisQueuedTwig
{
public:
  VisTrackerPiDigiTwig (IgState *state, IgTwig *parent, 
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
  std::string	 	m_text;
  const std::string  	m_friendlyName;
  const std::string  	m_moduleLabel;
  const std::string  	m_instanceName;
  const std::string 	m_processName;
    
  std::vector<std::pair<DetId, PixelDigi> > m_digis;
};

#endif // VIS_TRACKER_VIS_TRACKER_PIDIGI_TWIG_H
