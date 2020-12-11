//<<<<<< INCLUDES                                                       >>>>>>
#include "VisReco/VisCustomTracker/interface/VisCuTrackerDigi.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkMap2D.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModuleMap.h"
#include "FWCore/Framework/interface/Selector.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <iostream>
#include <iomanip>
#include <QPainter>

using namespace edm;

VisCuTrackerDigi::VisCuTrackerDigi ()
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
  std::cout <<"stripdigiProducer="<< stripdigiProducerName << " stripdigiLabel=" << stripdigiLabelName << std::endl;
  std::cout <<"pixeldigiProducer="<< pixeldigiProducerName << " pixeldigiLabel=" << pixeldigiLabelName << std::endl;
}

void
VisCuTrackerDigi::setEvent (const edm::Event &event,
			    const edm::EventSetup &eventSetup)
{
  eventSetup.get<TrackerDigiGeometryRecord> ().get (m_pDD);
  
  m_sdigi.clear ();
  
  
  m_firsteventloaded=true;
  bool nodigi=true;
  int sd_pointer=0;
  if(stripdigiProducerName!="ignore"){
    std::vector< edm::Handle<edm::DetSetVector<SiStripDigi> > > s_digi_collections;
    Selector sel(ProductInstanceNameSelector(stripdigiLabelName) &&
		 ModuleLabelSelector(stripdigiProducerName));
    event.getMany (sel, s_digi_collections);
    if (! s_digi_collections.empty ())nodigi=false;
    if(nodigi)std::cout << "no digi type "<<stripdigiProducerName<<"/"<<stripdigiLabelName<< " in event" << std::endl;
    if(!nodigi){
      std::vector< edm::Handle<edm::DetSetVector<SiStripDigi> > >::iterator i;
      i = s_digi_collections.begin ();
      const edm::DetSetVector<SiStripDigi> &sinput = *(*i); 
  
      if (sinput.size())
      { 
	edm::DetSetVector<SiStripDigi>::const_iterator DSViter=sinput.begin();
	std::cout <<  sinput.size() << std::endl;
	for (; DSViter!=sinput.end();DSViter++){
	  edm::DetSet<SiStripDigi> ds = *DSViter;
	  if (ds.data.size())
	  {
	    const uint32_t& detID = ds.id;
            DetId detIdObject(detID);
	    
            const GeomDetUnit   * geoUnit = m_pDD->idToDetUnit( detIdObject );
	    edm::DetSet<SiStripDigi>::const_iterator idigi=ds.data.begin();
	    
            VisCuTkModule * mod = VisCuTkModuleMap::moduleMap[geoUnit];
            if(mod){
	      mod->digi_start=sd_pointer;
	      mod->digi_size=ds.data.size();
            } else std::cout << "module not found " << std::endl; 
	    for(;idigi!=ds.data.end();idigi++){ 
	      
	      m_sdigi.push_back(*idigi);

	    }  
            sd_pointer=sd_pointer+ds.data.size();
	  }
	}
      }
    }
  }
  
  m_mdigi.clear ();
  sd_pointer=0;
  nodigi=true;
  if(pixeldigiProducerName!="ignore"){
    std::vector< edm::Handle<edm::DetSetVector<PixelDigi> > > p_digi_collections;
    Selector sel(ProductInstanceNameSelector(pixeldigiLabelName) &&
		 ModuleLabelSelector(pixeldigiProducerName));
    event.getMany (sel, p_digi_collections);
    if (! p_digi_collections.empty ())nodigi=false;
    if(nodigi) std::cout << "no pixeldigi type "<<pixeldigiProducerName<<"/"<<pixeldigiLabelName<< " in event" << std::endl;
    if(!nodigi){
      std::vector< edm::Handle<edm::DetSetVector<PixelDigi> > >::iterator i;
      i = p_digi_collections.begin ();
      const edm::DetSetVector<PixelDigi> &pinput = *(*i); 
      if (pinput.size())
      { 
	edm::DetSetVector<PixelDigi>::const_iterator DSViter=pinput.begin();
	std::cout <<  pinput.size() << std::endl;
	for (; DSViter!=pinput.end();DSViter++){
	  edm::DetSet<PixelDigi> ds = *DSViter;
	  if (ds.data.size())
	  {
	    const uint32_t& detID = ds.id;
	    DetId detIdObject(detID);
	    const GeomDetUnit   * geoUnit = m_pDD->idToDetUnit( detIdObject );
	    edm::DetSet<PixelDigi>::const_iterator idigi=ds.data.begin();
	    VisCuTkModule * mod = VisCuTkModuleMap::moduleMap[geoUnit];
	    if(mod){
	      mod->digi_start=sd_pointer;
	      mod->digi_size=ds.data.size();
	    } else std::cout << "module not found " << std::endl; 
	    for(;idigi!=ds.data.end();idigi++){ 
	      m_mdigi.push_back(*idigi);
	    } // for digis 
	    sd_pointer=sd_pointer+ds.data.size();
	  }
	}
      }
    }
  }
}

