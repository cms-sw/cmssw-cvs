#ifndef VISMUONCSCSTACKPLOTS_H
#define VISMUONCSCSTACKPLOTS_H

#include "qwidget.h"
#include <vector>
#include "DataFormats/MuonDetId/interface/CSCDetId.h"

class CSCStripDigi;
class CSCDetId;
class IgState;
class VisCSCChamberSelector;
class VisCSCPlotWidget;

/*!
 *	\class VisCSCStackPlots
 *	\brief Stacks together a number of bar plots with axes and
 *	packages them as a single widget.
 *
 *	\author George Alverson, Northeastern University/CMS
 */

class VisCSCStackPlots : public QWidget
{
        Q_OBJECT
public:

	typedef std::pair<uint64_t,std::vector<int> > dataPt; //! data is (bin [1 biased], value)
	/*!
	 * \arg \c np number of plots
	 * \arg \c nb number of bins in plots (must be common size)
	 * \arg \c parent parent widget, etc.
	 */
	VisCSCStackPlots(IgState *state, int np, int nb, QWidget* parent = 0, const char* name = 0, Qt::WFlags f = 0);
	~VisCSCStackPlots(void);
	/*!
	 * \arg did is the filter for which data to plot. A zero sub-field indicates a wild card.
	 */
        void setData(std::vector< std::pair<CSCDetId, CSCStripDigi> >& digis); //! sets data including address
public slots:
        void newSelector(uint32_t id);
	void show( void );
signals:
        void newData(void);
protected:
	/*!
	 * \arg \c plot is from top to bottom: 0 at top.
	 */
	void setData(const int plot, const std::vector<dataPt> &data); //! sets the data for the plot [0:np-1] and updates 
private:
	std::vector<VisCSCPlotWidget*> m_widgets; 
        const int m_np; //! number of plots
	std::vector< std::pair<CSCDetId, CSCStripDigi> > m_digis; //! local data copy
	CSCDetId m_did; //! strip filter
	VisCSCChamberSelector * m_selector;
};
#endif //VISMUONCSCSTACKPLOTS_H

