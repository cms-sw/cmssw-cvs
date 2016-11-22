#include "VisReco/VisMuonCSC/interface/VisCSCStripDigiTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCChamberSelectorImpl.h"
#include <qwidget.h>
#include <qstring.h>
#include <qspinbox.h>
#include <qradiobutton.h>
#undef emit /* qt problem... */
#undef signal /* ditto */

VisCSCChamberSelectorImpl::VisCSCChamberSelectorImpl(  QWidget* parent) :
VisCSCChamberSelector(parent), m_did(CSCDetId(0, 0, 0, 0))
{
    connect(ChamberSpin, SIGNAL(valueChanged(int)),this, SLOT(selectorInput(int)));
    connect(RingSpin, SIGNAL(valueChanged(int)),this, SLOT(selectorInput(int)));
    connect(StationSpin, SIGNAL(valueChanged(int)),this, SLOT(selectorInput(int)));
    connect(EndcapSpin, SIGNAL(valueChanged(int)),this, SLOT(selectorInput(int)));
    connect(ApplyFilter, SIGNAL(toggled(bool)),this, SLOT(filterChange(bool)));

}
VisCSCChamberSelectorImpl::~VisCSCChamberSelectorImpl()
{
}
void VisCSCChamberSelectorImpl::selectorInput(int)
{
    int chamber = ChamberSpin->value();
    int ring = RingSpin->value();
    int station = StationSpin->value();
    int endcap = EndcapSpin->value();
    m_did = CSCDetId(endcap, station, ring, chamber); 
    if (ApplyFilter->isChecked()) 
    {
	selectionChange( m_did() );
    }
         
}
void VisCSCChamberSelectorImpl::filterChange( bool )
{
    if (!ApplyFilter->isChecked()) 
    {
        CSCDetId id = CSCDetId(0, 0, 0, 0);
	selectionChange( id() );
    }
    else
    {
        selectionChange( m_did() );
    }	
}
