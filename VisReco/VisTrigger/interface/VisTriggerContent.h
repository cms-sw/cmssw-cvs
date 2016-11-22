#ifndef VIS_TRIGGER_VIS_TRIGGER_CONTENT_H
# define VIS_TRIGGER_VIS_TRIGGER_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisTriggerContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisTriggerContent);
public:
  VisTriggerContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisTriggerContent::state (void) const
{ return m_state; }

#endif // VIS_TRIGGER_VIS_TRIGGER_CONTENT_H
