#ifndef VIS_HCAL_VIS_HCAL_RECO_CONTENT_H
# define VIS_HCAL_VIS_HCAL_RECO_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisHcalRecoContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisHcalRecoContent);
public:
  VisHcalRecoContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisHcalRecoContent::state (void) const
{ return m_state; }

#endif // VIS_HCAL_VIS_HCAL_RECO_CONTENT_H
