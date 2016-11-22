#ifndef VIS_SIM_BASE_VIS_G4MAG_FIELD_CONTENT_H
# define VIS_SIM_BASE_VIS_G4MAG_FIELD_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

/** Data proxy to create the magnetic field twig.  */
class VisG4MagFieldContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisG4MagFieldContent);
public:
  VisG4MagFieldContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisG4MagFieldContent::state (void) const
{ return m_state; }

#endif // VIS_SIM_BASE_VIS_G4MAG_FIELD_CONTENT_H
