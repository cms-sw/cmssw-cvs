#ifndef VIS_MET_RECO_VIS_METRECO_CONTENT_H
# define VIS_MET_RECO_VIS_METRECO_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisMETRecoContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisMETRecoContent);
public:
  VisMETRecoContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisMETRecoContent::state (void) const
{ return m_state; }

#endif // VIS_MET_RECO_VIS_METRECO_CONTENT_H
