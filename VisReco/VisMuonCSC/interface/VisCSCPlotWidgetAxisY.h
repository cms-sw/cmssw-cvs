#ifndef PLOTWIDGETAXISY_H
#define PLOTWIDGETAXISY_H


#include "qwidget.h"
//Added by qt3to4:
#include <QPaintEvent>
#include "VisReco/VisMuonCSC/interface/VisCSCPlotWidget.h"

class VisCSCPlotWidgetAxisY :
	public QWidget
{
public:
	VisCSCPlotWidgetAxisY(VisCSCPlotWidget *pw, QWidget* parent = 0, const char* name = 0);
	~VisCSCPlotWidgetAxisY(void);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
protected:
	void paintEvent(QPaintEvent *);
private:
	VisCSCPlotWidget *m_pw;

};
#endif //PLOTWIDGETAXISY_H

