#ifndef VIS_EVENT_SETUP_VIS_EVENT_SETUP_CONTENT_H
# define VIS_EVENT_SETUP_VIS_EVENT_SETUP_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class IgState;
class IgTwig;

class VisEventSetupContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisEventSetupContent);
public:
  VisEventSetupContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void  	init (void);
  void		buildCalo (IgTwig *twig);
  void		buildCSCs (IgTwig *twig);
  void		buildDTs (IgTwig *twig);
  void		buildRPCs (IgTwig *twig);
  void		buildTracker (IgTwig *twig);
};

inline IgState *
VisEventSetupContent::state (void) const
{ return m_state; }

#endif // VIS_EVENT_SETUP_VIS_EVENT_SETUP_CONTENT_H
