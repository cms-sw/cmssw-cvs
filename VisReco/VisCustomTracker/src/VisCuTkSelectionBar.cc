#include "VisReco/VisCustomTracker/interface/VisCuTkSlWindow.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSelectionBar.h"
#include "VisReco/VisCustomTracker/interface/VisCuTracker.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkPartDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayer.h"

#include <QPainter>
#include <iostream>

VisCuTkSelectionBar::VisCuTkSelectionBar(VisCuTkSlWindow* sw, QWidget *parent, const char *name )
        : QWidget( parent, name )
{
   setPalette( QPalette( QColor( 250, 250, 250) ) );
   xmin = -3.0; xmax = 3.0; ymin = -0.1; ymax =1.0;

   gpp = sw;
}

void VisCuTkSelectionBar::setTracker(VisCuCmsTracker * tr)
{
  tracker=tr;
  drawSelectionBars();
}

void VisCuTkSelectionBar::drawSelectionBars(){
  
   float xsq[]={2.75,2.57,2.39,2.21,2.03,1.85,1.67,1.50,1.33,1.16,1.03,0.90,0.77,0.64,0.51,- 0.51,-0.64,-0.77,-0.90,-1.03,-1.16,-1.33,-1.50,-1.67,-1.85,-2.03,-2.21,-2.39,-2.57,-2.75,-2.85,-2.85,-2.85,-2.85,-2.85,-2.85,-2.85,-2.85,-2.85,-2.85,-2.85,-2.85,-2.85};
   float ysq[]={0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.95,0.090,0.140,0.190,0.31,0.36,0.43,0.48,0.55,0.61,0.67,0.74,0.81,0.88};

   float rmedioS[]={0.387, 0.447, 0.507, 0.567, 0.657, 0.777, 0.887};
   float rmedioP[]={0.0155, 0.065, 0.105, 0.145, 0.185, 0.225, 0.265};

   float sizeF[]={0.08,0.08,0.08,0.08,0.11,0.14,0.14};
   float xp[]={-0.40,-0.29, -0.18,-0.06,0.06,0.18,0.29,0.40}; 
   float xsi[]={-0.60,-0.50,-0.40,-0.30,-0.20,-0.10,0.10,0.20,0.30,0.40,0.50,0.60};
   float xso[]={-1.13,-0.93,-0.74,-0.55,-0.36,-0.16,0.05,0.24,0.44,0.63,0.82,1.01};
   int nlay;
   ntotlay =0;
   nelem=0;
   //draw squares
   for(unsigned int i=0; i<tracker->components(); i++){
     VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);       
     for(unsigned int j=0; j<subDetector->components(); j++ ){
       VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
       for(unsigned int k=0; k<partDetector->components(); k++ ){
	 ntotlay++;
         nlay=VisCuTracker::nlayer(i+1,j+1,k+1);
         //std::cout <<nlay << std::endl;
         VisCuTkLayer* layer = partDetector->getComponent(k+1);
         if(nlay<31){
	   rsqr[nelem]=QRect(xpixel(xsq[(31-nlay)-1]),ypixel(ysq[nlay-1]),5,5);	
	   state[nelem]=layer;
	   nelem=nelem+1;
	 }else {
	   rsqr[nelem]=QRect(xpixel(xsq[nlay-1]-0.05),ypixel(ysq[nlay-1]),5,5);
	   state[nelem]=layer;
	   nelem=nelem+1;
	 }
	 if(nlay<31){
	   rsqr[nelem]=QRect(xpixel(xsq[(31-nlay)-1]),ypixel(-0.03),5,5);
	   state[nelem]=layer;
	   nelem=nelem+1;
	 }
	 else{
	   rsqr[nelem]=QRect(xpixel(2.90),ypixel(ysq[nlay-1]),5,5);
	   state[nelem]=layer;
	   nelem=nelem+1;
	 }
       }	   
     }    
   }
   //std::cout << " ntotlay1=" << ntotlay<<std::endl;
   //draw bars
   for(unsigned int i=0; i<tracker->components(); i++){
      VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);
      for(unsigned int j=0; j<subDetector->components(); j++ ){
        VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
	for(unsigned int k=0; k<partDetector->components(); k++ ){
	  nlay=VisCuTracker::nlayer(i+1,j+1,k+1);
	  VisCuTkLayer* layer = partDetector->getComponent(k+1);
	  for(unsigned int l=0; l<layer->components(); l++ ){
	    VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
	    int id = sublayer->getId();
	    float a = 0;	    
	    if(nlay<13 || (nlay>18 && nlay<31)){//TEC+TID
	      if(nlay<13 && id%2==0)a=0.05; 
	      if(nlay<13 && id%2!=0)a=0.0;
	      if((nlay>18 && nlay<31) && id%2==0)a=0.0;
	      if((nlay>18 && nlay<31) && id%2!=0)a=0.05;
	      int xendcap = xpixel(xsq[(31-nlay)-1]+ a);
	      rsqr[nelem]=QRect(xendcap,ypixel(rmedioS[id-1]),3,ypixel(0)-ypixel(sizeF[id-1]));
	      state[nelem]= sublayer;
	      nelem=nelem+1;
	    }
	    if(nlay>12 && nlay<19 ){//Pixel EndCap
	      if(nlay==16 || nlay==17 || nlay==18)a=0.03;
	      int xpixel1 = xpixel(xsq[(31-nlay)-1]+a);
	      rsqr[nelem]=QRect(xpixel1,ypixel(rmedioP[id-1]),3,6);
              state[nelem]= sublayer;
              nelem=nelem+1;	      
	    }
	    if(nlay>30 && nlay<34){//Pixel Barrel
	       rsqr[nelem]=QRect(xpixel(xp[id-1]),ypixel(ysq[nlay-1]),8,3);
	       state[nelem]= sublayer;
	       nelem=nelem+1;
	    }
	    if(nlay>33 && nlay<38){//inner barrel
	      rsqr[nelem]=QRect(xpixel(xsi[id-1]),ypixel(ysq[nlay-1]),8,3);
	      state[nelem]= sublayer;
	      nelem=nelem+1;
	    }
	    if(nlay>37){//outer barrel
	      if((id%2==0 && id<7) || (id>6 && id%2!=0))a = 0.01;     
	      rsqr[nelem]=QRect(xpixel(xso[id-1]),ypixel(ysq[nlay-1]+a),17,3);
	      state[nelem]= sublayer;
	      nelem=nelem+1;
	    }
	  }
	}
      }
   }    
}

