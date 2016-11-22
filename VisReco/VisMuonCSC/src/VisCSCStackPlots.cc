#include "VisReco/VisMuonCSC/interface/VisCSCStackPlots.h"
#include "VisReco/VisMuonCSC/interface/VisCSCPlotWidget.h"
#include "VisReco/VisMuonCSC/interface/VisCSCPlotWidgetAxisX.h"
#include "VisReco/VisMuonCSC/interface/VisCSCPlotWidgetAxisY.h"
#include "VisReco/VisMuonCSC/interface/VisCSCChamberSelectorImpl.h"
#include "VisReco/VisMuonCSC/interface/VisCSCChamberSelectorImplService.h"

#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"

#include "Iguana/Framework/interface/IgState.h"

#include <QLayout>
#include <QLabel>
#include <QFont>
#include <QApplication>
#include <QGridLayout>
#include <iostream>

namespace
{
// could probably do a lot simply in the class, esp if use asymmetric test

        bool CSCDetCmp(const CSCDetId l, const CSCDetId r)
        {
            return
            (l.layer() == r.layer() || l.layer() ==0 || r.layer() ==0 )  &&
            (l.chamber() == r.chamber() || l.chamber() ==0 || r.chamber() ==0 )  &&
            (l.ring() == r.ring() || l.ring() ==0 || r.ring() ==0 )  &&
            (l.station() == r.station() || l.station() ==0 || r.station() ==0 )  &&
            (l.endcap() == r.endcap() || l.endcap() ==0 || r.endcap() ==0 );
        }
}


VisCSCStackPlots::VisCSCStackPlots(IgState* state, int np, int nb, QWidget* parent, const char* name, Qt::WFlags f): 
QWidget(parent, f), m_widgets(np), m_np(np)
{
        m_did = CSCDetId(0, 0, 0, 0); // accept all by default
	
	setWindowTitle("CSC Strip ADC: Shift-F1 for info");
	setWhatsThis(tr("Strip pulse heights.X: channels; Y: ADC counts; chamber layers (6) are stacked in the strip plots, each of which represents one of the time ticks (8). Click on a strip for its details."));
	
        // Create the selector widget
        VisCSCChamberSelectorImplService *selSvc = VisCSCChamberSelectorImplService::get(state);
        if (! selSvc)
        {
            selSvc = new VisCSCChamberSelectorImplService (state);
        }
        m_selector = selSvc->selector();
	connect(m_selector, SIGNAL(selectionChange(uint32_t)), this, SLOT(newSelector(uint32_t)));
	
	/* 3 columns by np+2 rows:

	      x | label  |
	 y-axis | p0     |
	 y-axis | p1     | spacer
	 :        :        :
	 y-axis | pn-1   |
	        | x-axis |
	 
	 if we decide later to add labels to axis, will increase size
    */
	
	QGridLayout *l = new QGridLayout( this );
	//, 1, 1, 0, 0 );
	QLabel *label = new QLabel(name, this);
	QFont fnt("Helvetica");
	label->setFont(fnt);
	label->setAlignment(Qt::AlignCenter);

	l->addWidget(label, 0, 1);
	l->setColSpacing( 2, 10 ); // insert spacer on right: 10 px
	l->setColStretch( 1, 1 );  // default is zero, so only col 1 likes to stretch

	for (int i=0; i < m_np; i++)
	{
		VisCSCPlotWidget *pn = new VisCSCPlotWidget( nb, this, "plot" );
		m_widgets[i] = pn;
		pn->setAutoScale(true); // do this before adding data
		VisCSCPlotWidgetAxisY *y = new VisCSCPlotWidgetAxisY(pn, this);
		l->addWidget( y, i+1, 0 );
		l->addWidget( pn, i+1, 1 );
		connect(this,SIGNAL(newData(void)),y,SLOT(update()));
	}
	VisCSCPlotWidgetAxisX *xaxis= new VisCSCPlotWidgetAxisX( nb, this );
	l->addWidget( xaxis, m_np+1, 1 );
}

