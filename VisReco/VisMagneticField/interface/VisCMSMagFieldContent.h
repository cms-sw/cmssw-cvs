#ifndef VIS_MAGNETIC_FIELD_VIS_CMSMAG_FIELD_CONTENT_H
# define VIS_MAGNETIC_FIELD_VIS_CMSMAG_FIELD_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

/** Data proxy to create the CMS magnetic field twig.  */
class VisCMSMagFieldContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisCMSMagFieldContent);
public:
  VisCMSMagFieldContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisCMSMagFieldContent::state (void) const
{ return m_state; }

#endif // VIS_MAGNETIC_FIELD_VIS_CMSMAG_FIELD_CONTENT_H
