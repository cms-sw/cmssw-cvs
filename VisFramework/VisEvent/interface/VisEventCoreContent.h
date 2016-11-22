#ifndef VIS_EVENT_VIS_EVENT_CORE_CONTENT_H
# define VIS_EVENT_VIS_EVENT_CORE_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisEventCoreContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisEventCoreContent);
public:
  VisEventCoreContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisEventCoreContent::state (void) const
{ return m_state; }

#endif // VIS_EVENT_VIS_EVENT_CORE_CONTENT_H
