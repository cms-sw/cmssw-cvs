#ifndef VIS_SIM_HITS_VIS_SIM_HITS_CONTENT_H
# define VIS_SIM_HITS_VIS_SIM_HITS_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisSimHitsContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisSimHitsContent);
public:
  VisSimHitsContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisSimHitsContent::state (void) const
{ return m_state; }

#endif // VIS_SIM_HITS_VIS_SIM_HITS_CONTENT_H
