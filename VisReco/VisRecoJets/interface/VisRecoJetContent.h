#ifndef VIS_RECO_JETS_VIS_RECO_JET_CONTENT_H
# define VIS_RECO_JETS_VIS_RECO_JET_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisRecoJetContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisRecoJetContent);
public:
  VisRecoJetContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisRecoJetContent::state (void) const
{ return m_state; }

#endif // VIS_RECO_JETS_VIS_RECO_JET_CONTENT_H
