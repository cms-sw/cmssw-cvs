/****************************************************************************
**      CuTkMap2D.cc
**
****************************************************************************/
#include "VisReco/VisCustomTracker/interface/VisCuCmsTracker.h"
#include "VisReco/VisCustomTracker/interface/VisCuTracker.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkMap2D.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkMapWindow.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkPartDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModule.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerSimHit.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerDigi.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerCluster.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerRecHit.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModuleMap.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <classlib/utils/DebugAids.h>
#include <QLabel>
#include <QPainter>
#include <QFileDialog>
#include <QPrinter>
#include <iostream>

using namespace edm;

VisCuTkMap2D::VisCuTkMap2D( QWidget *parent, const char *name,VisCuTrackerSimHit *simhitdr,VisCuTrackerDigi *digidr, VisCuTrackerCluster *clusterdr, VisCuTrackerRecHit *rechitdr)
  : QWidget( parent, name )
{
     const edm::ParameterSet *pset=VisConfigurationService::pSet();
  	   if(!pset)
  	   {
  	     edm::ParameterSet p;
  	     VisConfigurationService *visService = new VisConfigurationService(p);
  	     pset = visService->pSet();
  	     delete visService;
  	   }
  setPalette( QPalette( QColor( 250, 250, 250) ) );
  xmin = -3.0; xmax = 3.0; ymin = -0.1; ymax =1.0;
  ximg=600; yimg=600; x_win = 600;
  deltax=0; deltay=0;
  setMinimumSize( 600, 600 );
  setMaximumSize( 600, 600 );
  separated = false;
  drawall=false;
  drawsimhits=false;
  drawdigis=false;
  drawclusters=false;
  drawrechits=false;
  printing=false;
  xsize=250;
  ysize=200;
  acc = false;
  firsteventloaded = false;
  nev = 0;
  ix=0;iy=0;zoom=1;
  zoom_in_out =false;
  printer = new QPrinter(QPrinter::HighResolution);
  startintegrate=false;
  stopintegrate=false;
  simhitdraw=simhitdr;
  digidraw=digidr;
  clusterdraw=clusterdr;
  rechitdraw=rechitdr;
  horizontal_view = true;
}

void  VisCuTkMap2D::setParent(VisCuTkMapWindow *pw){
 	  	  gp = pw;
 	  	 }

void  VisCuTkMap2D::update(){
  repaint();
}

void  VisCuTkMap2D:: setEvent(const edm::Event& event,const edm::EventSetup &es)
{
  firsteventloaded =true;
  nev++;
  if(!acc)reset_val();
   if(acc && separated && (nev%nevToIntegrate)==0)switch_val();
}
void  VisCuTkMap2D::save(){
      QString fn = QFileDialog::getSaveFileName();
      QPixmap p = QPixmap::grabWidget(this);
      if (!fn.isEmpty())p.save( fn, "PNG");
  }

void  VisCuTkMap2D::print(){
  if ( printer->setup(this) ) {               
    std::cout << "Printing..."<<std::endl;
    printing=true;
    QPainter *p = new QPainter( this );
    p->end();                                // send job to printer
    if( !p->begin( printer ) )               // paint on printer
      return;
    drawpalette(p);
    p->setPen(Qt::black);
    for(unsigned int i=0; i<tracker->components(); i++){
      VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);       
      for(unsigned int j=0; j<subDetector->components(); j++ ){
	VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
	for(unsigned int k=0; k<partDetector->components(); k++ ){
	  nlay=VisCuTracker::nlayer(i+1,j+1,k+1);
	  defwindow(nlay);
	  VisCuTkLayer* layer = partDetector->getComponent(k+1);
	  for(unsigned int l=0; l<layer->components(); l++ ){
	    VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
	    for(unsigned int m=0; m<sublayer->components(); m++ ){
	      VisCuTkModule* module = sublayer->getComponent(m+1);
	      if (!module->notInUse()){
		drawmodule(module,nlay,p);
		if(drawsimhits&&module->simhit_size>0)simhitdraw->drawsimhit(module,nlay,p); 
		if(drawdigis&&module->digi_size>0)digidraw->drawdigi(module,nlay,p);
		if(drawclusters&&module->cluster_size>0)clusterdraw->drawcluster(module,nlay,p);
		if(drawrechits&&module->rechit_size>0)rechitdraw->drawrechit(module,nlay,p);
	      }
	    }   
	  }   
	}   
      }   
    }   
    p->end();                                // send job to printer
    std::cout <<   "Printing completed"<<std::endl;
    printing=false;
  } else {
    std::cout << "Printing aborted"<<std::endl;
  }
}

