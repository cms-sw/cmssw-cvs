#ifndef VIS_SIM_DATA_VIS_SIM_DATA_CONTENT_H
# define VIS_SIM_DATA_VIS_SIM_DATA_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisSimDataContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisSimDataContent);
public:
  VisSimDataContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisSimDataContent::state (void) const
{ return m_state; }

#endif // VIS_SIM_DATA_VIS_SIM_DATA_CONTENT_H
