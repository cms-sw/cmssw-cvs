#include "VisReco/VisMuonCSC/interface/VisCSCPlotWidgetAxisX.h"
#include "QPainter"
#include <QPaintEvent>
#include <cassert>
#include <cmath>
#include <iostream>

VisCSCPlotWidgetAxisX::VisCSCPlotWidgetAxisX(const int nbins, QWidget* parent, const char* name): QWidget(parent, name), 
m_bins(nbins)
{
	assert(m_bins>0);
}

VisCSCPlotWidgetAxisX::~VisCSCPlotWidgetAxisX(void)
{
}
QSize VisCSCPlotWidgetAxisX::minimumSizeHint() const
{
	// minimumSizeHint/sizeHint must be coordinated with y axis/VisCSCPlotWidget 
	return QSize( 80, 40 );
}
QSize VisCSCPlotWidgetAxisX::sizeHint() const
{
	return QSize( 400, 50 );
}
void VisCSCPlotWidgetAxisX::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setPen(Qt::blue);
	QFont sansFont( "Helvetica", 9 );
	painter.setFont( sansFont );
	QFontMetrics fm( sansFont );

	// assume every five bins is a good choice: change to 16 for CSC
	const int bin_spacing = 16;
	QPolygon scale( 4 * int(floor(float(m_bins) / bin_spacing)) );
	int yoffset = 2;
	int tickHeight = 4;

	// don't forget that the last pixel is width()-1
	for (int i = bin_spacing, j = 0; i <= m_bins; i += bin_spacing )
	{
		// labels go under center of bin before tick marks
		QString label = QString("%1").arg(i);
		int labelWidth = fm.width(label);
		int xtext = int(floor( (i - 0.5F)*width()/m_bins - labelWidth/2.F + 0.5F) );
		// check to see last label doesn't stick over the edge
		if (xtext > width()-1 - fm.width(label))
		{
			xtext = width()-1 - fm.width(label);
		}
		painter.drawText( xtext, fm.height(), label );
		// draw tick marks
		scale.setPoint(j++, int(float(i - bin_spacing)*(width()-1)/m_bins + 0.5F), yoffset+tickHeight );
		scale.setPoint(j++, int(float(i - bin_spacing)*(width()-1)/m_bins + 0.5F), yoffset );
		scale.setPoint(j++, int(float(i              )*(width()-1)/m_bins + 0.5F), yoffset );
		scale.setPoint(j++, int(float(i              )*(width()-1)/m_bins + 0.5F), yoffset+tickHeight );
	}	
	painter.drawPolyline(scale);
}