void VisCuTkMap2D::paintEvent(QPaintEvent*)
{
  std::cout << "Starting paint" << std::endl;
  QPainter *p = new QPainter( this );
  drawpalette(p);
  if(stopintegrate){if(!separated){delete image;delete pm;}stopintegrate=false;}
  if(startintegrate){
    if(!separated){
      image = new QImage(ximg,yimg,8,256);
      pm = new QPixmap(ximg,yimg);
      for ( int i=1; i<256; i++ )image->setColor( i, qRgb(255,i,0) );
      image->setColor( 0, qRgb(255,255,255) );
      for ( int y=0; y<yimg; y++ ) {                 // set image pixels
	uchar *p = image->scanLine(y);
	for ( int x=0; x<ximg; x++ )
	  *p++ = 0;
      }
    }
    startintegrate=false;
  }
  p->setPen(Qt::black);
  ASSERT (tracker);
  for(unsigned int i=0; i<tracker->components(); i++){
    VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);       
    for(unsigned int j=0; j<subDetector->components(); j++ ){
      VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
      for(unsigned int k=0; k<partDetector->components(); k++ ){
	nlay=VisCuTracker::nlayer(i+1,j+1,k+1);
	defwindow(nlay);
	VisCuTkLayer* layer = partDetector->getComponent(k+1);
	for(unsigned int l=0; l<layer->components(); l++ ){
	  VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
          for(unsigned int m=0; m<sublayer->components(); m++ ){
	    VisCuTkModule* module = sublayer->getComponent(m+1);
            if (module->isVisible()&&!module->notInUse()){
	      if(!(!separated && acc)) drawmodule(module,nlay,p);
              if(drawsimhits&&module->simhit_size>0)simhitdraw->drawsimhit(module,nlay,p); 
	      if(drawdigis &&module->digi_size>0)digidraw->drawdigi(module,nlay,p);
	      if(drawclusters&&module->cluster_size>0)clusterdraw->drawcluster(module,nlay,p);
	      if(drawrechits&&module->rechit_size>0)rechitdraw->drawrechit(module,nlay,p);
	    }
	  }   
	}   
      }
    }							    
  }
  if(acc && !separated){
   //pm = new QPixmap(ximg,yimg);
    *pm = *image;
    bitBlt(this,0,0,pm);
 }
}

QSizePolicy VisCuTkMap2D::sizePolicy() const
{
   return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}

