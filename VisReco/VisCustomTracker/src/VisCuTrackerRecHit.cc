//<<<<<< INCLUDES  >>>>>>

#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "FWCore/Framework/interface/Selector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerRecHit.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModuleMap.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkMap2D.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
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

VisCuTrackerRecHit::VisCuTrackerRecHit ()
{
  m_horizontal_view =true;
  m_firsteventloaded=false;
  const edm::ParameterSet *pset=VisConfigurationService::pSet();
  if(!pset)
  {
    edm::ParameterSet p;
    VisConfigurationService *visService = new VisConfigurationService(p);
    pset = visService->pSet();
    delete visService;
  }
  std::cout <<"striprechitProducer="<< striprechitProducerName <<  std::endl;
  std::cout <<"pixelrechitProducer="<< pixelrechitProducerName <<  std::endl;
}

void
VisCuTrackerRecHit::setEvent (const edm::Event &event,
			      const edm::EventSetup &eventSetup)
{
  eventSetup.get<TrackerDigiGeometryRecord> ().get (m_geom);
  
  edm::Handle<SiStripMatchedRecHit2DCollection> rechitsmatched;
  edm::Handle<SiStripRecHit2DCollection> rechitsrphi;
  edm::Handle<SiStripRecHit2DCollection> rechitsstereo;
  m_rdigi.clear();
  int rd_pointer=0;
  bool norechits=true;
  m_firsteventloaded=true; 
  if(striprechitProducerName!="ignore"){
    std::vector< edm::Handle<SiStripMatchedRecHit2DCollection> > rechit2d_matched_collections;
    ModuleLabelSelector sel(striprechitProducerName);
    event.getMany (sel, rechit2d_matched_collections);
    std::vector< edm::Handle<SiStripRecHit2DCollection> > rechit2d_collections;
    event.getMany (sel, rechit2d_collections);
    if (! rechit2d_matched_collections.empty () && !rechit2d_collections.empty ())norechits=false;
    if(norechits) std::cout << "no striprechits type "<<striprechitProducerName<<" in event" << std::endl;
    if(!norechits){
      event.getByLabel(striprechitProducerName,"matchedRecHit", rechitsmatched);
      event.getByLabel(striprechitProducerName,"rphiRecHit", rechitsrphi);
      event.getByLabel(striprechitProducerName,"stereoRecHit", rechitsstereo);
      if(rechitsmatched->size()>0||rechitsrphi->size()>0||rechitsstereo->size()>0){
	std::vector<std::pair<const GeomDetUnit *,std::pair<LocalPoint, const GeomDetUnit *> > > rtemp;
	if(rechitsmatched->size()>0){ 
	  for (SiStripMatchedRecHit2DCollection::const_iterator detunit_iterator = rechitsmatched->begin(), detunit_end = rechitsmatched->end();
	       detunit_iterator != detunit_end; ++detunit_iterator )
	  {
	    SiStripMatchedRecHit2DCollection::DetSet rechitRange = *detunit_iterator;
	    unsigned int id = rechitRange.detId();
	    if(id!=999999999){ //if is valid detector
	      VisCuTkModule * mod = 0, *mod1 = 0,*mod2 = 0;
	      DetId detIdObject(id);
	      DetId detIdObject1(id+1);
	      DetId detIdObject2(id+2);
	      const GeomDetUnit * geoUnit = 0,*geoUnit1 = 0,*geoUnit2 = 0;
	      int isStereo=id&0x3;
	      if(isStereo!=0){
		geoUnit = m_geom->idToDetUnit( detIdObject );
		mod = VisCuTkModuleMap::moduleMap[geoUnit];
	      }else{
		geoUnit1 = m_geom->idToDetUnit( detIdObject1 );
		geoUnit2 = m_geom->idToDetUnit( detIdObject2 );
		mod1 = VisCuTkModuleMap::moduleMap[geoUnit1];
		mod2 = VisCuTkModuleMap::moduleMap[geoUnit2];
	      }
	      SiStripMatchedRecHit2DCollection::DetSet::const_iterator rechitRangeIteratorBegin = rechitRange.begin();
	      SiStripMatchedRecHit2DCollection::DetSet::const_iterator rechitRangeIteratorEnd   = rechitRange.end();
	      SiStripMatchedRecHit2DCollection::DetSet::const_iterator iter=rechitRangeIteratorBegin;
	    
	      for(iter=rechitRangeIteratorBegin;iter!=rechitRangeIteratorEnd;++iter){//loop on the rechit
		SiStripMatchedRecHit2D const rechit=*iter;
		LocalPoint position=rechit.localPosition();
		if(isStereo!=0){
		  std::pair<const GeomDetUnit*, std::pair<LocalPoint, const GeomDetUnit* > > pxd;
		  pxd.first = geoUnit;
		  pxd.second.first = position;
		  pxd.second.second = geoUnit;
		  rtemp.push_back(pxd);
		  if(mod){ mod->rechit_size++; } else std::cout << "module not found " << std::endl; 
		}else{
		  std::pair<const GeomDetUnit*, std::pair<LocalPoint, const GeomDetUnit* > > pxd1;
		  pxd1.first = geoUnit1;
		  pxd1.second.first = position;
		  pxd1.second.second = geoUnit2;
		  rtemp.push_back(pxd1);
		  if(mod1){ mod1->rechit_size++; } else std::cout << "module not found " << std::endl; 
		  std::pair<const GeomDetUnit*, std::pair<LocalPoint, const GeomDetUnit*  > > pxd2;
		  pxd2.first = geoUnit2;
		  pxd2.second.first = position;
		  pxd2.second.second = geoUnit2;
		  rtemp.push_back(pxd2);
		  if(mod2){ mod2->rechit_size++; } else std::cout << "module not found " << std::endl; 
		}
	      }
	    }
	  } 
	} 
	if(rechitsrphi->size()>0){ 
	  for (SiStripRecHit2DCollection::const_iterator detunit_iterator = rechitsrphi->begin(), detunit_end = rechitsrphi->end();
	       detunit_iterator != detunit_end; ++detunit_iterator )
	  {
	    SiStripRecHit2DCollection::DetSet rechitRange = *detunit_iterator;
	    unsigned int id = rechitRange.detId();
	    if(id!=999999999){ //if is valid detector
	      DetId detIdObject(id);
	      const GeomDetUnit      * geoUnit = m_geom->idToDetUnit( detIdObject );
	      VisCuTkModule * mod = VisCuTkModuleMap::moduleMap[geoUnit];
	      SiStripRecHit2DCollection::DetSet::const_iterator rechitRangeIteratorBegin = rechitRange.begin();
	      SiStripRecHit2DCollection::DetSet::const_iterator rechitRangeIteratorEnd   = rechitRange.end();
	      SiStripRecHit2DCollection::DetSet::const_iterator iter=rechitRangeIteratorBegin;
	      for(iter=rechitRangeIteratorBegin;iter!=rechitRangeIteratorEnd;++iter){//loop on the rechit
		SiStripRecHit2D const rechit=*iter;
		LocalPoint position=rechit.localPosition();
                std::pair<const GeomDetUnit*, std::pair<LocalPoint, const GeomDetUnit* > > pxd;
		pxd.first = geoUnit;
                pxd.second.first = position;
                pxd.second.second = geoUnit;
		rtemp.push_back(pxd);
		if(mod){ mod->rechit_size++; } else std::cout << "module not found " << std::endl; 
	      }
	    }
	  } 
	} 
	if(rechitsstereo->size()>0){ 
	  for (SiStripRecHit2DCollection::const_iterator detunit_iterator = rechitsstereo->begin(), detunit_end = rechitsstereo->end();
	       detunit_iterator != detunit_end; ++detunit_iterator )
	  {
	    SiStripRecHit2DCollection::DetSet rechitRange = *detunit_iterator;
	    unsigned int id = rechitRange.detId();
	    if(id!=999999999){ //if is valid detector
	      DetId detIdObject(id);
	      const GeomDetUnit      * geoUnit = m_geom->idToDetUnit( detIdObject );
	      VisCuTkModule * mod = VisCuTkModuleMap::moduleMap[geoUnit];
	      SiStripRecHit2DCollection::DetSet::const_iterator rechitRangeIteratorBegin = rechitRange.begin();
	      SiStripRecHit2DCollection::DetSet::const_iterator rechitRangeIteratorEnd   = rechitRange.end();
	      SiStripRecHit2DCollection::DetSet::const_iterator iter=rechitRangeIteratorBegin;
	      for(iter=rechitRangeIteratorBegin;iter!=rechitRangeIteratorEnd;++iter){//loop on the rechit
		SiStripRecHit2D const rechit=*iter;
		LocalPoint position=rechit.localPosition();
                std::pair<const GeomDetUnit*, std::pair<LocalPoint, const GeomDetUnit* > > pxd;
		pxd.first = geoUnit;
                pxd.second.first = position;
                pxd.second.second = geoUnit;
		rtemp.push_back(pxd);
		if(mod){ mod->rechit_size++; } else std::cout << "module not found " << std::endl; 
	      }
	    }
	  } 
	} 
	std::cout << rtemp.size() << " rechits in sistrip " << std::endl; 
      
	std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod;
	for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
	  VisCuTkModule* mod = imod->second;
	  if(mod->rechit_size>0){
	    mod->rechit_start=rd_pointer; rd_pointer = rd_pointer + mod->rechit_size;
	    int rt = rtemp.size(); 
	    for(int i =0;i<rt;i++){
	      std::pair<const GeomDetUnit*, std::pair<LocalPoint, const GeomDetUnit* > > pxd = rtemp[i];
	      const GeomDetUnit * geomdet = pxd.first;
	      if (geomdet==mod->detUnit)
		m_rdigi.push_back(pxd.second);
	    }
	  } 
	}
      }
    }
  }
  norechits=true;
  edm::Handle<SiPixelRecHitCollection> rechitspixel;
  if(pixelrechitProducerName!="ignore"){
    std::vector< edm::Handle<SiPixelRecHitCollection> > pixelrechit_collections;
    ModuleLabelSelector sel(pixelrechitProducerName);
    event.getMany (sel, pixelrechit_collections);
    if (! pixelrechit_collections.empty ())norechits=false;
    if(norechits) std::cout << "no pixelrechits type "<<pixelrechitProducerName<<" in event" << std::endl;
    if(!norechits){
      std::vector< edm::Handle<SiPixelRecHitCollection> >::iterator i;
      i = pixelrechit_collections.begin ();
      const SiPixelRecHitCollection&rechitspixel = *(*i);
      if(rechitspixel.size()>0){
	std::vector<std::pair<const GeomDetUnit *,std::pair<LocalPoint, const GeomDetUnit* > > > rtemp;
	for (SiPixelRecHitCollection::const_iterator detunit_iterator = rechitspixel.begin(), detunit_end = rechitspixel.end();
	     detunit_iterator != detunit_end; ++detunit_iterator) 
	{
	  SiPixelRecHitCollection::DetSet rechitRange = *detunit_iterator;
	  unsigned int id = rechitRange.detId();
	  if(id!=999999999)
	  { //if is valid detector
	    DetId detIdObject(id);
            const GeomDetUnit      * geoUnit = m_geom->idToDetUnit( detIdObject );
	    VisCuTkModule * mod = VisCuTkModuleMap::moduleMap[geoUnit];
	    SiPixelRecHitCollection::DetSet::const_iterator rechitRangeIteratorBegin = rechitRange.begin();
	    SiPixelRecHitCollection::DetSet::const_iterator rechitRangeIteratorEnd   = rechitRange.end();
	    SiPixelRecHitCollection::DetSet::const_iterator iter=rechitRangeIteratorBegin;
	    
	    for(iter=rechitRangeIteratorBegin;iter!=rechitRangeIteratorEnd;++iter){//loop on the rechit
	      LocalPoint position=iter->localPosition();
              std::pair<const GeomDetUnit*, std::pair<LocalPoint, const GeomDetUnit* > > pxd;
	      pxd.first = geoUnit;
	      pxd.second.first = position;
	      pxd.second.second = geoUnit;
	      rtemp.push_back(pxd);
	      if(mod){ mod->rechit_size++; } else std::cout << "module not found " << std::endl; 
	    }
	  }
	} 
	std::cout << rtemp.size() << " pixel rechits  " << std::endl; 
	std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod;
	for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
	  VisCuTkModule* mod = imod->second;
	  unsigned int detType = mod->detUnit->geographicalId().subdetId();
	  if(detType<3 && mod->rechit_size>0){
	    mod->rechit_start=rd_pointer; rd_pointer = rd_pointer + mod->rechit_size;
	    int rt = rtemp.size(); 
	    for(int i =0;i<rt;i++){
	      std::pair<const GeomDetUnit*, std::pair<LocalPoint, const GeomDetUnit* > > pxd = rtemp[i];
	      const GeomDetUnit * geomdet = pxd.first;
	      if (geomdet==mod->detUnit)
		m_rdigi.push_back(pxd.second);
	    }
	  } 
	}
      }
    }
  }
}

