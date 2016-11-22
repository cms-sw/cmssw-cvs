#ifndef VIS_FRAMEWORK_BASE_CORE_CONTENT_H
# define VIS_FRAMEWORK_BASE_CORE_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

/** Content proxy to create framework visualisation core.  */
class VisCoreContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisCoreContent);
public:
  VisCoreContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisCoreContent::state (void) const
{ return m_state; }

#endif // VIS_FRAMEWORK_BASE_CORE_CONTENT_H