void VisCuTkMap2D::defwindow(int nlay){
  if(separated){ // separated modules
    setPalette( QPalette( QColor( 158, 185, 225 ) ));
    xmin=-2.;ymin=-2.;xmax=2.;ymax=2.;
    if(nlay >12 && nlay < 19){
      xmin=-.40;xmax=.40;ymin=-.40;ymax=.40;
    }
    if(nlay>30){
      xmin=-0.1;xmax=3.;ymin=-0.1;ymax=8.5;
      if(nlay<34){xmin=-0.3;xmax=1.0;}
      if(nlay>33&&nlay<38){xmax=2.0;}
      if(nlay>37){ymax=8.;}//inner
    }
  }else{ //overlayed modules
    setPalette( QPalette( QColor( 255, 255, 255) ) );
    xmin=-1.3;ymin=-1.3;xmax=1.3;ymax=1.3;
    if(nlay >12 && nlay < 19){
      xmin=-.20;xmax=.20;ymin=-.20;ymax=.20;
    }
    if(nlay>30){
      xmin=-1.5;xmax=1.5;ymin=-1.;ymax=28.;
      if(nlay<34){xmin=-0.5;xmax=0.5;}
      if(nlay>33&&nlay<38){xmin=-1.;xmax=1.;}
    }    
  }
  if(zoom_in_out){
   float xmin1,xmax1,ymin1,ymax1;
   xmin1=xmin+(xmax-xmin)/2.-(xmax-xmin)*zoom/2.;
   xmax1=xmin+(xmax-xmin)/2.+(xmax-xmin)*zoom/2.;
   ymin1=ymin+(ymax-ymin)/2.-(ymax-ymin)*zoom/2.;
   ymax1=ymin+(ymax-ymin)/2.+(ymax-ymin)*zoom/2.;
   xmin=xmin1;xmax=xmax1;ymin=ymin1;ymax=ymax1;
  }
  if(!drawall && (deltax!=0 || deltay!=0)){
    float xmin1,xmax1,ymin1,ymax1;
    float movex= (float)deltax/ximg; 
    float movey=(float) deltay/yimg;
    if(horizontal_view){
      movex= (float)deltay/yimg; 
      movey= (float)deltax/ximg;
    }
    xmin1=xmin-movex*(xmax-xmin);
    xmax1=xmax-movex*(xmax-xmin);
    ymin1=ymin-movey*(ymax-ymin);
    ymax1=ymax-movey*(ymax-ymin);
    xmin=xmin1;xmax=xmax1;ymin=ymin1;ymax=ymax1;
  }
  if(drawall){
    if(nlay<16){
      ix=0;iy=(15-nlay)*ysize;}
    if(nlay>15&&nlay<31){
      ix=3*xsize;iy=(nlay-16)*ysize;}
    if(nlay>30){
      if(nlay==31){ix=(int)(1.5*xsize);iy=0;}    		 
      if(nlay>31 && nlay%2==0){int il=(nlay-30)/2;ix=xsize;iy=il*2*ysize;}
      if(nlay>31 && nlay%2!=0){int il=(nlay-30)/2;ix=2*xsize;iy=il*2*ysize;}
    } 
  } 
}

void VisCuTkMap2D::zoomIn()
{
  zoom = zoom*0.5;
  if(deltax!=0)deltax=deltax*2;
  if(deltay!=0)deltay=deltay*2;
  zoom_in_out = true;
  if(acc)startintegrate=true;
  repaint();
}

void VisCuTkMap2D::zoomOut()
{
  zoom = zoom*2;
  if(deltax!=0)deltax=(int)(deltax*0.5);
  if(deltay!=0)deltay=(int)(deltay*0.5);
  zoom_in_out = true;
  if(acc)startintegrate=true;
  repaint();
}

void VisCuTkMap2D::separateModules(){
  if(separated)separated=false;else separated=true;
  if(acc) startintegrate=true;
  repaint();
}

void VisCuTkMap2D::drawSimHits(){
  if(drawsimhits)drawsimhits=false;else 
    {drawsimhits=true;if(!acc)reset_val();}
  repaint();
}
void VisCuTkMap2D::drawDigi(){ 
  if(drawdigis)drawdigis=false;else 
   { drawdigis=true;if(!acc)reset_val();}
   repaint();
}
void VisCuTkMap2D::drawCluster(){ 
  if(drawclusters)drawclusters=false;else 
   {drawclusters=true;if(!acc)reset_val();}
   repaint();
}
void VisCuTkMap2D::drawRechit(){ 
  if(drawrechits)drawrechits=false;else 
    {drawrechits=true;if(!acc)reset_val();}
   repaint();
}

void VisCuTkMap2D::integratedSignal(bool acc1)
{
  acc=acc1;
  if(acc ){
    startintegrate=true;
    reset_val();
  } else stopintegrate=true;
  repaint();
}

void VisCuTkMap2D::pan(QPoint p, int dx, int dy){
    if(!drawall &&  (dx!=0 || dy!=0))
     {
    deltax=deltax+dx; deltay=deltay+dy;
    repaint();
  } else {
 if(dx==0 && dy==0) {
for(unsigned int i=0; i<tracker->components(); i++){
      VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);
      for(unsigned int j=0; j<subDetector->components(); j++ ){
        VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
        for(unsigned int k=0; k<partDetector->components(); k++ ){
          nlay=VisCuTracker::nlayer(i+1,j+1,k+1);
      if(!drawall || (drawall && nlay==getLayer(p.x(),p.y()))){
  defwindow(nlay);
         VisCuTkLayer* layer = partDetector->getComponent(k+1);
          for(unsigned int l=0; l<layer->components(); l++ ){
            VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
            for(unsigned int m=0; m<sublayer->components(); m++ ){
              VisCuTkModule* module = sublayer->getComponent(m+1);
              if (module->isVisible()&&!module->notInUse()){
                QPolygon a = QPolygon(4); 
                 computemodule(module,nlay,a);
                QRegion q(a,false);
                if(q.contains(p)){
                 std::cout << module->getName() << std::endl; 
                 gp->selectedModuleInfo->setText(QString::fromStdString (module->getName()));
                 break;
	      }
	      }
	    }   
	    }   
	  }   
	}   
      }   
    }   

}
}
}

