#ifndef VIS_CSC_CHAMBER_SELECTOR_IMPL_H
#define VIS_CSC_CHAMBER_SELECTOR_IMPL_H

#include "VisReco/VisMuonCSC/interface/VisCSCChamberSelector.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"

class VisCSCStripDigiTwig;

/**
   \class VisCSCChamberSelectorImpl
   \brief Implementation of the (Qt Designer generated) VisCSCChamberSelector
   class. Gathers signals from the control widget and funnels them to the parent
   VisMuonCSCContent class.

   \date 2008-05-05
   \author G. Alverson, Northeastern University
**/



class VisCSCChamberSelectorImpl : public VisCSCChamberSelector
{
    Q_OBJECT
public:

    VisCSCChamberSelectorImpl(QWidget* parent = 0 );
    ~VisCSCChamberSelectorImpl();
    void setTwig(VisCSCStripDigiTwig* t);
signals:
    void selectionChange( uint32_t ); //! passes on new selection
public slots:
    void selectorInput( int );
    void filterChange( bool );

private:
    CSCDetId m_did;
};
#endif // VIS_CSC_CHAMBER_SELECTOR_IMPL_H
