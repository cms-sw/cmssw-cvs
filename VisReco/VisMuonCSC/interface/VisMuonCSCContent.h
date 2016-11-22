#ifndef VIS_MUON_CSC_VIS_MUON_CSC_CONTENT_H
# define VIS_MUON_CSC_VIS_MUON_CSC_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisMuonCSCContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisMuonCSCContent);
public:
  VisMuonCSCContent (IgState *state);

  protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisMuonCSCContent::state (void) const
{ return m_state; }

#endif // VIS_MUON_CSC_VIS_MUON_CSC_CONTENT_H