void VisCuTkMap2D::drawAllTracker(){
  zoom_in_out =false;
  deltax=0; deltay=0;
  zoom=1.;
  if(drawall){
    drawall=false;ix=0;iy=0;
    ximg=600; yimg=600; x_win = 600;
    setMinimumSize( x_win, 600 );
    setMaximumSize( x_win, 600 );
  }
  else {
    drawall=true;
    if(!horizontal_view){
      ximg=4*xsize; yimg=15*ysize;
      setMinimumSize( 4*xsize, 15*ysize );
      setMaximumSize( 4*xsize, 15*ysize );
    } else{
      ximg=15*ysize; yimg=4*xsize; x_win = 4*xsize;
      setMinimumSize( 15*ysize, x_win );
      setMaximumSize( 15*ysize, x_win );
    }
  }
  if(acc&& !separated)startintegrate=true;
  repaint();
}

int VisCuTkMap2D::getLayer(int ix,int iy){
 int add;
 int res=0;
  if(iy <= xsize){//endcap+z
   add = 15;
    res = ix/ysize;
     res = res+add+1;
    }
   if(iy > xsize && iy< 3*xsize){//barrel
    add=30;
     if(ix < 2*ysize){
        res=1;
         }else {
  res = ix/(2*ysize);
     if(iy < 2*xsize)res=res*2+1; else res=res*2;
      }
       res = res+add;
      }
     if(iy >= 3*xsize){        //endcap-z
      res = ix/ysize;
       res = 15-res;
      }
 return res;
 }

int VisCuTkMap2D::xpixel(double x){
  int res;
  if(drawall) {res= (int)((x-xmin)/(xmax-xmin)*xsize)+ix;
  if(printing)res=res*3;return res;}
  else{
    res= (int)((x-xmin)/(xmax-xmin)*width());
    if(printing)res=res*7;return res;
  }
}
int VisCuTkMap2D::ypixel(double y){
  int res;
  double y1;
  y1 = (y-ymin)/(ymax-ymin);
  if(drawall){if(nlay>30)   res= 2*ysize - (int)(y1*2*ysize)+iy;
  else res= ysize - (int)(y1*ysize)+iy;
  if(printing)res=res*2;}
  //else {res= height() - (int)(y1*height());
  else {res= height() - (int)(y1*yimg);
  if(printing)res=res*7;}
  if (tracker->type==2){
    if(!drawall){
      if(nlay==34)res=res+(int)(50/zoom);
      if(nlay==35)res=res-(int)(50/zoom);
      if(nlay==38)res=res-(int)(150/zoom);
      if(nlay==39)res=res -(int)(200/zoom);
    }
  }
  return res;
}

