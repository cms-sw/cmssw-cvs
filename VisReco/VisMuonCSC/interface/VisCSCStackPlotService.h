#ifndef VIS_CSC_STACK_PLOT_SERVICE_H
#define VIS_CSC_STACK_PLOT_SERVICE_H

#include "Iguana/Framework/interface/IgStateElement.h"

class VisCSCStackPlots;
class QWidget;

class VisCSCStackPlotService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (VisCSCStackPlotService);
public:
    VisCSCStackPlotService (IgState *state, QWidget *w=0);
    ~VisCSCStackPlotService (void);
    VisCSCStackPlots * getPlots (void);
    
private:
    IgState *m_state;
    QWidget *m_parent;
    VisCSCStackPlots * m_plots;
    
};

#endif // VIS_CSC_STACK_PLOT_SERVICE_H
