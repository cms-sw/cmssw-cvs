#ifndef VIS_TRIGGER_VIS_L1MU_DTCHAMB_PH_CONTAINER_TWIG_H
# define VIS_TRIGGER_VIS_L1MU_DTCHAMB_PH_CONTAINER_TWIG_H

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include <qstring.h>
# include <vector>

class DTGeometry;

class VisL1MuDTChambPhContainerTwig : public VisQueuedTwig
{
public:
  VisL1MuDTChambPhContainerTwig (IgState *state, IgTwig *parent, 
				 const std::string &name,
				 const std::string &friendlyName = "",
				 const std::string &moduleLabel = "",
				 const std::string &instanceName = "",
				 const std::string &processName = "");

  virtual void	onNewEvent (const edm::Event &event,
			    const edm::EventSetup &eventSetup);
  using VisQueuedTwig::update;
  virtual void       	update (Ig3DRep *rep);
  virtual void       	update (IgTextRep *rep);

private:    
  IgState              *m_state;
  std::string	       	m_text;
  std::string	 	m_friendlyName;
  std::string	 	m_moduleLabel;
  std::string	 	m_instanceName;
  std::string	 	m_processName;
  std::vector<L1MuDTChambPhDigi>  *m_phiContainer;
  edm::ESHandle<DTGeometry> m_muonGeom;
  QString		m_decision;
};

#endif // VIS_TRIGGER_VIS_L1MU_DTCHAMB_PH_CONTAINER_TWIG_H
