#ifndef VIS_SIM_BASE_VIS_SIM_CONTENT_H
# define VIS_SIM_BASE_VIS_SIM_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

/** Data proxy to initialise Geant4.  */
class VisSimContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisSimContent);
public:
  VisSimContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisSimContent::state (void) const
{ return m_state; }

#endif // VIS_SIM_BASE_VIS_SIM_CONTENT_H
