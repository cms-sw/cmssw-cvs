#include "VisReco/VisMuonCSC/interface/VisCSCPlotWidget.h"
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPaintEvent>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cassert>
#include <cmath>
#include <iostream>

namespace 
{
struct dataPtCompare
{
	bool operator() (const VisCSCStackPlots::dataPt& p1, const VisCSCStackPlots::dataPt& p2) const
	{
		return p1.first < p2.first ;
	}
};
struct dataPtDataCompare
{
	bool operator() (const VisCSCStackPlots::dataPt& p1, const VisCSCStackPlots::dataPt& p2) const
	{
		return std::accumulate(p1.second.begin(), p1.second.end(), 0.F ) < std::accumulate(p2.second.begin(), p2.second.end(), 0.F) ;
	}
};
}



VisCSCPlotWidget::VisCSCPlotWidget (const int nbins, QWidget* parent, const char* name, Qt::WFlags f) :
  QWidget(parent, f), m_bins(nbins), m_autoScale(true), m_maxVal(-1.F)
{
	assert(m_bins>0);
	

	// set up multiple colors. Use hsv so can just pick hue.

	QColor q;
	q.setHsv(120, 255, 255); //green
	m_brushes.push_back(QBrush(q));
	q.setHsv(220, 255, 255);
	m_brushes.push_back(QBrush(q)); 
	q.setHsv(320, 255, 255);
	m_brushes.push_back(QBrush(q)); 
	q.setHsv(170, 255, 255); // about cyan
	m_brushes.push_back(QBrush(q)); 
	q.setHsv(270, 255, 255);
	m_brushes.push_back(QBrush(q)); 
	q.setHsv( 10, 255, 255);
	m_brushes.push_back(QBrush(q)); 

}

VisCSCPlotWidget::~VisCSCPlotWidget(void)
{
}
QSize VisCSCPlotWidget::minimumSizeHint() const
{
	// minimumSizeHint/sizeHint must be coordinated with x/y axis 
	return QSize( 80, 40 );
}
QSize VisCSCPlotWidget::sizeHint() const
{
	return QSize( 400, 100 );
}
void VisCSCPlotWidget::paintEvent(QPaintEvent*)
{
	int lastBin = 0;
	std::vector<QPoint> vtx(1,xform(0,0)); //first corner
	m_scale = getScale();

	for (std::vector<VisCSCStackPlots::dataPt>::const_iterator it = m_data.begin(); it != m_data.end(); it++ )
	{
	        int height = std::accumulate((it->second).begin(), (it->second).end(), 0);
		vtx.push_back( xform(lastBin++, height ) );
		vtx.push_back( xform(lastBin,   height ) ); 
	}
	vtx.push_back( xform(lastBin, 0) ); //last corner
	

	QPainter painter(this);
	painter.setPen(Qt::blue);
//	painter.setBrush(Qt::green);

	// bars
 	for (std::vector<VisCSCStackPlots::dataPt>::iterator it = m_data.begin(); it != m_data.end(); it++ )
	{
		std::vector<int>::const_iterator hit0 = it->second.begin();	
		int height = 0;
		for (std::vector<int>::const_iterator hit=hit0; hit != it->second.end(); ++hit)
		{
		        int dx = std::distance(m_data.begin(), it);
			painter.setBrush( m_brushes.at( std::distance( hit0, hit ) ) );
			painter.drawRect(QRect(xform(dx, *hit+height), xform(dx+1, height) ));
			height += *hit;
		}
	}
 	// outline
	QPolygon corners(int(vtx.size()));
	for (uint i=0;  i < vtx.size() ; i++)
	{
		corners.setPoint(i, vtx[i]);
	}
	painter.drawPolyline(corners);
}
QPoint VisCSCPlotWidget::xform(const int i, const int val)
{

	return QPoint(int(float(i*(width()-1))/m_bins + 0.5F), int(floor( height() - (val * m_scale + 0.5F ))));
}
int VisCSCPlotWidget::xinvform(const QPoint p)
{
        return   int( (float(p.x()))*(m_bins)/(width()-1) ); 

}	
void VisCSCPlotWidget::setData(const std::vector<VisCSCStackPlots::dataPt> & d)
{
	m_data = d;
	dataPtCompare dpc;
	std::sort( m_data.begin(), m_data.end(), dpc );
	dataPtDataCompare dpdc;
	if (d.begin() == d.end())
	{
		m_maxDataVal = -1.F; 
	}
	else
	{
		std::vector<VisCSCStackPlots::dataPt>::const_iterator it(std::max_element(m_data.begin(), m_data.end(), dpdc ));
		m_maxDataVal = std::accumulate((it->second).begin(), (it->second).end(), 0.F);
	}
}
void VisCSCPlotWidget::setMaxVal(float val) 
{
	m_maxVal = val;
	getScale(); // resets the scale to use the max value
}
void VisCSCPlotWidget::setAutoScale(bool b) {m_autoScale = b;}
float VisCSCPlotWidget::getScale(void) {

	float scale = -1.F;
	if (!m_autoScale && m_maxVal > 0)  // rolls over to auto if no maxVal
	{
		scale = 0.85F * height()/m_maxVal;
	}
	else if ( m_maxDataVal != 0)
	{
		scale = 0.85F * height()/m_maxDataVal;
	}
	
	
	return scale;
}
void VisCSCPlotWidget::mousePressEvent( QMouseEvent *e )
{
        uint i = uint( xinvform( e->pos() ) );
	if (i < m_data.size() )
	{
	    QString text = QString("Bin No: %1\n").arg(i);
	    uint32_t strip = m_data[i].first & 0xFFFFFFFF;
	    uint32_t chamber = (m_data[i].first >> 32) & 0xFFFFFFFF;
	    CSCDetId id(chamber);
	    text.append(QString("Strip: %1/").arg(strip));
	    text.append(QString("Chamber: %1/").arg(id.chamber()));
	    text.append(QString("Ring: %1/").arg(id.ring()));
	    text.append(QString("Station: %1/").arg(id.station()));
	    text.append(QString("Endcap: %1\nValues: \n").arg(id.endcap()));
	    for (std::vector<int>::const_iterator it = m_data[i].second.begin(); it != m_data[i].second.end(); ++it)
	    {
		// FIXME: Format
	        text.append (QString("%1\n").arg(*it));
	    }
	    QMessageBox::information(0, "CSC Channel info", text); 
	}
	else
	{
	    QMessageBox::warning(0, "CSC channel info", "channel not found");
	}
}
