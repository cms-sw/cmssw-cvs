#ifndef VIS_MUON_VIS_REFIT_TRACK_TWIG_H
# define VIS_MUON_VIS_REFIT_TRACK_TWIG_H

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisPropagatorIn.h"
# include "VisFramework/VisFrameworkBase/interface/VisPropagatorOut.h"
# include "VisFramework/VisFrameworkBase/interface/VisPropagatorStep.h"
# include "DataFormats/TrackReco/interface/TrackFwd.h"
# include "FWCore/Framework/interface/Event.h"

class SoSeparator;

class VisRefitTrackTwig : public VisQueuedTwig
{
public:
  VisRefitTrackTwig (IgState *state, IgTwig *parent,
		     const std::string &name,
		     reco::TrackRef tracks);

  virtual void	onNewEvent (const edm::Event &event,
			    const edm::EventSetup &eventSetup);
  using VisQueuedTwig::update;
  virtual void	update (Ig3DRep *rep);
  virtual void  update (IgRPhiRep *rep);
  virtual void  update (IgRZRep *rep);
  virtual void  configChanged (void);

private:
  reco::TrackRef 	m_track;
  VisPropagatorIn	m_in;
  VisPropagatorOut	m_out;
  VisPropagatorStep	m_step;
  
  SoSeparator *m_track3DSep;
  SoSeparator *m_trackRPhiSep;
  SoSeparator *m_trackRZSep;
};

#endif // VIS_MUON_VIS_REFIT_TRACK_TWIG_H