void VisCuTkMap2D::drawmodule(VisCuTkModule * mod,int nlay,QPainter *p){
  int x,y;
  double phi,r,dx,dy,dy1;
  double xp[4],yp[4],xp1,yp1;
  double vhbot,vhtop,vhapo;
  double rmedio[]={0.041,0.0701,0.0988,0.255,0.340,0.430,0.520,0.610,0.696,0.782,0.868,0.965,1.080};
  double xt1,yt1,xs1=0.,ys1=0.,xt2,yt2,xs2,ys2,pv1,pv2;
  
  QPolygon a =  QPolygon(4);
  phi = phival(mod->posx,mod->posy);
  r = sqrt(mod->posx*mod->posx+mod->posy*mod->posy);
  
  vhbot = mod->width; 	  
  vhtop=mod->width; 	  
  vhapo=mod->length;
  if(nlay < 31){//endcap
    vhbot = mod->widthAtHalfLength/2.-(mod->width/2.-mod->widthAtHalfLength/2.); 	  
    vhtop=mod->width/2.; 	  
    vhapo=mod->length/2.;
    if(nlay >12 && nlay <19){//pix endcap
      if(separated)r = r+r;
      xp[0]=r-vhtop;yp[0]=-vhapo;
      xp[1]=r+vhtop;yp[1]=-vhapo;
      xp[2]=r+vhtop;yp[2]=vhapo;
      xp[3]=r-vhtop;yp[3]=vhapo;
    }else{
      if(separated)r = r + r/3.;
      xp[0]=r-vhapo;yp[0]=-vhbot;
      xp[1]=r+vhapo;yp[1]=-vhtop;
      xp[2]=r+vhapo;yp[2]=vhtop;
      xp[3]=r-vhapo;yp[3]=vhbot;
    }
    for(int j=0;j<4;j++){
      xp1 = xp[j]*cos(phi)-yp[j]*sin(phi);
      yp1 = xp[j]*sin(phi)+yp[j]*cos(phi);
      xp[j] = xp1;yp[j]=yp1;
    }
  } else { //barrel
    int numod;
    numod=mod->getId();if(numod>100)numod=numod-100;
    int vane = mod->getOwner()->getId();
    if(separated){
      dx = vhapo;
      phi=M_PI;
      xt1=rmedio[nlay-31]; yt1=-vhtop/2.;
      xs1 = xt1*cos(phi)-yt1*sin(phi);
      ys1 = xt1*sin(phi)+yt1*cos(phi);
      xt2=rmedio[nlay-31]; yt2=vhtop/2.;
      xs2 = xt2*cos(phi)-yt2*sin(phi);
      ys2 = xt2*sin(phi)+yt2*cos(phi);
      dy=phival(xs2,ys2)-phival(xs1,ys1);
      dy1 = dy;
      if(nlay==31)dy1=0.39;
      if(nlay==32)dy1=0.23;
      if(nlay==33)dy1=0.16;
      xp[0]=vane*(dx+dx/8.);yp[0]=numod*(dy1);
      xp[1]=vane*(dx+dx/8.)+dx;yp[1]=numod*(dy1);
      xp[2]=vane*(dx+dx/8.)+dx;yp[2]=numod*(dy1)+dy;
      xp[3]=vane*(dx+dx/8.);yp[3]=numod*(dy1)+dy;
    }else{
      xt1=r; yt1=-vhtop/2.;
      xs1 = xt1*cos(phi)-yt1*sin(phi);
      ys1 = xt1*sin(phi)+yt1*cos(phi);
      xt2=r; yt2=vhtop/2.;
      xs2 = xt2*cos(phi)-yt2*sin(phi);
      ys2 = xt2*sin(phi)+yt2*cos(phi);
      pv1=phival(xs1,ys1);
      pv2=phival(xs2,ys2);
      if(fabs(pv1-pv2)>M_PI && numod==1)pv1=pv1-2.*M_PI;
      if(fabs(pv1-pv2)>M_PI && numod!=1)pv2=pv2+2.*M_PI;
      xp[0]=mod->posz-vhapo/2.;yp[0]=4.2*pv1;
      xp[1]=mod->posz+vhapo/2.;yp[1]=4.2*pv1;
      xp[2]=mod->posz+vhapo/2. ;yp[2]=4.2*pv2;
      xp[3]=mod->posz-vhapo/2.;yp[3]=4.2*pv2;
    }
  }
  VisCuTkSubLayer* sl = mod->getOwner();
  if(sl->isStereo()&&separated)
    {
      if(mod->getId()>100 ){
	for(int j=0;j<3;j++){
	  x=xpixel(xp[j]);y=ypixel(yp[j]);
	  if(!horizontal_view)a.setPoint(j,x,y);else a.setPoint(j,y,x_win-x);
	}
	if(!horizontal_view)a.setPoint(3,x,y);else a.setPoint(3,y,x_win-x);
      }else {
	x=xpixel(xp[2]);y=ypixel(yp[2]);
	if(!horizontal_view)a.setPoint(0,x,y);else a.setPoint(0,y,x_win-x);
	x=xpixel(xp[3]);y=ypixel(yp[3]);
	if(!horizontal_view)a.setPoint(1,x,y);else a.setPoint(1,y,x_win-x);
	x=xpixel(xp[0]);y=ypixel(yp[0]);
	if(!horizontal_view)a.setPoint(2,x,y);else a.setPoint(2,y,x_win-x);
	if(!horizontal_view)a.setPoint(3,x,y);else a.setPoint(3,y,x_win-x);
      }
    } else {
      for(int j=0;j<4;j++){
	x=xpixel(xp[j]);y=ypixel(yp[j]);
	if(!horizontal_view)a.setPoint(j,x,y);else a.setPoint(j,y,x_win-x);
      }
    }                 
  if(separated){    
    if(drawdigis||drawsimhits||drawclusters||drawrechits){
      p->setPen(QColor( 158, 185, 225));
      int green = 255-mod->value; if (green < 0)green = 0;if(green > 255)green=255;
      p->setBrush(QColor(255,green,0));
      if(mod->value==0)p->setBrush(Qt::white);
    } else {
      
      p->setBrush(Qt::yellow);
      if(mod->getId()>100) p->setBrush(Qt::blue); 
    }
  }
  p->drawPolygon(a);
}
void VisCuTkMap2D::computemodule(VisCuTkModule * mod,int nlay,QPolygon &a){
  int x,y;
  double phi,r,dx,dy,dy1;
  double xp[4],yp[4],xp1,yp1;
  double vhbot,vhtop,vhapo;
  double rmedio[]={0.041,0.0701,0.0988,0.255,0.340,0.430,0.520,0.610,0.696,0.782,0.868,0.965,1.080};
  double xt1,yt1,xs1=0.,ys1=0.,xt2,yt2,xs2,ys2,pv1,pv2;
  
  phi = phival(mod->posx,mod->posy);
  r = sqrt(mod->posx*mod->posx+mod->posy*mod->posy);
  
  vhbot = mod->width; 	  
  vhtop=mod->width; 	  
  vhapo=mod->length;
  if(nlay < 31){//endcap
    vhbot = mod->widthAtHalfLength/2.-(mod->width/2.-mod->widthAtHalfLength/2.); 	  
    vhtop=mod->width/2.; 	  
    vhapo=mod->length/2.;
    if(nlay >12 && nlay <19){//pix endcap
      if(separated)r = r+r;
      xp[0]=r-vhtop;yp[0]=-vhapo;
      xp[1]=r+vhtop;yp[1]=-vhapo;
      xp[2]=r+vhtop;yp[2]=vhapo;
      xp[3]=r-vhtop;yp[3]=vhapo;
    }else{
      if(separated)r = r + r/3.;
      xp[0]=r-vhapo;yp[0]=-vhbot;
      xp[1]=r+vhapo;yp[1]=-vhtop;
      xp[2]=r+vhapo;yp[2]=vhtop;
      xp[3]=r-vhapo;yp[3]=vhbot;
    }
    for(int j=0;j<4;j++){
      xp1 = xp[j]*cos(phi)-yp[j]*sin(phi);
      yp1 = xp[j]*sin(phi)+yp[j]*cos(phi);
      xp[j] = xp1;yp[j]=yp1;
    }
  } else { //barrel
    int numod;
    numod=mod->getId();if(numod>100)numod=numod-100;
    int vane = mod->getOwner()->getId();
    if(separated){
      dx = vhapo;
      phi=M_PI;
      xt1=rmedio[nlay-31]; yt1=-vhtop/2.;
      xs1 = xt1*cos(phi)-yt1*sin(phi);
      ys1 = xt1*sin(phi)+yt1*cos(phi);
      xt2=rmedio[nlay-31]; yt2=vhtop/2.;
      xs2 = xt2*cos(phi)-yt2*sin(phi);
      ys2 = xt2*sin(phi)+yt2*cos(phi);
      dy=phival(xs2,ys2)-phival(xs1,ys1);
      dy1 = dy;
      if(nlay==31)dy1=0.39;
      if(nlay==32)dy1=0.23;
      if(nlay==33)dy1=0.16;
      xp[0]=vane*(dx+dx/8.);yp[0]=numod*(dy1);
      xp[1]=vane*(dx+dx/8.)+dx;yp[1]=numod*(dy1);
      xp[2]=vane*(dx+dx/8.)+dx;yp[2]=numod*(dy1)+dy;
      xp[3]=vane*(dx+dx/8.);yp[3]=numod*(dy1)+dy;
    }else{
      xt1=r; yt1=-vhtop/2.;
      xs1 = xt1*cos(phi)-yt1*sin(phi);
      ys1 = xt1*sin(phi)+yt1*cos(phi);
      xt2=r; yt2=vhtop/2.;
      xs2 = xt2*cos(phi)-yt2*sin(phi);
      ys2 = xt2*sin(phi)+yt2*cos(phi);
      pv1=phival(xs1,ys1);
      pv2=phival(xs2,ys2);
      if(fabs(pv1-pv2)>M_PI && numod==1)pv1=pv1-2.*M_PI;
      if(fabs(pv1-pv2)>M_PI && numod!=1)pv2=pv2+2.*M_PI;
      xp[0]=mod->posz-vhapo/2.;yp[0]=4.2*pv1;
      xp[1]=mod->posz+vhapo/2.;yp[1]=4.2*pv1;
      xp[2]=mod->posz+vhapo/2. ;yp[2]=4.2*pv2;
      xp[3]=mod->posz-vhapo/2.;yp[3]=4.2*pv2;
    }
  }
  VisCuTkSubLayer* sl = mod->getOwner();
  if(sl->isStereo()&&separated)
    {
      if(mod->getId()>100 ){
	for(int j=0;j<3;j++){
	  x=xpixel(xp[j]);y=ypixel(yp[j]);
	  if(!horizontal_view)a.setPoint(j,x,y);else a.setPoint(j,y,x_win-x);
	}
	if(!horizontal_view)a.setPoint(3,x,y);else a.setPoint(3,y,x_win-x);
      }else {
	x=xpixel(xp[2]);y=ypixel(yp[2]);
	if(!horizontal_view)a.setPoint(0,x,y);else a.setPoint(0,y,x_win-x);
	x=xpixel(xp[3]);y=ypixel(yp[3]);
	if(!horizontal_view)a.setPoint(1,x,y);else a.setPoint(1,y,x_win-x);
	x=xpixel(xp[0]);y=ypixel(yp[0]);
	if(!horizontal_view)a.setPoint(2,x,y);else a.setPoint(2,y,x_win-x);
	if(!horizontal_view)a.setPoint(3,x,y);else a.setPoint(3,y,x_win-x);
      }
    } else {
      for(int j=0;j<4;j++){
	x=xpixel(xp[j]);y=ypixel(yp[j]);
	if(!horizontal_view)a.setPoint(j,x,y);else a.setPoint(j,y,x_win-x);
      }
    }                 
}