void  VisCuTkSelectionBar::update(){
  repaint();
}

void VisCuTkSelectionBar::mousePressEvent(QMouseEvent *e)
{
  for(int i=0; i<nelem; i++){
    if(rsqr[i].contains(e->pos())){
      if((i%2==1 && i<(2*ntotlay)) || i>(2*ntotlay-1)){ //yellow or bars
	if(i%2==1 && i<(2*ntotlay)){ 
	  VisCuTkLayer* lay = dynamic_cast<VisCuTkLayer*>(state[i]);     
	  label.setNum(lay->getId());   
	  gpp->labelinfop->setText(QString(" Layer ").append(label).append (" of ").append (QString::fromStdString (lay->getOwner()->getName())).append(" selected"));  
	}            else{ 
	  VisCuTkSubLayer* subly = dynamic_cast<VisCuTkSubLayer*>(state[i]); 
	  label.setNum(subly->getId());  
	  label1.setNum(subly->getOwner()->getId());   
	  gpp->labelinfop->setText(QString("Ring ").append(label).append(" layer ").append(label1).append(" of ").append(QString::fromStdString (subly->getOwner()->getOwner()->getName())).append(" selected")); 
	}
	gpp->drawpart(state[i]);
	repaint();
      }
      if(i%2==0 && i<(2*ntotlay)){//green squares
	VisCuTkLayer* lay = dynamic_cast<VisCuTkLayer*>(state[i]);  
	if(lay->getSlWindow()==0 ||lay->getSlWindow()->close() ){
	  layerWindow = new VisCuTkLayerSlWindow(0, "layerWindow",0,state[i], this);
	  layerWindow->show();	 
	  lay->setSlWindow(layerWindow);
	  repaint();
	} else {
	  if(lay->getSlWindow()->isMinimized()){
	    lay->getSlWindow()->showNormal();
	    lay->getSlWindow()->setActiveWindow();
	    lay->getSlWindow()->raise();
	  }
	}
	repaint();
      }
      break;
    }
  }  
}

void VisCuTkSelectionBar::paintEvent(QPaintEvent*)
{
  QPainter p( this );
  p.setPen(Qt::black);
  
  for(int i=0; i<(2*ntotlay); i++){
    if((i%2)==1){
      p.setBrush(Qt::yellow);
      if(state[i]->isVisible()) p.setBrush(Qt::red);
    }   
    else {
      VisCuTkLayer* lay = dynamic_cast<VisCuTkLayer*>(state[i]);  
      if(lay->getSlWindow()!=0)p.setBrush(Qt::red);else p.setBrush(Qt::green);	    
    }
    p.drawRect(rsqr[i]); 
  }
  for(int i=(2*ntotlay); i<nelem; i++){
    p.setBrush(Qt::black);
    VisCuTkSubLayer* sublay = dynamic_cast<VisCuTkSubLayer*>(state[i]); 
    if(sublay->isStereo()){
      p.setPen(Qt::blue);
      p.setBrush(Qt::blue);  
    }     
    if(state[i]->isVisible()) {
      p.setPen(Qt::red);
      p.setBrush(Qt::red);
    }     
    p.drawRect(rsqr[i]);
    p.setPen(Qt::black); 
  } 
}
QSizePolicy VisCuTkSelectionBar::sizePolicy() const
{
   return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}


