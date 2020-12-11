//<<<<<< INCLUDES                                                       >>>>>>
#include "VisReco/VisCustomTracker/interface/VisCuTrackerSimHit.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "FWCore/Framework/interface/Selector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkMap2D.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModuleMap.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <QPainter>
#include <iostream>
#include <iomanip>

using namespace edm;

VisCuTrackerSimHit::VisCuTrackerSimHit ()
{
  m_horizontal_view = true;
  m_firsteventloaded=false;
    const edm::ParameterSet *pset=VisConfigurationService::pSet();
  if(!pset)
    {
      edm::ParameterSet p;
      VisConfigurationService *visService = new VisConfigurationService(p);
      pset = visService->pSet();
      delete visService;
    }
  std::cout <<"simhitProducer="<< simhitProducerName <<  std::endl;

}

void
VisCuTrackerSimHit::setEvent (const edm::Event &event,
			      const edm::EventSetup &eventSetup)
{
  eventSetup.get<TrackerDigiGeometryRecord> ().get (m_pDD);
  m_trackerHits.clear();
  m_firsteventloaded=true;
  std::vector<PSimHit> tempTrackerHits;
  std::cout << "sono VisCuTrackerSimhit 0"<< std::endl;
  
  edm::Handle<edm::PSimHitContainer> PixelBarrelHitsLowTof;
  edm::Handle<edm::PSimHitContainer> PixelBarrelHitsHighTof;
  edm::Handle<edm::PSimHitContainer> PixelEndcapHitsLowTof;
  edm::Handle<edm::PSimHitContainer> PixelEndcapHitsHighTof;
  edm::Handle<edm::PSimHitContainer> TIBHitsLowTof;
  edm::Handle<edm::PSimHitContainer> TIBHitsHighTof;
  edm::Handle<edm::PSimHitContainer> TIDHitsLowTof;
  edm::Handle<edm::PSimHitContainer> TIDHitsHighTof;
  edm::Handle<edm::PSimHitContainer> TOBHitsLowTof;
  edm::Handle<edm::PSimHitContainer> TOBHitsHighTof;
  edm::Handle<edm::PSimHitContainer> TECHitsLowTof;
  edm::Handle<edm::PSimHitContainer> TECHitsHighTof;
  bool nosimhits=true;
if(simhitProducerName!="ignore"){
 std::vector<edm::Handle<edm::PSimHitContainer> > hitCollections;
    ModuleLabelSelector sel(simhitProducerName);
    event.getMany (sel, hitCollections);
      if (! hitCollections.empty () )nosimhits=false;
      if(nosimhits) std::cout << "no simhits type "<<simhitProducerName<<"in event" << std::endl;
      if(!nosimhits){
      event.getByLabel(simhitProducerName,"TrackerHitsPixelBarrelLowTof", PixelBarrelHitsLowTof);
      event.getByLabel(simhitProducerName,"TrackerHitsPixelBarrelHighTof", PixelBarrelHitsHighTof);
      event.getByLabel(simhitProducerName,"TrackerHitsPixelEndcapLowTof", PixelEndcapHitsLowTof);
      event.getByLabel(simhitProducerName,"TrackerHitsPixelEndcapHighTof", PixelEndcapHitsHighTof);
      event.getByLabel(simhitProducerName,"TrackerHitsTIBLowTof", TIBHitsLowTof);
      event.getByLabel(simhitProducerName,"TrackerHitsTIBHighTof", TIBHitsHighTof);
      event.getByLabel(simhitProducerName,"TrackerHitsTIDLowTof", TIDHitsLowTof);
      event.getByLabel(simhitProducerName,"TrackerHitsTIDHighTof", TIDHitsHighTof);
      event.getByLabel(simhitProducerName,"TrackerHitsTOBLowTof", TOBHitsLowTof);
      event.getByLabel(simhitProducerName,"TrackerHitsTOBHighTof", TOBHitsHighTof);
      event.getByLabel(simhitProducerName,"TrackerHitsTECLowTof", TECHitsLowTof);
      event.getByLabel(simhitProducerName,"TrackerHitsTECHighTof", TECHitsHighTof);
      tempTrackerHits.insert(tempTrackerHits.end(), PixelBarrelHitsLowTof->begin(), PixelBarrelHitsLowTof->end());
      tempTrackerHits.insert(tempTrackerHits.end(), PixelBarrelHitsHighTof->begin(), PixelBarrelHitsHighTof->end());
      tempTrackerHits.insert(tempTrackerHits.end(), PixelEndcapHitsLowTof->begin(), PixelEndcapHitsLowTof->end());
      tempTrackerHits.insert(tempTrackerHits.end(), PixelEndcapHitsHighTof->begin(), PixelEndcapHitsHighTof->end());
      tempTrackerHits.insert(tempTrackerHits.end(), TIBHitsLowTof->begin(), TIBHitsLowTof->end());
      tempTrackerHits.insert(tempTrackerHits.end(), TIBHitsHighTof->begin(), TIBHitsHighTof->end());
      tempTrackerHits.insert(tempTrackerHits.end(), TIDHitsLowTof->begin(), TIDHitsLowTof->end());
      tempTrackerHits.insert(tempTrackerHits.end(), TIDHitsHighTof->begin(), TIDHitsHighTof->end());
      tempTrackerHits.insert(tempTrackerHits.end(), TOBHitsLowTof->begin(), TOBHitsLowTof->end());
      tempTrackerHits.insert(tempTrackerHits.end(), TOBHitsHighTof->begin(), TOBHitsHighTof->end());
      tempTrackerHits.insert(tempTrackerHits.end(), TECHitsLowTof->begin(), TECHitsLowTof->end());
      tempTrackerHits.insert(tempTrackerHits.end(), TECHitsHighTof->begin(), TECHitsHighTof->end());
      std::cout << "sono VisCuTrackerSimhit 1"<< std::endl;
      for (std::vector<PSimHit>::iterator isim = tempTrackerHits.begin(); isim != tempTrackerHits.end(); ++isim)
	{
	  m_trackerHits.push_back(*isim);
	}
            
//	      std::vector<std::pair<GeomDetUnit *,PSimHit > > rtemp;
	      if (! tempTrackerHits.empty () && m_pDD.isValid ()){
	      int nsimhit=0;
	      for (std::vector<PSimHit>::iterator isim = tempTrackerHits.begin(); isim != tempTrackerHits.end(); ++isim)
	      {
	      DetId detIdObject(isim->detUnitId());
	      const GeomDetUnit      * geoUnit = m_pDD->idToDetUnit( detIdObject );      
	      VisCuTkModule * mod = VisCuTkModuleMap::moduleMap[geoUnit];
	      nsimhit++;
	      if(mod){
	      mod->simhit_size=mod->simhit_size+1;
//	      std::pair<GeomDetUnit*, PSimHit> pxd;
//	      pxd.first = mod->detUnit;
//	      pxd.second = *isim;
//	      rtemp.push_back(pxd);
	      } else std::cout <<"simhit#"<< nsimhit<<" module not found " << std::endl;
	      
	      }//for on isim
	      }// if
//	      int rd_pointer=0;
//	      std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod;
//	      for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
//	      VisCuTkModule* mod = imod->second;
//	      if(mod->simhit_size>0){
//	      mod->simhit_start=rd_pointer; rd_pointer = rd_pointer + mod->simhit_size;
//	      for(int i =0;i<rtemp.size();i++){
//	      std::pair<GeomDetUnit*, PSimHit> pxd = rtemp[i];
//	      GeomDetUnit * geomdet = pxd.first;
//	      if (geomdet==mod->detUnit)m_trackerHits.push_back(pxd.second);
//	      }//for on rtemp
//	      }//if module has simhits
//	      }//loop on modules
 //     */     
    }
    }
}