void VisCuTkMap2D::drawpalette(QPainter *p){
  int posx = 200, posy=15,thickness=20, htext = 30;
  if(separated&&(drawsimhits||drawdigis||drawclusters||drawrechits)){
    for(int value=0;value<255;value++){
      int green=255-value;
      if (green < 0)green = 0;if(green > 255)green=255;
      p->setPen(QColor(255,green,0));
      p->drawRect( posx+value, posy, 1,thickness );
      if(value%100 == 0 ){
	QString colnum=QString::number(value);
	p->setPen(Qt::black);
	p->drawText( posx+value, posy,  colnum ); 
	p->drawRect( posx+value, posy, 1,thickness );
      }
    }
    p->setPen(Qt::black);
    p->drawRect( posx+255, posy, 1,thickness );
    p->drawText( posx+255, posy,">255" ); 
    if(drawsimhits)p->drawText( posx, posy+htext ,  " # of SimHits per module" ); 
    if(drawdigis)p->drawText( posx, posy+htext ,  " # of digi per module" );    
    if(drawclusters)p->drawText( posx, posy+htext ,  " # of clusters per module" );    
    if(drawrechits)p->drawText( posx, posy+htext ,  " # of rechits per module" );    
  }
}

void VisCuTkMap2D::reset_val(){
  std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod; 
  for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
    VisCuTkModule* mod = imod->second;
    mod->value = 0;
    mod->bufvalue = 0;
  }
}
void VisCuTkMap2D::switch_val(){
   std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod;
   for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
     VisCuTkModule* mod = imod->second;
     mod->value = mod->bufvalue;
     mod->bufvalue = 0;
 }
 }

