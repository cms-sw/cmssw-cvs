#include "VisReco/VisMuonCSC/interface/VisCSCChamberSelectorImplService.h"
#include "VisReco/VisMuonCSC/interface/VisCSCChamberSelectorImpl.h"

IG_DEFINE_STATE_ELEMENT (VisCSCChamberSelectorImplService,
"Services/VisReco/VisMuonCSC/ChamberSelector");

VisCSCChamberSelectorImplService::VisCSCChamberSelectorImplService (IgState
*state, QWidget *parent) : m_state(state)
{
    ASSERT (state);
    state->put (s_key, this);
    m_selector = new VisCSCChamberSelectorImpl(parent);
}
VisCSCChamberSelectorImplService::~VisCSCChamberSelectorImplService(void)
{
    ASSERT (m_state);
    m_state->detach (s_key);
    delete m_selector;
}   
VisCSCChamberSelectorImpl* VisCSCChamberSelectorImplService::selector(void)
{
    return m_selector;
}
