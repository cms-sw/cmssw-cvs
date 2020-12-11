#ifndef VIS_RECO_VIS_PF_CONTENT_H
#define VIS_RECO_VIS_PF_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisPFContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT(VisPFContent);
public:
  VisPFContent(IgState *state);
  
protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisPFContent::state (void) const
{ return m_state; }

#endif