SoSeparator*
VisCuTrackerRecHit::update (VisCuDetectorRep::DetectorRep type)
{

  SoSeparator *sep = new SoSeparator;
  if (! m_rdigi.empty ())
  { 
    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (1.0, 0.0, 0.0);//red
    mat->emissiveColor.setValue(1.0, 0.0, 0.0);
      
    SoDrawStyle *drawStyle = new SoDrawStyle;
    drawStyle->pointSize = 4.0;
      
    // global
    sep->addChild (mat);
    sep->addChild (drawStyle);
      
    try 
    {
      SoVertexProperty *vertices = new SoVertexProperty;
      int nVrtx = 0;
      std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod;
      for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
	VisCuTkModule* mod = imod->second;
	if(mod->rechit_size>0){
	  if(mod->isVisible()){
	    for(int idigi=mod->rechit_start;idigi!=(mod->rechit_start+mod->rechit_size);idigi++){
	      LocalPoint point = m_rdigi[idigi].first;
	      float x = m_rdigi[idigi].second->surface().toGlobal(point).x()/100;
	      float y;
	      if(type==VisCuDetectorRep::RZDet)
	      {
		x=0.0;
		y = sqrt(m_rdigi[idigi].second->surface().toGlobal(point).x()*m_rdigi[idigi].second->surface().toGlobal(point).x()
			 +m_rdigi[idigi].second->surface().toGlobal(point).y()*m_rdigi[idigi].second->surface().toGlobal(point).y()) / 100.0;
		if(m_rdigi[idigi].second->surface().toGlobal(point).y()< 0. )
		  y = - y;
	      }
	      else
		y = m_rdigi[idigi].second->surface().toGlobal(point).y()/100;
	      float z = m_rdigi[idigi].second->surface().toGlobal(point).z()/100;
	      vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
	    }
	  }//if mod is visible
	}// if rechits in module
      }//loop on modules
	  
      vertices->vertex.setNum (nVrtx);
      SoPointSet *points = new SoPointSet;
      points->vertexProperty.setValue (vertices);
      points->numPoints.setValue (nVrtx);
	  
      sep->addChild (points);
    }
    catch (...) 
    {
      std::cout << "No Rechits" << std::endl;
    }
  }
  return sep;
 
}

