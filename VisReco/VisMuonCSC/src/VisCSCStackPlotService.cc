#include "VisReco/VisMuonCSC/interface/VisCSCStackPlotService.h"
#include "VisReco/VisMuonCSC/interface/VisCSCStackPlots.h"

IG_DEFINE_STATE_ELEMENT (VisCSCStackPlotService,
"Services/VisReco/VisMuonCSC/StackPlots");

VisCSCStackPlotService::VisCSCStackPlotService (IgState
*state, QWidget *parent) : m_state(state)
{
    ASSERT (state);
    state->put (s_key, this);
    m_plots = new VisCSCStackPlots( state, 8, 96, parent, "CSC Strips");
}
VisCSCStackPlotService::~VisCSCStackPlotService(void)
{
    ASSERT (m_state);
    m_state->detach (s_key);
    delete m_plots;
}   
VisCSCStackPlots* VisCSCStackPlotService::getPlots(void)
{
    return m_plots;
}
