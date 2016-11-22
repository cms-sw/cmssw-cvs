#ifndef VISCSCCHAMBERSELECTOR_H
#define VISCSCCHAMBERSELECTOR_H

#include <QApplication>
#include <QWidget>

#include "VisReco/VisMuonCSC/interface/ui_VisCSCChamberSelector.h"

class VisCSCChamberSelector : public QWidget, public  Ui::VisCSCChamberSelector
{
    Q_OBJECT
    
public:
    VisCSCChamberSelector(QWidget *parent=0)
    	: QWidget(parent)
    {  setupUi(this); }
};

#endif // VISCSCCHAMBERSELECTOR_H
