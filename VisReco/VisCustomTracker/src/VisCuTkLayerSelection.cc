/****************************************************
 *
 *       VisCuTkLayerSelection.cc
 *
 * ***************************************************/
#define QT_NO_EMIT

#include "VisReco/VisCustomTracker/interface/VisCuTracker.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayerSlWindow.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSlWindow.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayerSelection.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModule.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSelectionBar.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkMap2D.h"
#include <QPainter>

VisCuTkLayerSelection::VisCuTkLayerSelection( QWidget *parent, const char *name, VisCuTkState* layer)
	: QWidget( parent, name)
{
   setPalette( QPalette( QColor( 250, 250, 250) ) );
   xmin = -2.0; xmax = 2.0; ymin = -2.0; ymax =2.0; x_win = 380;
   gp = dynamic_cast<VisCuTkLayerSlWindow *>(parent);  
   lay = dynamic_cast<VisCuTkLayer*>(layer);
   posrel = true; 
   horizontal_view = true;
   int part = lay->getOwner()->getId();
   int det = lay->getOwner()->getOwner()->getId();
   if(lay==0) std::cout <<"error lay " <<std::endl;
   else {
       nlay =VisCuTracker::nlayer(det,part,lay->getId());
       drawModules();
   }
}

void VisCuTkLayerSelection::update()
{
    repaint();	
}

void VisCuTkLayerSelection::mousePressEvent(QMouseEvent *e)
{
  for(int i=0; i<nelem; i++){
    QRegion q(rg[i],false);
    if(q.contains(e->pos())){
      VisCuTkModule* module = dynamic_cast<VisCuTkModule*>(state[i]) ;
      gp->labelinfo->setText(QString::fromStdString (module->getName()));
      gp->qw->gpp->drawpart(state[i]);
      repaint();
      break;
    }
  }
}

void VisCuTkLayerSelection::drawModules()
{
  nelem = 0; 
  defwindow(nlay);
  for(unsigned int l=0; l<lay->components(); l++ ){
    VisCuTkSubLayer* sublayer = lay->getComponent(l+1);
    for(unsigned int m=0; m<sublayer->components(); m++ ){
      VisCuTkModule* module = sublayer->getComponent(m+1);
      if(!module->notInUse()){
      QPolygon a = QPolygon(4); 
      drawmodule(module,nlay,a);
      rg[nelem]=a;
      rg[nelem].detach();
      state[nelem]=module;
      nelem++;
      }
    }
  }			     
}

void VisCuTkLayerSelection::paintEvent(QPaintEvent*)
{
    QPainter p( this );
    p.setPen(Qt::black);    
    for(int i=0; i<nelem; i++){
      p.setBrush(Qt::yellow);
      if(state[i]->getId()>100) p.setBrush(Qt::blue);   
      if(state[i]->isVisible())
	{
	  p.setBrush(Qt::red);  
	}     
      p.drawPolygon(rg[i]);
    }
}

