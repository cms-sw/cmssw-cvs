#ifndef PLOTWIDGETAXISX_H
#define PLOTWIDGETAXISX_H
#include "qwidget.h"
//Added by qt3to4:
#include <QPaintEvent>

class VisCSCPlotWidgetAxisX :
	public QWidget
{
public:
	VisCSCPlotWidgetAxisX(const int nbins, QWidget* parent = 0, const char* name = 0);
	~VisCSCPlotWidgetAxisX(void);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
protected:
	void paintEvent(QPaintEvent *);
private:
	int m_bins;
};

#endif// PLOTWIDGETAXISX_H