SoSeparator*
VisCuTrackerSimHit::update (VisCuDetectorRep::DetectorRep type)
{
  
      SoSeparator *sep = new SoSeparator; // global
  if (! m_trackerHits.empty () && m_pDD.isValid ())
    {
      SoMaterial *mat = new SoMaterial;
      mat->diffuseColor.setValue (1.0, 0.0, 1.0);//pink
      mat->emissiveColor.setValue(1.0, 0.0, 1.0);
      
      SoDrawStyle *drawStyle = new SoDrawStyle;
      drawStyle->pointSize = 4.0;
      
      sep->addChild (mat);
      sep->addChild (drawStyle);
      
      try
        {
	  SoVertexProperty *vertices = new SoVertexProperty;
	  int nVrtx = 0;
	  std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod;
	  for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
	    VisCuTkModule* mod = imod->second;
	    if(mod->isVisible()){
	      for (std::vector<PSimHit>::iterator isim = m_trackerHits.begin(); isim != m_trackerHits.end(); ++isim){
		if(mod->detUnit->geographicalId().rawId()==isim->detUnitId()){
		  
		  GlobalPoint inner = mod->detUnit->surface().toGlobal((*isim).localPosition());
		  float x = inner.x()/100;
                  float y;
                  if(type==VisCuDetectorRep::RZDet)
		  {
                   x=0.0;
                   y = sqrt(inner.x()*inner.x()+inner.y()*inner.y()) / 100.0;
		   if(inner.y()< 0. )y = - y;
		  }
		  else
		    y =  inner.y()/100;
		  float z = inner.z()/100;
		  vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
		}
	      }
            }
	  }
	  vertices->vertex.setNum (nVrtx);
	  SoPointSet *points = new SoPointSet;
	  points->vertexProperty.setValue (vertices);
	  points->numPoints.setValue (nVrtx);
	  
	  sep->addChild (points);
        }
      catch (...)
        {
	  std::cout << "No Simhits in tracker." << std::endl;
        }
    }
      return sep;
}

