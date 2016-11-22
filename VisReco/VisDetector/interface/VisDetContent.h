#ifndef VIS_DETECTOR_VIS_DET_CONTENT_H
# define VIS_DETECTOR_VIS_DET_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

/** Data proxy to create the CMS detector twig.  */
class VisDetContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisDetContent);

public:
  VisDetContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisDetContent::state (void) const
{ return m_state; }

#endif // VIS_DETECTOR_VIS_DET_CONTENT_H
