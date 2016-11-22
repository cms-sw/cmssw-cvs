#ifndef VIS_CU_TK_Map_2D_H
#define VIS_CU_TK_Map_2D_H

# include <QWidget>
# include <cmath>
# include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
# include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
# include "DataFormats/GeometryVector/interface/LocalPoint.h"
# include "SimDataFormats/TrackingHit/interface/PSimHit.h"
# include <vector>

namespace edm 
{
    class Event;
    class EventSetup;
}

class SiStripDigi;
class SiStripCluster;

class VisCuCmsTracker;

class QPainter;
class QPrinter;
class VisCuTkModule;
class VisCuTrackerSimHit;
class VisCuTrackerDigi;
class VisCuTrackerCluster;
class VisCuTrackerRecHit;
class VisCuTkMapWindow;
class QImage;
class QPolygon;

class VisCuTkMap2D: public QWidget
{
    Q_OBJECT

public:
    VisCuTkMap2D(QWidget *parent=0, const char *name=0,VisCuTrackerSimHit *simhitdr=0, VisCuTrackerDigi *digidr=0, VisCuTrackerCluster *clusterdr=0, VisCuTrackerRecHit *rechitdr=0);
  
    void update();
    void separateModules();	
    void drawAllTracker();	
    void drawSimHits();	
    void drawDigi();
    void drawCluster();
    void drawRechit();
    void setTracker(VisCuCmsTracker * tr){tracker=tr;};
    void setEvent(const edm::Event& iEvent,const edm::EventSetup &eventSetup);
    void integratedSignal(bool acc1);
    void pan(QPoint pos , int dx, int dy);
    void setParent(VisCuTkMapWindow * pw);
  
    static double phival(double x, double y){
	double phi;
	double phi1=atan(y/x); 
	phi = phi1;
	if(y<0. && x>0) phi = phi1+2.*M_PI;
	if(x<0.)phi=phi1+M_PI;
	if(fabs(y)<0.000001 && x>0)phi=0;
	if(fabs(y)<0.000001&&x<0)phi=M_PI;
	if(fabs(x)<0.000001&&y>0)phi=M_PI/2.;
	if(fabs(x)<0.000001&&y<0)phi=3.*M_PI/2.;
	return phi;
    }
    int xpixel(double x);
    int ypixel(double y);
    int getLayer(int x, int y);
    bool separated;
    bool acc;
    QImage* image;
    QPixmap* pm;
    int ximg,yimg, x_win;
    VisCuTkMapWindow *gp;
    int nevToIntegrate;
    int xsize,ysize;

public slots:
    
    void print();
    void save();
    void zoomIn();
    void zoomOut();
  
protected:
    void paintEvent(QPaintEvent* );	
  
private:  
 
    std::vector<SiStripDigi> sdigi;
    std::vector<SiStripCluster> cdigi;
    std::vector<LocalPoint> rdigi;
    // std::string HepMCLabel;
    QSizePolicy sizePolicy() const;
    QPrinter *printer;
    VisCuCmsTracker * tracker;
    VisCuTrackerSimHit * simhitdraw;
    VisCuTrackerDigi * digidraw;
    VisCuTrackerCluster * clusterdraw;
    VisCuTrackerRecHit * rechitdraw;

    double xmin,xmax,ymin,ymax,zoom;
    int deltax,deltay; 
    int ix,iy;
    void  defwindow(int nlay);
    void drawmodule(VisCuTkModule * mod,int nlay,QPainter *p);
    void computemodule(VisCuTkModule * mod,int nlay,QPolygon &a);
    void  drawrechit(VisCuTkModule * mod,int nlay,QPainter *p);
    void  drawpalette(QPainter *p);
    void  reset_val();
    void  switch_val();
 
    bool drawall, horizontal_view;
    bool drawsimhits,drawdigis,drawclusters,drawrechits;
    bool printing;
    bool firsteventloaded;
    bool zoom_in_out;
    bool startintegrate,stopintegrate;
    int nlay;
    std::vector<PSimHit> theTrackerHits;
    int nev;
  
};
#endif 