VisCSCStackPlots::~VisCSCStackPlots(void)
{
}
void VisCSCStackPlots::setData(const int plot, const std::vector<dataPt> &data)
{
        if (plot >= m_np)
        {
            // issue warning and reject
            return;
        }
	m_widgets[plot]->setData(data);
}
void VisCSCStackPlots::setData(std::vector< std::pair<CSCDetId, CSCStripDigi> >& digis)
{
        m_digis = digis;
	newSelector(m_did);
}
void VisCSCStackPlots::newSelector(uint32_t id)
{
        m_did = id;
	
//      need to sort the digis by strip, then re-assemble in groups of layers and times ticks:
//      final:          layers (6)
//                   +--------------------
//                0  |                    |
//      ticks (8) 1  |                    |    repeated number of strips
//                2  |                    |

 	const int n_tick = 8; // 8 time ticks per strip is standard
	const int n_layer = 6;
	//one bin will have six layer values, a single tick
	std::vector<int> dummy6(n_layer,0);
	std::vector<int> dummy8(n_tick,0);
	
        // get block of chamber/layer/ticks and generate a unique index based on chamber+strip
        std::map<uint64_t, std::vector<std::vector<int> > > buff;
	std::vector< std::vector<int > > dummy68(n_layer, dummy8); 	// will use this to initialize 

	// stuff the digis in 
	for (std::vector<std::pair<CSCDetId, CSCStripDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	{
            if (CSCDetCmp((*i).first, m_did)) //filter
            {
                uint64_t tid = (*i).first.chamberId().rawId(); //chamberId strips off the layer
                tid <<= 32;
                tid |= (*i).second.getStrip();
		if (buff.find(tid) == buff.end()) buff[tid] = dummy68;
//                int i1 = (*i).second.getStrip();
//                int i2 = (*i).first.layer();
//                std::cout << " To buff: strip/layer/tid: " << i1 << " " << i2 << " " << std::hex << tid << std::endl;
//                std::vector<int> i3 = (*i).second.getADCCounts();
//                std::ostream_iterator<int> o(std::cout, " ");
//                std::copy(i3.begin(), i3.end(), o);  
//                std::cout << std::endl;
		buff[tid][(*i).first.layer()-1] = (*i).second.getADCCounts();  // strips are 1-biased
	     }
	}        
	// now should be roughly in chamber/strip order. pull data and put in output array.
	std::vector<std::vector<int> > obuf(n_tick, dummy6);
        std::vector<dataPt> ddummy;
        std::vector<std::vector<dataPt> > final(n_tick,ddummy);
        int iout = 1;
        for (std::map<uint64_t, std::vector<std::vector<int> > >::const_iterator im = buff.begin(); im != buff.end(); ++im, ++iout)
        {
            for (std::vector<std::vector<int> >::const_iterator in = (*im).second.begin(); in != (*im).second.end(); ++in)
            {
                for (std::vector<int>::const_iterator it = (*in).begin(); it != (*in).end(); ++it)
                {
		    // transpose and store     
                   int itd = static_cast<int>(std::distance((*in).begin(),it));
                   int ind = static_cast<int>(std::distance((*im).second.begin(),in));
                   obuf[itd][ind] = (*it);
                }
            }
            for (std::vector<std::vector<dataPt > >::iterator o = final.begin(); o != final.end(); ++o )
            {
                (*o).push_back(dataPt((*im).first, obuf[std::distance(final.begin(), o)]));
//                 std::cout << "Putting point " << iout << std::endl;
//                 std::ostream_iterator<float> oi(std::cout, " ");
//                 std::copy(id.begin(), id.end(), oi);
//                 std::cout << std::endl;
            }
         }
	 int plot=0;
	 for (std::vector<std::vector<dataPt> >::iterator it=final.begin(); it != final.end(); ++it, ++plot)
	 {
	     m_widgets[plot]->setData(*it);
	 }
	 newData(); // reset the y-axis, then update the plots
	 for (std::vector<VisCSCPlotWidget*>::iterator it=m_widgets.begin(); it != m_widgets.end(); ++it)
	 {
	     (*it)->update();
	 }
	 
}
void VisCSCStackPlots::show( void )
{
        this->QWidget::show();
	m_selector->show();
}
