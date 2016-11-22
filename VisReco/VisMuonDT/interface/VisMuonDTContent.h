#ifndef VIS_MUON_DT_VIS_MUON_DT_CONTENT_H
# define VIS_MUON_DT_VIS_MUON_DT_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisMuonDTContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisMuonDTContent);
public:
  VisMuonDTContent (IgState *state);
  
protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisMuonDTContent::state (void) const
{ return m_state; }

#endif // VIS_MUON_DT_VIS_MUON_DT_CONTENT_H
