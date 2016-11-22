#ifndef VIS_MUON_RPC_VIS_RPC_CONTENT_H
# define VIS_MUON_RPC_VIS_RPC_CONTENT_H

# include "Iguana/Framework/interface/IgExtension.h"

class VisRPCContent : public IgExtension
{
  IG_DECLARE_STATE_ELEMENT (VisRPCContent);
public:
  VisRPCContent (IgState *state);

protected:
  IgState *    	state (void) const;

private:
  IgState     	*m_state;

  void	    	init (void);
};

inline IgState *
VisRPCContent::state (void) const
{ return m_state; }

#endif // VIS_MUON_RPC_VIS_RPC_CONTENT_H
