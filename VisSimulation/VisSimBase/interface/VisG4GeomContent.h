#ifndef VIS_SIM_BASE_VIS_G4GEOM_CONTENT_H
# define VIS_SIM_BASE_VIS_G4GEOM_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

/** Data proxy to create the event twig.  */
class VisG4GeomContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisG4GeomContent);
public:
  VisG4GeomContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisG4GeomContent::state (void) const
{ return m_state; }

#endif // VIS_SIM_BASE_VIS_G4GEOM_CONTENT_H