void VisCuTrackerRecHit::drawrechit(VisCuTkModule * mod,int nlay,QPainter *p)
{
  int x,y;
  double phi,r,dx,dy,dy1;
  double xp[4],yp[4],xp1,yp1;
  double vhbot,vhtop,vhapo;
  double rmedio[]={0.041,0.0701,0.0988,0.255,0.340,0.430,0.520,0.610,0.696,0.782,0.868,0.965,1.080};
  double xt1,yt1,xs1=0.,ys1=0.,xt2,yt2,xs2,ys2,pv1,pv2;
  unsigned int numberOfDigis = 0;
  
  if(m_firsteventloaded){
    QPolygon a =  QPolygon(4);
    phi = VisCuTkMap2D::phival(mod->posx,mod->posy);
    r = sqrt(mod->posx*mod->posx+mod->posy*mod->posy);
    vhbot = mod->width; 	  
    vhtop=mod->width; 	  
    vhapo=mod->length;
    if(nlay < 31){
      vhbot = mod->widthAtHalfLength/2.-(mod->width/2.-mod->widthAtHalfLength/2.); 	  
      vhtop=mod->width/2.; 	  
      vhapo=mod->length/2.;
      if(nlay >12 && nlay <19){
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
      for(int idigi=mod->rechit_start;idigi!=(mod->rechit_start+mod->rechit_size);idigi++){ 
	if(!m_map->separated){
          LocalPoint point = m_rdigi[idigi].first;
          float x = m_rdigi[idigi].second->surface().toGlobal(point).x()/100;
          float y = m_rdigi[idigi].second->surface().toGlobal(point).y()/100;
	  int xc = m_map->xpixel(x);
	  int yc = m_map->ypixel(y);
	  if(!m_map->acc){
	    p->setPen(QColor(255,0,255));    
	    p->setBrush(QColor(255,0,255));
	    if(!m_horizontal_view)p->drawRect(xc,yc,3,3);else p->drawRect(yc,m_map->x_win-xc,3,3); 
	  } else {
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
	} 
	numberOfDigis++;
      } //for digis 
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
      for(int idigi=mod->rechit_start;idigi!=(mod->rechit_start+mod->rechit_size);idigi++){ 
	if(!m_map->separated){
          LocalPoint point = m_rdigi[idigi].first;
          float x = m_rdigi[idigi].second->surface().toGlobal(point).x()/100;
          float y = m_rdigi[idigi].second->surface().toGlobal(point).y()/100;
          float z = m_rdigi[idigi].second->surface().toGlobal(point).z()/100;
	  int xc = m_map->xpixel(z);
	  int yc = m_map->ypixel(4.2*VisCuTkMap2D::phival(x,y));
	  if(!m_map->acc){
	    p->setPen(QColor(255,0,0));//red    
	    p->setBrush(QColor(255,0,0));
	    if(!m_horizontal_view)p->drawRect(xc,yc,3,3);else p->drawRect(yc,m_map->x_win-xc,3,3);
	  } else {
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
	    }else pixel = m_map->image->pixel(yc,xc);
	    int green = qGreen(pixel);
	    if(green > 1) green = green -20;
	    if(green < 0) green = 0;
	    if(!m_horizontal_view)m_map->image->setPixel(xc,yc,green);else m_map->image->setPixel(yc,m_map->x_win-xc,green);
	  }
	}//not separated
	numberOfDigis++;	
      }//digis
    }//endcap
    VisCuTkSubLayer* sl = mod->getOwner();
    if(sl->isStereo()&& m_map->separated)
    {
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
    if(m_map->acc){mod->value = mod->value + mod->rechit_size;mod->bufvalue = mod->bufvalue + mod->rechit_size;}
    else mod->value = mod->rechit_size; 
    if(m_map->separated){
      p->setPen(QColor( 158, 185, 225));
      int green = 255-mod->value; if (green < 0)green = 0;if(green > 255)green=255;
      p->setBrush(QColor(255,green,0));
      if(mod->value==0)p->setBrush(QColor(255,255,255));
      p->drawPolygon(a);
    }
    p->setPen(QColor(0,0,0));    
    p->setBrush(Qt::NoBrush); 
  }//firsteventloaded
}

std::string VisCuTrackerRecHit::updatetext (VisCuTkModule * mod)
{
  std::ostringstream  text;
  if(mod->rechit_size>0){
    int nrechit=0;
    text << mod->rechit_size << " rechit : --------------------------------------- <p>" ;
    for(int idigi=mod->rechit_start;idigi!=(mod->rechit_start+mod->rechit_size);idigi++){
      LocalPoint point = m_rdigi[idigi].first;
      float x = m_rdigi[idigi].second->surface().toGlobal(point).x()/100;
      float y = m_rdigi[idigi].second->surface().toGlobal(point).y()/100;
      float z = m_rdigi[idigi].second->surface().toGlobal(point).z()/100;
      text << " Number=" << nrechit++ 
	   << " Position=" << x  << ", " << y  << ", " << z << "<p>";
    }
    text <<  " -----------------------------" ;
  }
  return text.str();
}
