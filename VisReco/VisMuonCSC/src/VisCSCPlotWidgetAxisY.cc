#include "VisReco/VisMuonCSC/interface/VisCSCPlotWidgetAxisY.h"
#include "QPainter"
#include <QPaintEvent>
#include <iostream>

VisCSCPlotWidgetAxisY::VisCSCPlotWidgetAxisY(VisCSCPlotWidget *pw, QWidget* parent, const char* name): 
QWidget(parent, name), m_pw(pw)
{
}

VisCSCPlotWidgetAxisY::~VisCSCPlotWidgetAxisY(void)
{
}
QSize VisCSCPlotWidgetAxisY::minimumSizeHint() const
{
	// minimumSizeHint/sizeHint must be coordinated with x axis/VisCSCPlotWidget 
	return QSize( 50, 40 );
}
QSize VisCSCPlotWidgetAxisY::sizeHint() const
{
	return QSize( 60, 50 );
}
void VisCSCPlotWidgetAxisY::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setPen(Qt::gray);
	int xoffset = 4; // 3 + 1 to back off from the width()'th pixel
	int tickLength = 4;
	QPolygon scale( 4 );
	scale.setPoint(0, width()- xoffset - tickLength, 0);
	scale.setPoint(1, width()-xoffset, 0 );
	scale.setPoint(2, width()-xoffset, height()-1 );
	scale.setPoint(3, width()- xoffset - tickLength, height()-1);
	painter.drawPolyline(scale);

	QFont sansFont( "Arial", 12 );
	painter.setFont( sansFont );
	QFontMetrics fm( sansFont );
	// maxv is the full-scale value
        if (m_pw->getScale() > 0.F ) 
        {
	    QString maxv = QString("%1").arg(m_pw->height()/m_pw->getScale(),0,'f',0);
	    painter.drawText( width() - xoffset -tickLength - fm.width(maxv) - 2, fm.height(), maxv );
        }
}
