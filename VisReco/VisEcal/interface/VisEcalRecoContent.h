#ifndef VIS_ECAL_VIS_ECAL_RECO_CONTENT_H
# define VIS_ECAL_VIS_ECAL_RECO_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisEcalRecoContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisEcalRecoContent);
public:
  VisEcalRecoContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisEcalRecoContent::state (void) const
{ return m_state; }

#endif // VIS_ECAL_VIS_ECAL_RECO_CONTENT_H