SoSeparator*
VisCuTrackerDigi::update (VisCuDetectorRep::DetectorRep type)
{
  SoSeparator *sep = new SoSeparator; // global
  if (! m_mdigi.empty () || !m_sdigi.empty())
  {
    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (0.0, 1.0, 0.0);//green
    mat->emissiveColor.setValue(0.0, 1.0, 0.0);
      
    SoDrawStyle *drawStyle = new SoDrawStyle;
    drawStyle->pointSize = 4.0;
      
    sep->addChild (mat);
    sep->addChild (drawStyle);
      
    try {
      SoVertexProperty *vertices = new SoVertexProperty;
      int nVrtx = 0;
	
      std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod;
      for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
	VisCuTkModule* mod = imod->second;
	unsigned int detType = mod->detUnit->geographicalId().subdetId();
	if(mod->isVisible()){
	  if(detType < 3)//pixel
	  {
	    const PixelGeomDetUnit* theDet = dynamic_cast<const PixelGeomDetUnit*>(mod->detUnit);
	    const RectangularPixelTopology *theTopol = dynamic_cast<const RectangularPixelTopology *>( &(theDet->specificTopology ()));
	    for(int idigi=mod->digi_start;idigi!=(mod->digi_start+mod->digi_size);idigi++){ 
	      int row = m_mdigi[idigi].row();
	      int column =m_mdigi[idigi].column();
	      GlobalPoint pos =  mod->detUnit->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,column)));
	      float x = pos.x () / 100.0;  // cm -> m
	      float y;
	      if(type==VisCuDetectorRep::RZDet)
	      {
		x=0.0;
		y = sqrt(pos.x ()*pos.x()+pos.y()*pos.y()) / 100.0;
		if(pos.y()< 0. )y = - y;
	      }
	      else
		y = pos.y () / 100.0;  // cm -> m
	      float z = pos.z () / 100.0;  // cm -> m
		  
	      vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
	    }
	  }else {
	    for(int idigi=mod->digi_start;idigi!=(mod->digi_start+mod->digi_size);idigi++){ 
		  
	      SiStripDigi d = m_sdigi[idigi];
	      short strip = d.strip(); 
	      const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit*>(mod->detUnit);
	      const StripTopology * theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
	      GlobalPoint pos =  mod->detUnit->surface().toGlobal(theStripTopol->localPosition(strip));
	      float x = pos.x () / 100.0;  // cm -> m
	      float y;
	      if(type==VisCuDetectorRep::RZDet)
	      {
		y = sqrt(pos.x ()*pos.x()+pos.y()*pos.y()) / 100.0;
		if(pos.y()< 0. )y = - y;
	      }
	      else
		y = pos.y () / 100.0;  // cm -> m
	      float z = pos.z () / 100.0;  // cm -> m
		  
	      vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
	    }//for
	  }// else
	}//is visible
      }//module loop
      vertices->vertex.setNum (nVrtx);
      SoPointSet *points = new SoPointSet;
      points->vertexProperty.setValue (vertices);
      points->numPoints.setValue (nVrtx);
	
      sep->addChild (points);
    }
    catch (...) 
    {
      std::cout << "No PixelDigi Digis." << std::endl;
    }
  }
  return sep;
}

