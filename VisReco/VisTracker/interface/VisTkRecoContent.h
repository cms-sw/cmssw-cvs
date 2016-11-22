#ifndef VIS_TRACKER_VIS_TK_RECO_CONTENT_H
# define VIS_TRACKER_VIS_TK_RECO_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisTkRecoContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisTkRecoContent);
public:
  VisTkRecoContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisTkRecoContent::state (void) const
{ return m_state; }

#endif // VIS_TRACKER_VIS_TK_RECO_CONTENT_H
