#ifndef VIS_MUON_VIS_MUON_CONTENT_H
# define VIS_MUON_VIS_MUON_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisMuonContent : public IgExtension
{
    IG_DECLARE_STATE_ELEMENT (VisMuonContent);
public:
    VisMuonContent (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisMuonContent::state (void) const
{ return m_state; }

#endif // VIS_MUON_VIS_MUON_CONTENT_H