void VisCuTrackerDigi::drawdigi(VisCuTkModule * mod,int nlay,QPainter *p)
{
  int x,y;
  double phi,r,dx,dy,dy1;
  double xp[4],yp[4],xp1,yp1;
  double vhbot,vhtop,vhapo;
  double rmedio[]={0.041,0.0701,0.0988,0.255,0.340,0.430,0.520,0.610,0.696,0.782,0.868,0.965,1.080};
  double xt1,yt1,xs1=0.,ys1=0.,xt2,yt2,xs2,ys2,pv1,pv2;
  unsigned int numberOfDigis = 0;
  unsigned int detType = mod->detUnit->geographicalId().subdetId(); 
  const RectangularPixelTopology * theTopol = 0;
  const StripTopology * theStripTopol = 0;
  const PixelGeomDetUnit* theDet;
  const StripGeomDetUnit* theStripDet;
  if(detType<3) {//pixel
    theDet = dynamic_cast<const PixelGeomDetUnit*>(mod->detUnit);
    theTopol = dynamic_cast<const RectangularPixelTopology *>( &(theDet->specificTopology ()));
  } else { //SiStrip
    theStripDet = dynamic_cast<const StripGeomDetUnit*>(mod->detUnit);
    theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
  }
  
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
      if(detType<3) { //Pixel
	for(int idigi=mod->digi_start;idigi!=(mod->digi_start+mod->digi_size);idigi++){ 
	  if(!m_map->separated){
	    //int adc = m_mdigi[idigi].adc();    // charge
	    int row = m_mdigi[idigi].row();  
	    int column =m_mdigi[idigi].column();  
            GlobalPoint pos =  mod->detUnit->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,column)));
            float x = pos.x () / 100.0;  // cm -> m
            float y = pos.y () / 100.0;  // cm -> m
	    int xc = m_map->xpixel(x);
	    int yc = m_map->ypixel(y);
	    if(!m_map->acc){
	      p->setPen(QColor(0,255,0));    
	      p->setBrush(QColor(0,255,0));
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
	      if(!m_horizontal_view)m_map->image->setPixel(xc,yc,green); else m_map->image->setPixel(yc,m_map->x_win-xc,green);
	    }
	  } 
	  numberOfDigis++;
	} // for digis 
      } else { //SiStrip
	for(int idigi=mod->digi_start;idigi!=(mod->digi_start+mod->digi_size);idigi++){ 
	  if(!m_map->separated){
	    short strip =m_sdigi[idigi].strip();  
            GlobalPoint pos =  mod->detUnit->surface().toGlobal(theStripTopol->localPosition(strip));
	    float x = pos.x () / 100.0;  // cm -> m
	    float y = pos.y () / 100.0;  // cm -> m
	    //float z = pos.z () / 100.0;  // cm -> m
	    int xc = m_map->xpixel(x);
	    int yc = m_map->ypixel(y);
	    if(!m_map->acc){
	      p->setPen(QColor(0,255,0));    
	      p->setBrush(QColor(0,255,0));
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
      if(detType<3) {//pixel
	for(int idigi=mod->digi_start;idigi!=(mod->digi_start+mod->digi_size);idigi++){ 
	  if(!m_map->separated){
	    //int adc = m_mdigi[idigi].adc();    // charge
	    int row = m_mdigi[idigi].row();  
	    int column =m_mdigi[idigi].column();  
	    GlobalPoint pos =  mod->detUnit->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,column)));
	    float x = pos.x () / 100.0;  // cm -> m
	    float y = pos.y () / 100.0;  // cm -> m
	    float z = pos.z () / 100.0;  // cm -> m
	    int xc = m_map->xpixel(z);
	    int yc = m_map->ypixel(4.2*VisCuTkMap2D::phival(x,y));
	    if(!m_map->acc){
	      p->setPen(QColor(0,255,0));    
	      p->setBrush(QColor(0,255,0));
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
	  }//not separated
	  numberOfDigis++;	  
	}//digis
      } else { //SiStrip
	for(int idigi=mod->digi_start;idigi!=(mod->digi_start+mod->digi_size);idigi++){ 
	  if(!m_map->separated){
	    short strip =m_sdigi[idigi].strip();
	    GlobalPoint pos =  mod->detUnit->surface().toGlobal(theStripTopol->localPosition(strip));
	    float x = pos.x () / 100.0;  // cm -> m
	    float y = pos.y () / 100.0;  // cm -> m
	    float z = pos.z () / 100.0;  // cm -> m
	    int xc = m_map->xpixel(z);
	    int yc = m_map->ypixel(4.2*VisCuTkMap2D::phival(x,y));
	    if(!m_map->acc){
	      p->setPen(QColor(0,255,0));    
	      p->setBrush(QColor(0,255,0));
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
	  }//not separated
	  numberOfDigis++;	  
	}//digis
      }
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
    if(m_map->acc){mod->value = mod->value + mod->digi_size;mod->bufvalue = mod->bufvalue + mod->digi_size;    }
    else mod->value = mod->digi_size; 
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

std::string VisCuTrackerDigi::updatetext (VisCuTkModule * mod)
{
  const RectangularPixelTopology * theTopol = 0;
  const StripTopology * theStripTopol = 0;
  const PixelGeomDetUnit* theDet;
  const StripGeomDetUnit* theStripDet;
  std::ostringstream  text;
  if(mod->digi_size>0){
    unsigned int detType = mod->detUnit->geographicalId().subdetId();
    if(detType<3) {//pixel
      theDet = dynamic_cast<const PixelGeomDetUnit*>(mod->detUnit);
      theTopol = dynamic_cast<const RectangularPixelTopology *>( &(theDet->specificTopology ()));
      text << mod->digi_size << " pixel digis : ---------------------------------------------------- <p>" ;
    } else { //SiStrip
      theStripDet = dynamic_cast<const StripGeomDetUnit*>(mod->detUnit);
      theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
      text << mod->digi_size << " strip digis : --------------------------------------- <p>" ;
    }
    int nM_Sdigis=0;
    for(int idigi=mod->digi_start;idigi!=(mod->digi_start+mod->digi_size);idigi++){
      text << " Number=" <<  nM_Sdigis++ ;
      if(detType<3) {//pixel
	int row = m_mdigi[idigi].row();
	int column =m_mdigi[idigi].column();
	GlobalPoint pos =  mod->detUnit->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,column)));
	text << " Position=" << pos.x () << ", " << pos.y () << ", " << pos.z () 
	     << " Charge=" << m_mdigi[idigi].adc ()  
	     << " Row=" << row  
	     << " Column=" << column <<"<p>"  ;
      } else { //SiStrip
	short strip =m_sdigi[idigi].strip();
	GlobalPoint pos =  mod->detUnit->surface().toGlobal(theStripTopol->localPosition(strip));
	text << " Position=" << pos.x () << ", " << pos.y () << ", " << pos.z ()  
	     << " Charge=" << m_sdigi[idigi].adc ()  
	     << " Strip=" << strip<< "<p>";
      }
    }
    text << " -----------------------------" ;
  }
  return text.str();
}