void VisCuTkLayerSelection::drawmodule(VisCuTkModule * mod,int nlay,QPolygon &a )
{
  int x,y;
  double phi,r,dx,dy,dy1;
  double xp[4],yp[4],xp1,yp1;
  double vhbot,vhtop,vhapo;
  double rmedio[]={0.041,0.0701,0.0988,0.255,0.340,0.430,0.520,0.610,0.696,0.782,0.868,0.965,1.080};
  double xt1,yt1,xs1=0.,ys1=0.,xt2,yt2,xs2,ys2;
  
  phi = VisCuTkMap2D::phival(mod->posx,mod->posy);
  r = sqrt(mod->posx*mod->posx+mod->posy*mod->posy);
  vhbot = mod->width;
  vhtop=mod->width;
  vhapo=mod->length;
  
  if(nlay < 31){ //endcap
    vhbot = mod->widthAtHalfLength/2.-(mod->width/2.-mod->widthAtHalfLength/2.);
    vhtop=mod->width/2.;
    vhapo=mod->length/2.;
    if(nlay >12 && nlay <19){//pix endcap
      if(posrel)r = r+r;
      xp[0]=r-vhtop;yp[0]=-vhapo;
      xp[1]=r+vhtop;yp[1]=-vhapo;
      xp[2]=r+vhtop;yp[2]=vhapo;
      xp[3]=r-vhtop;yp[3]=vhapo;
    }else{
      if(posrel)r = r + r/3.;
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
    if(posrel){
      int numod;
      numod=mod->getId();if(numod>100)numod=numod-100;
      int vane = mod->getOwner()->getId();
      dx = vhapo;
      phi=M_PI;
      xt1=rmedio[nlay-31]; yt1=-vhtop/2.;
      xs1 = xt1*cos(phi)-yt1*sin(phi);
      ys1 = xt1*sin(phi)+yt1*cos(phi);
      xt2=rmedio[nlay-31]; yt2=vhtop/2.;
      xs2 = xt2*cos(phi)-yt2*sin(phi);
      ys2 = xt2*sin(phi)+yt2*cos(phi);
      dy=VisCuTkMap2D::phival(xs2,ys2)-VisCuTkMap2D::phival(xs1,ys1);
      dy1 = dy;
      if(nlay==31)dy1=0.39;
      if(nlay==32)dy1=0.23;
      if(nlay==33)dy1=0.16;
      xp[0]=vane*(dx+dx/6.);yp[0]=numod*(dy1+dy1/6.);
      xp[1]=vane*(dx+dx/6.)+dx;yp[1]=numod*(dy1+dy1/6.);
      xp[2]=vane*(dx+dx/6.)+dx;yp[2]=numod*(dy1+dy1/6.)+dy;
      xp[3]=vane*(dx+dx/6.);yp[3]=numod*(dy1+dy1/6.)+dy;
    }else{
      xt1=r; yt1=-vhtop/2.;
      xs1 = xt1*cos(phi)-yt1*sin(phi);
      ys1 = xt1*sin(phi)+yt1*cos(phi);
      xt2=r; yt2=vhtop/2.;
      xs2 = xt2*cos(phi)-yt2*sin(phi);
      ys2 = xt2*sin(phi)+yt2*cos(phi);
      xp[0]=mod->posz-vhapo/2.;yp[0]=4.2*VisCuTkMap2D::phival(xs1,ys1);
      xp[1]=mod->posz+vhapo/2.;yp[1]=4.2*VisCuTkMap2D::phival(xs1,ys1);
      xp[2]=mod->posz+vhapo/2.;yp[2]=4.2*VisCuTkMap2D::phival(xs2,ys2);
      xp[3]=mod->posz-vhapo/2.;yp[3]=4.2*VisCuTkMap2D::phival(xs2,ys2);
    }
  }
  VisCuTkSubLayer* sl = mod->getOwner();
  if(sl->isStereo())
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

void VisCuTkLayerSelection::defwindow(int nlay){
    if(posrel){ //separated modules
      xmin=-2.;ymin=-2.;xmax=2.;ymax=2.;
      if(nlay >12 && nlay < 19){//endcap pixel
        xmin=-.40;xmax=.40;ymin=-.40;ymax=.40;
      }
      if(nlay>30){//barrel
        xmin=-0.1;xmax=3.0;ymin=-0.1;ymax=8.5;
        if(nlay<34){xmin=-0.3;xmax=1.0;ymax=9.5;}//pixel
        if(nlay>33&&nlay<38){xmax=2.0;}//inner
        if(nlay>37){ymax=8.0;}//outer
      }
    }else{ //overlayed modules
        xmin=-1.3;ymin=-1.3;xmax=1.3;ymax=1.3;
        if(nlay >12 && nlay < 19){
          xmin=-.20;xmax=.20;ymin=-.20;ymax=.20;
        }
        if(nlay>30){
          xmin=-1.5;xmax=1.5;ymin=-1.;ymax=28.;
          if(nlay<32){xmin=-0.5;xmax=0.5;}
          if(nlay>33&&nlay<38){xmin=-1.;xmax=1.;}
        }
    }
}

QSizePolicy VisCuTkLayerSelection::sizePolicy() const
{
    return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}
void VisCuTkLayerSelection::closeEvent( QCloseEvent* ce )
{
  lay->setSlWindow(0);
  std::cout << "window closed"<< std::endl;
  ce->accept();
  return;
}
