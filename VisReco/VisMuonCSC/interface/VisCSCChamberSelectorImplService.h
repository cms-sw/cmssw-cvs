#ifndef VIS_CSC_CHAMBER_SELECTOR_IMPL_SERVICE_H
#define VIS_CSC_CHAMBER_SELECTOR_IMPL_SERVICE_H

#include "Iguana/Framework/interface/IgStateElement.h"

class VisCSCChamberSelectorImpl;
class QWidget;

class VisCSCChamberSelectorImplService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (VisCSCChamberSelectorImplService);
public:
    VisCSCChamberSelectorImplService (IgState *state, QWidget *w=0);
    ~VisCSCChamberSelectorImplService (void);
    VisCSCChamberSelectorImpl * selector (void);
    
private:
    IgState *m_state;
    QWidget *m_parent;
    VisCSCChamberSelectorImpl * m_selector;
    
};

#endif // VIS_CSC_CHAMBER_SELECTOR_IMPL_SERVICE_H
