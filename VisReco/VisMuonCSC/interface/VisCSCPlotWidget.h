#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include "qwidget.h"
#include "q3pointarray.h"
#include "qbrush.h"
//Added by qt3to4:
#include <QPaintEvent>
#include <QMouseEvent>
#include "VisReco/VisMuonCSC/interface/VisCSCStackPlots.h"
#include <vector>

/*! \class VisCSCPlotWidget
 *  \brief A Qt widget holding a bar plot
 *
 *  \author George Alverson, Northeastern University/CMS
 *  \date 10-MAY-2008
 *
 *   Designed specifically to display a (possibly sparse) 
 *   set of integer data. x and y axis scales are
 *   provided by the helper classes VisCSCPlotWidgetAxisX and
 *   VisCSCPlotWidgetAxisY.
 *   
 *   The number of bins must be declared at creation, but the
 *   data may be dynamically altered. The scale for the y-axis may 
 *   either be set or calculated automatically. 
 *
 */

class VisCSCPlotWidget :
	public QWidget
{
        Q_OBJECT
public:
	VisCSCPlotWidget(const int nbins, QWidget* parent = 0, const char* name = 0, Qt::WFlags f = 0);
	~VisCSCPlotWidget(void);
	void setData( const std::vector<VisCSCStackPlots::dataPt> &data); //! sets the data for the graph and updates
	QSize minimumSizeHint() const; //! standard qwidget: this is synched with x/y axis info
	QSize sizeHint() const; //! ditto
	void setMaxVal(float val); //! set the scale: val will be at 85% of height of plot
	void setAutoScale(bool b); //! let the widget calculate the scale: largest element is like MaxVal
	float getScale(void); //! return the scale factor: volatile

protected:
	void paintEvent(QPaintEvent *); //! where the work is done
	QPoint xform(const int i, const int val); //! transform data from bin/value space to pixels on widget 
	int xinvform(const QPoint ); //! returns bin from position
        void mousePressEvent( QMouseEvent * );
private:
	std::vector<VisCSCStackPlots::dataPt> m_data; //! local copy of the data
	Q3PointArray m_corners; //! coords of corners of outline
	const int m_bins; //! number of bins
	bool m_autoScale;
	float m_scale; //! volatile: y scale
	float m_maxVal; 
	float m_maxDataVal; //! maximum of supplied data
	std::vector<QBrush> m_brushes;
};
#endif //PLOTWIDGET_H