void VisCuTrackerSimHit::drawsimhit( VisCuTkModule * mod,int nlay,QPainter *p)
{
  int x,y;
  double phi,r,dx,dy,dy1;
  double xp[4],yp[4],xp1,yp1;
  double vhbot,vhtop,vhapo;
  double rmedio[]={0.041,0.0701,0.0988,0.255,0.340,0.430,0.520,0.610,0.696,0.782,0.868,0.965,1.080};
  double xt1,yt1,xs1=0.,ys1=0.,xt2,yt2,xs2,ys2,pv1,pv2;
  int numbersimhits=0;

  if(m_firsteventloaded){
    QPolygon a =  QPolygon(4);
    phi = VisCuTkMap2D::phival(mod->posx,mod->posy);
    r = sqrt(mod->posx*mod->posx+mod->posy*mod->posy);
    vhbot = mod->width;
    vhtop=mod->width;
    vhapo=mod->length;
    
    if(nlay < 31){//endcap
      vhbot = mod->widthAtHalfLength/2.-(mod->width/2.-mod->widthAtHalfLength/2.);
      vhtop=mod->width/2.;
      vhapo=mod->length/2.;
      if(nlay >12 && nlay <19){//pix endcap
        if(m_map->separated)r = r+r;
        xp[0]=r-vhtop;yp[0]=-vhapo;
        xp[1]=r+vhtop;yp[1]=-vhapo;
        xp[2]=r+vhtop;yp[2]=vhapo;
        xp[3]=r-vhtop;yp[3]=vhapo;
      }else{
        if(m_map->separated)r = r + r/3.;
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
      p->setPen(QColor(255,0,0));
      p->setBrush(QColor(255,0,0));

      for (std::vector<PSimHit>::iterator isim = m_trackerHits.begin(); isim != m_trackerHits.end(); ++isim){
        if(mod->detUnit->geographicalId().rawId()==isim->detUnitId()){
          numbersimhits++;
          float x = mod->detUnit->surface().toGlobal((*isim).localPosition()).x()/100;
          float y = mod->detUnit->surface().toGlobal((*isim).localPosition()).y()/100;
          int xc = m_map->xpixel(x);
          int yc = m_map->ypixel(y);
          if(m_map->acc && !m_map->separated){
              if(!m_horizontal_view){
              if(xc > (m_map->ximg -1)) xc =  (m_map->ximg -1);
              if(yc > (m_map->yimg -1)) yc =(m_map->yimg-1);
                } else {
              if(yc > (m_map->ximg -1)) yc =  (m_map->ximg -1);
              if(xc > (m_map->yimg -1)) xc =(m_map->yimg-1);
                }
              QRgb pixel;
	      if(!m_horizontal_view){
		pixel = m_map->image->pixel(xc,yc);
              }else pixel = m_map->image->pixel(yc,m_map->x_win-xc);
	      int green = qGreen(pixel);
              if(green > 1) green = green -20;
              if(green < 0) green = 0;
              if(!m_horizontal_view)m_map->image->setPixel(xc,yc,green);else m_map->image->setPixel(yc,m_map->x_win-xc,green);
          }
          if(!m_map->acc && !m_map->separated && !m_horizontal_view) p->drawRect(xc,yc,3,3);
	  if(!m_map->acc && !m_map->separated && m_horizontal_view) p->drawRect(yc,m_map->x_win-xc,3,3);
        }
      }
    } else { //barrel
      int numod;
      numod=mod->getId();if(numod>100)numod=numod-100;
      int vane = mod->getOwner()->getId();
      if(m_map->separated){
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
        pv1=VisCuTkMap2D::phival(xs1,ys1);
        pv2=VisCuTkMap2D::phival(xs2,ys2);
        if(fabs(pv1-pv2)>M_PI && numod==1)pv1=pv1-2.*M_PI;
        if(fabs(pv1-pv2)>M_PI && numod!=1)pv2=pv2+2.*M_PI;
        xp[0]=mod->posz-vhapo/2.;yp[0]=4.2*pv1;
        xp[1]=mod->posz+vhapo/2.;yp[1]=4.2*pv1;
        xp[2]=mod->posz+vhapo/2. ;yp[2]=4.2*pv2;
        xp[3]=mod->posz-vhapo/2.;yp[3]=4.2*pv2;
      }
      p->setPen( QColor(255,0,255));
      p->setBrush(QColor(255,0,255));
      for (std::vector<PSimHit>::iterator isim = m_trackerHits.begin(); isim != m_trackerHits.end(); ++isim){
        if(mod->detUnit->geographicalId().rawId()==isim->detUnitId()){
          numbersimhits++;
          float x = mod->detUnit->surface().toGlobal((*isim).localPosition()).x()/100;
          float y = mod->detUnit->surface().toGlobal((*isim).localPosition()).y()/100;
          float z = mod->detUnit->surface().toGlobal((*isim).localPosition()).z()/100;
          int xc = m_map->xpixel(z);
          int yc = m_map->ypixel(4.2*VisCuTkMap2D::phival(x,y));
          if(m_map->acc && !m_map->separated){
              if(!m_horizontal_view){
              if(xc > (m_map->ximg -1)) xc =  (m_map->ximg -1);
              if(yc > (m_map->yimg -1)) yc =(m_map->yimg-1);
                } else {
              if(yc > (m_map->ximg -1)) yc =  (m_map->ximg -1);
              if(xc > (m_map->yimg -1)) xc =(m_map->yimg-1);
                }
              QRgb pixel;
	      if(!m_horizontal_view){
		pixel = m_map->image->pixel(xc,yc);
              }else pixel = m_map->image->pixel(yc,m_map->x_win-xc);
	      int green = qGreen(pixel);
              if(green > 1) green = green -20;
              if(green < 0) green = 0;
              if(!m_horizontal_view)m_map->image->setPixel(xc,yc,green);else m_map->image->setPixel(yc,m_map->x_win-xc,green);
          }
          if(!m_map->acc && !m_map->separated && !m_horizontal_view)p->drawRect(xc,yc,3,3);
	  if(!m_map->acc && !m_map->separated && m_horizontal_view)p->drawRect(yc,m_map->x_win-xc,3,3);
        }
      }
      }
    VisCuTkSubLayer* sl = mod->getOwner();
    if(sl->isStereo()&&m_map->separated){
      if(mod->getId()>100 ){
        for(int j=0;j<3;j++){
          x=m_map->xpixel(xp[j]);y=m_map->ypixel(yp[j]);
	  if(!m_horizontal_view)a.setPoint(j,x,y);else a.setPoint(j,y,m_map->x_win-x);
        }
	if(!m_horizontal_view)a.setPoint(3,x,y);else a.setPoint(3,y,m_map->x_win-x);
      }else {
        x=m_map->xpixel(xp[2]);y=m_map->ypixel(yp[2]);
	if(!m_horizontal_view)a.setPoint(0,x,y);else a.setPoint(0,y,m_map->x_win-x);
        x=m_map->xpixel(xp[3]);y=m_map->ypixel(yp[3]);
	if(!m_horizontal_view)a.setPoint(1,x,y);else a.setPoint(1,y,m_map->x_win-x);
        x=m_map->xpixel(xp[0]);y=m_map->ypixel(yp[0]);
	if(!m_horizontal_view)a.setPoint(2,x,y);else a.setPoint(2,y,m_map->x_win-x);
	if(!m_horizontal_view)a.setPoint(3,x,y);else a.setPoint(3,y,m_map->x_win-x);
      }
    } else {
      for(int j=0;j<4;j++){
        x=m_map->xpixel(xp[j]);y=m_map->ypixel(yp[j]);
	if(!m_horizontal_view)a.setPoint(j,x,y);else a.setPoint(j,y,m_map->x_win-x);
       }
    }
    if(m_map->acc){mod->value = mod->value + numbersimhits;mod->bufvalue = mod->bufvalue + numbersimhits;}
    else mod->value = numbersimhits;
    if(m_map->separated){
      p->setPen(QColor( 158, 185, 225));
      int green = 255-mod->value; if (green < 0)green = 0;if(green > 255)green=255;
      p->setBrush(QColor(255,green,0));
      if(mod->value==0)p->setBrush(QColor(255,255,255));
      p->drawPolygon(a);
    }
    p->setPen(QColor(0,0,0));
    p->setBrush(Qt::NoBrush);

  }//m_firsteventloaded
}

std::string
VisCuTrackerSimHit::updatetext (VisCuTkModule * mod)
{
  std::ostringstream  text;
  int nsimhit=0;
  for (std::vector<PSimHit>::iterator isim = m_trackerHits.begin(); isim != m_trackerHits.end(); ++isim){
    if(mod->detUnit->geographicalId().rawId()==isim->detUnitId()){
    if(nsimhit==0)text << " simhits: --------------------------------------- <p>" ;
    GlobalPoint inner = mod->detUnit->surface().toGlobal((*isim).localPosition());
    float x = inner.x()/100;
    float y = inner.y()/100;
    float z = inner.z()/100;
    text << " Number=" << nsimhit++
	 << " Position=" << x  << ", " << y  << ", " << z << "<p>";
    }
  }
  if(nsimhit>0)text << " -----------------------------" ;
  return text.str();
}

