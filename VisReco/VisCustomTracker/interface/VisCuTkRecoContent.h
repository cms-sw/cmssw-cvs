#ifndef VIS_CUTRACKER_VIS_TK_RECO_CONTENT_H
# define VIS_CUTRACKER_VIS_TK_RECO_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisCuTkRecoContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisCuTkRecoContent);
public:
  VisCuTkRecoContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisCuTkRecoContent::state (void) const
{ return m_state; }

#endif // VIS_CUTRACKER_VIS_TK_RECO_CONTENT_H
