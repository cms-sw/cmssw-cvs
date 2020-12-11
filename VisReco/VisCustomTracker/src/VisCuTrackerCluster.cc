//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "FWCore/Framework/interface/Selector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerCluster.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModuleMap.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkMap2D.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
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
#include "Inventor/nodes/SoIndexedLineSet.h"
#include <Inventor/nodes/SoVertexProperty.h>
#include <iostream>
#include <iomanip>
#include <QPainter>

using namespace edm;

VisCuTrackerCluster::VisCuTrackerCluster(void)
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
}

void
VisCuTrackerCluster::setEvent (const edm::Event &event,
			       const edm::EventSetup &eventSetup)
{
  eventSetup.get<TrackerDigiGeometryRecord> ().get (m_pDD);
  
  m_cdigi.clear();
  m_cpdigi.clear();
  m_firsteventloaded=true;
  bool nocluster=true;
  int cd_pointer=0;
  
  if(stripclusterProducerName!="ignore")
  {
    std::vector< edm::Handle<edmNew::DetSetVector<SiStripCluster> > > s_cluster_collections;
    Selector sel(ProductInstanceNameSelector(stripclusterLabelName) &&
		 ModuleLabelSelector(stripclusterProducerName));
    event.getMany (sel, s_cluster_collections);
    if (! s_cluster_collections.empty ())
      nocluster=false;
    if(nocluster)
      std::cout << "no stripclusters type "<<stripclusterProducerName<<"/"<<stripclusterLabelName<< " in event" << std::endl;
    if(!nocluster)
    {
      std::vector< edm::Handle<edmNew::DetSetVector<SiStripCluster> > >::iterator i;
      i = s_cluster_collections.begin ();
      const edmNew::DetSetVector<SiStripCluster> &cinput = *(*i);
      if (cinput.size())
      { 
	edmNew::DetSetVector<SiStripCluster>::const_iterator DSViter=cinput.begin();
	edmNew::DetSetVector<SiStripCluster>::const_iterator DSViterEnd=cinput.end();
	for (; DSViter!=DSViterEnd; ++DSViter)
	{
	  const uint32_t detID = DSViter->detId();
	  DetId detIdObject(detID);
	  const GeomDetUnit  * geoUnit = m_pDD->idToDetUnit( detIdObject );
	  VisCuTkModule * mod = VisCuTkModuleMap::moduleMap[geoUnit];
	  if(mod)
	  {
	    mod->cluster_start=cd_pointer;
	    mod->cluster_size=DSViter->size();
	  }
	  else
	    std::cout << "module not found " << std::endl; 
	  edmNew::DetSet<SiStripCluster>::const_iterator clustIt = DSViter->begin(), clustEnd = DSViter->end();
	  for ( ; clustIt != clustEnd; ++clustIt) 
	  {
	    m_cdigi.push_back(*clustIt);	      
	  }  
	  cd_pointer=cd_pointer+DSViter->size();
	}
      }
    }
  }
  
  cd_pointer=0;
  nocluster=true;
  if(pixelclusterProducerName!="ignore")
  {
    std::vector< edm::Handle<edmNew::DetSetVector<SiPixelCluster> > > p_cluster_collections;
    Selector sel(ProductInstanceNameSelector(pixelclusterLabelName) &&
		 ModuleLabelSelector(pixelclusterProducerName));
    event.getMany (sel, p_cluster_collections);
    if (! p_cluster_collections.empty ())
      nocluster=false;
    if(nocluster)
      std::cout << "no pixelcluster type "<<pixelclusterProducerName<<"/"<<pixelclusterLabelName<< " in event" << std::endl;
    if(!nocluster)
    {
      std::vector< edm::Handle<edmNew::DetSetVector<SiPixelCluster> > >::iterator i;
      i = p_cluster_collections.begin ();
      const edmNew::DetSetVector<SiPixelCluster> &cpinput = *(*i);
      if (cpinput.size())
      { 
	edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViter=cpinput.begin();
	edmNew::DetSetVector<SiPixelCluster>::const_iterator DSViterEnd=cpinput.end();
	for (; DSViter!=DSViterEnd;++DSViter)
	{
	  const uint32_t detID = DSViter->detId();
	  DetId detIdObject(detID);
	  const GeomDetUnit  * geoUnit = m_pDD->idToDetUnit( detIdObject );
	  VisCuTkModule * mod = VisCuTkModuleMap::moduleMap[geoUnit];
	  if(mod)
	  {
	    mod->cluster_start=cd_pointer;
	    mod->cluster_size=DSViter->size();
	  }
	  else
	    std::cout << "module not found " << std::endl; 
	  edmNew::DetSet<SiPixelCluster>::const_iterator clustIt = DSViter->begin(), clustEnd = DSViter->end();
	  for ( ; clustIt != clustEnd; ++clustIt) 
	  { 
	    m_cpdigi.push_back(*clustIt);
	  } // for digis 
	  cd_pointer=cd_pointer+DSViter->size();
	}
      }
    }
  }
}

SoSeparator *
VisCuTrackerCluster::update (VisCuDetectorRep::DetectorRep type)
{ 
  SoSeparator *sep = new SoSeparator; // global
  if (! m_cdigi.empty ())
  {
    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (0.0, 0.0, 1.0);//blue
    mat->emissiveColor.setValue(0.0, 0.0, 1.0);
      
    SoDrawStyle *drawStyle = new SoDrawStyle;
    drawStyle->pointSize = clusterPointSize;
      
    sep->addChild (mat);
    sep->addChild (drawStyle);
      
    try 
    {
      SoVertexProperty *vertices = new SoVertexProperty;
      SoVertexProperty *vtx = 0;
      SoIndexedLineSet *strips=0 ;
      int nVrtx = 0;
      int n = 0;
      int coord=0;
      SoMFInt32 coords;
      if(siStripClusterRepresentation=="strip"){
	vtx = new SoVertexProperty;
	strips = new SoIndexedLineSet;
      }

      std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod;
      for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
	VisCuTkModule* mod = imod->second;
	unsigned int detType = mod->detUnit->geographicalId().subdetId();
	if(mod->isVisible()){
	  if(detType < 3) //pixel
	  {
	    const PixelGeomDetUnit* theDet = dynamic_cast<const PixelGeomDetUnit*>(mod->detUnit);
	    const RectangularPixelTopology *theTopol = dynamic_cast<const RectangularPixelTopology *>( &(theDet->specificTopology ()));
	    for(int idigi=mod->cluster_start;idigi!=(mod->cluster_start+mod->cluster_size);idigi++){ 
	      SiPixelCluster d = m_cpdigi[idigi];
	      int row = d.minPixelRow(); 
	      int col = d.minPixelCol(); 
	      GlobalPoint pos =  mod->detUnit->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,col)));
	      float x = pos.x () / 100.0;  // cm -> m
	      float y;
	      if(type==VisCuDetectorRep::RZDet)
	      {
		x=0.0; 
		y = sqrt(pos.x ()*pos.x()+pos.y()*pos.y()) / 100.0;
		if(pos.y()< 0. )
		  y = - y;
	      }
	      else
		y = pos.y () / 100.0;  // cm -> m
	      float z = pos.z () / 100.0;  // cm -> m
		    
	      vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
	    }
	  } else {
	    const StripGeomDetUnit*  theStripDet = dynamic_cast<const StripGeomDetUnit*>(mod->detUnit);
	    const StripTopology * theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
	    for(int idigi=mod->cluster_start;idigi!=(mod->cluster_start+mod->cluster_size);idigi++){ 
	      SiStripCluster d = m_cdigi[idigi];
	      short strip = d.firstStrip(); 
	      GlobalPoint pos =  mod->detUnit->surface().toGlobal(theStripTopol->localPosition(strip));
	      float x = pos.x () / 100.0;  // cm -> m
	      float y;
	      if(type==VisCuDetectorRep::RZDet)
	      {
		x=0.0;
		y = sqrt(pos.x ()*pos.x()+pos.y()*pos.y()) / 100.0;
		if(pos.y()< 0. )
		  y = - y;
	      }
	      else
		y = pos.y () / 100.0;  // cm -> m

	      float z = pos.z () / 100.0;  // cm -> m
	      vertices->vertex.set1Value (nVrtx++, SbVec3f (x, y, z));
	      if(siStripClusterRepresentation=="strip"){
		//Compute strip 
		MeasurementPoint mp1((float)strip,(float)-0.5);LocalPoint lp1=theStripTopol->localPosition(mp1);
		MeasurementPoint mp2((float)strip,(float)+0.5);LocalPoint lp2=theStripTopol->localPosition(mp2);
		GlobalPoint pos1 =  mod->detUnit->surface().toGlobal(lp1);
		GlobalPoint pos2 =  mod->detUnit->surface().toGlobal(lp2);
		vtx->vertex.set1Value(n,SbVec3f(pos1.x()/100.0,pos1.y()/100.0,pos1.z()/100.0));
		coords.set1Value (coord, n);
		coord++;n++;
		vtx->vertex.set1Value(n,SbVec3f(pos2.x()/100.0,pos2.y()/100.0,pos2.z()/100.0));
		coords.set1Value (coord, n);
		coord++;n++;
		coords.set1Value (coord, SO_END_LINE_INDEX);
		coord++;
	      }//end compute strip

	    }
	  }//end of else if detType
	}//mod is visible
      }//module loop
      vertices->vertex.setNum (nVrtx);
      SoPointSet *points = new SoPointSet;
      points->vertexProperty.setValue (vertices);
      points->numPoints.setValue (nVrtx);
      sep->addChild (points);

      if(siStripClusterRepresentation=="strip"){
	strips->vertexProperty = vtx;
	strips->coordIndex = coords;
	sep->addChild (strips);
      }


    }
    catch (...) 
    {
      std::cout << "No Clusters." << std::endl;
    }
  }
  return sep;
}

void VisCuTrackerCluster::drawcluster(VisCuTkModule * mod,int nlay,QPainter *p)
{
  int x,y;
  int xc1=0,xc2=0,yc1=0,yc2=0;
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
    if(nlay < 31){ //endcap
      vhbot = mod->widthAtHalfLength/2.-(mod->width/2.-mod->widthAtHalfLength/2.); 	  
      vhtop=mod->width/2.; 	  
      vhapo=mod->length/2.;
      if(nlay >12 && nlay <19)
      {
	if(m_map->separated)
	  r = r+r;
	xp[0]=r-vhtop;yp[0]=-vhapo;
	xp[1]=r+vhtop;yp[1]=-vhapo;
	xp[2]=r+vhtop;yp[2]=vhapo;
	xp[3]=r-vhtop;yp[3]=vhapo;
      }
      else
      {
	if(m_map->separated)
	  r = r + r/3.;
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
	for(int idigi=mod->cluster_start;idigi!=(mod->cluster_start+mod->cluster_size);idigi++)
	{ 
	  if(!m_map->separated)
	  {
	    int row = m_cpdigi[idigi].minPixelRow();  
	    int col = m_cpdigi[idigi].minPixelCol();  
	    GlobalPoint pos =  mod->detUnit->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,col)));
	    float x = pos.x () / 100.0;  // cm -> m
	    float y = pos.y () / 100.0;  // cm -> m
	    int xc = m_map->xpixel(x);
	    int yc = m_map->ypixel(y);
	    if(!m_map->acc)
	    {
	      p->setPen(QColor(0,0,255));    
	      p->setBrush(QColor(0,0,255));
	      if(!m_horizontal_view)
		p->drawRect(xc,yc,3,3);
	      else
		p->drawRect(yc, m_map->x_win-xc,3,3);
	    }
	    else
	    {
              if(!m_horizontal_view)
	      {
		if(xc > (m_map->ximg -1))
		  xc = (m_map->ximg -1);
		if(yc > (m_map->yimg -1))
		  yc =(m_map->yimg-1);
	      }
	      else
	      {
		if(yc > (m_map->ximg -1))
		  yc = (m_map->ximg -1);
		if(xc > (m_map->yimg -1))
		  xc = (m_map->yimg-1);
	      }
	      QRgb pixel;
	      if(!m_horizontal_view)
	      {
		pixel = m_map->image->pixel(xc,yc);
	      }
	      else
		pixel = m_map->image->pixel(yc, m_map->x_win-xc);
	      int green = qGreen(pixel);
	      if(green > 1) green = green -20;
	      if(green < 0) green = 0;
	      if(!m_horizontal_view)
		m_map->image->setPixel(xc,yc,green);
	      else
		m_map->image->setPixel(yc, m_map->x_win-xc,green);
	    }
	  } 
	  numberOfDigis++;
	} // for digis 
      } else { //SiStrip
	for(int idigi=mod->cluster_start;idigi!=(mod->cluster_start+mod->cluster_size);idigi++)
	{ 
	  if(!m_map->separated)
	  {
	    short strip =m_cdigi[idigi].firstStrip();  
	    GlobalPoint pos =  mod->detUnit->surface().toGlobal(theStripTopol->localPosition(strip));
	    if(siStripClusterRepresentation=="strip")
	    {
	      MeasurementPoint mp1((float)strip,(float)-0.5);
	      LocalPoint lp1=theStripTopol->localPosition(mp1);
	      MeasurementPoint mp2((float)strip,(float)+0.5);
	      LocalPoint lp2=theStripTopol->localPosition(mp2);
	      GlobalPoint pos1 =  mod->detUnit->surface().toGlobal(lp1);
	      xc1 = m_map->xpixel(pos1.x()/100.);
	      yc1 = m_map->ypixel(pos1.y()/100.);
	      GlobalPoint pos2 =  mod->detUnit->surface().toGlobal(lp2);
	      xc2 = m_map->xpixel(pos2.x()/100.);
	      yc2 = m_map->ypixel(pos2.y()/100.);
            }
	    float x = pos.x () / 100.0;  // cm -> m
	    float y = pos.y () / 100.0;  // cm -> m
	    int xc = m_map->xpixel(x);
	    int yc = m_map->ypixel(y);
	    if(!m_map->acc)
	    {
	      p->setPen(QColor(0,0,255));    
	      p->setBrush(QColor(0,0,255));
	      if(!m_horizontal_view)
	      {
		p->drawRect(xc,yc,3,3);
		if(siStripClusterRepresentation=="strip")
		  p->drawLine(xc1,yc1,xc2,yc2);
	      }
	      else
	      {
		p->drawRect(yc, m_map->x_win-xc,3,3);
		if(siStripClusterRepresentation=="strip")
		  p->drawLine(yc1, m_map->x_win-xc1,yc2, m_map->x_win-xc2);
	      }
                         
	    }
	    else
	    {
              if(!m_horizontal_view)
	      {
		if(xc > (m_map->ximg -1))
		  xc = (m_map->ximg -1);
		if(yc > (m_map->yimg -1))
		  yc = (m_map->yimg-1);
	      }
	      else
	      {
		if(yc > (m_map->ximg -1))
		  yc = (m_map->ximg -1);
		if(xc > (m_map->yimg -1))
		  xc = (m_map->yimg-1);
	      }
	      QRgb pixel;
	      if(!m_horizontal_view)
	      {
		pixel = m_map->image->pixel(xc,yc);
	      }
	      else
		pixel = m_map->image->pixel(yc, m_map->x_win-xc);
	      int green = qGreen(pixel);
	      if(green > 1) green = green -20;
	      if(green < 0) green = 0;
	      if(!m_horizontal_view)
		m_map->image->setPixel(xc,yc,green);
	      else
		m_map->image->setPixel(yc, m_map->x_win-xc,green);
	    }
	  } 
	  numberOfDigis++;
	} //for digis 
      }
    }
    else
    { //barrel
      int numod;
      numod=mod->getId();if(numod>100)numod=numod-100;
      int vane = mod->getOwner()->getId();
      if(m_map->separated)
      {
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
      }
      else
      {
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
      if(detType<3)
      {//pixel
	for(int idigi=mod->cluster_start;idigi!=(mod->cluster_start+mod->cluster_size);idigi++)
	{ 
	  if(!m_map->separated)
	  {
	    int row = m_cpdigi[idigi].minPixelRow();  
	    int col = m_cpdigi[idigi].minPixelCol();  
	    GlobalPoint pos =  mod->detUnit->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,col)));
	    float x = pos.x () / 100.0;  // cm -> m
	    float y = pos.y () / 100.0;  // cm -> m
	    float z = pos.z () / 100.0;  // cm -> m
	    int xc = m_map->xpixel(z);
	    int yc = m_map->ypixel(4.2*VisCuTkMap2D::phival(x,y));
	    if(!m_map->acc)
	    {
	      p->setPen(QColor(0,0,255));    
	      p->setBrush(QColor(0,0,255));
	      if(!m_horizontal_view)
		p->drawRect(xc,yc,3,3);
	      else
		p->drawRect(yc, m_map->x_win-xc,3,3);
	    }
	    else
	    {
              if(!m_horizontal_view)
	      {
		if(xc > (m_map->ximg -1))
		  xc = (m_map->ximg -1);
		if(yc > (m_map->yimg -1))
		  yc = (m_map->yimg-1);
	      }
	      else
	      {
		if(yc > (m_map->ximg -1))
		  yc = (m_map->ximg -1);
		if(xc > (m_map->yimg -1))
		  xc = (m_map->yimg-1);
	      }
	      QRgb pixel;
	      if(!m_horizontal_view)
	      {
		pixel = m_map->image->pixel(m_map->x_win-xc,yc);
	      }
	      else
		pixel = m_map->image->pixel(yc,xc);
	      int green = qGreen(pixel);
	      if(green > 1) green = green -20;
	      if(green < 0) green = 0;
	      if(!m_horizontal_view)
		m_map->image->setPixel(xc,yc,green);
	      else
		m_map->image->setPixel(yc, m_map->x_win-xc,green);
	    }
	  }//not separated
	  numberOfDigis++;
	}//digis
      } else { //SiStrip
	for(int idigi=mod->cluster_start;idigi!=(mod->cluster_start+mod->cluster_size);idigi++)
	{ 
	  if(!m_map->separated)
	  {
	    short strip =m_cdigi[idigi].firstStrip();
	    GlobalPoint pos =  mod->detUnit->surface().toGlobal(theStripTopol->localPosition(strip));
	    float x = pos.x () / 100.0;  // cm -> m
	    float y = pos.y () / 100.0;  // cm -> m
	    float z = pos.z () / 100.0;  // cm -> m
	    int xc = m_map->xpixel(z);
	    int yc = m_map->ypixel(4.2*VisCuTkMap2D::phival(x,y));
	    if(siStripClusterRepresentation=="strip")
	    {
	      MeasurementPoint mp1((float)strip,(float)-0.5);
	      LocalPoint lp1=theStripTopol->localPosition(mp1);
	      MeasurementPoint mp2((float)strip,(float)+0.5);
	      LocalPoint lp2=theStripTopol->localPosition(mp2);
	      GlobalPoint pos1 =  mod->detUnit->surface().toGlobal(lp1);
	      xc1 = m_map->xpixel(pos1.z()/100.);
	      yc1 = m_map->ypixel(4.2*VisCuTkMap2D::phival(pos1.x()/100.,pos1.y()/100.));
	      GlobalPoint pos2 =  mod->detUnit->surface().toGlobal(lp2);
	      xc2 = m_map->xpixel(pos2.z()/100.);
	      yc2 = m_map->ypixel(4.2*VisCuTkMap2D::phival(pos2.x()/100.,pos2.y()/100.));
	      if(mod->getId() > 100 && (abs(abs(yc2-yc)-abs(yc1-yc))) > 20)
	      {
		if(abs(yc1-yc)>abs(yc2-yc))
		{
		  yc1=yc;
		  xc1=xc;
		  yc2=yc;
		  xc2=xc;
		}
		else
		{
		  yc1=yc;
		  xc1=xc;
		  yc2=yc;
		  xc2=xc;
		}
	      }
            }
	    if(!m_map->acc)
	    {
	      p->setPen(QColor(0,0,255));    
	      p->setBrush(QColor(0,0,255));
	      if(!m_horizontal_view)
	      {
		if(siStripClusterRepresentation=="strip")
		  p->drawLine(xc1,yc1,xc2,yc2);
		p->drawRect(xc,yc,3,3);
	      }
	      else
	      {
		if(siStripClusterRepresentation=="strip")
		  p->drawLine(yc1, m_map->x_win-xc1,yc2, m_map->x_win-xc2);
		p->drawRect(yc, m_map->x_win-xc,3,3);
	      }
	    }
	    else
	    {
              if(!m_horizontal_view)
	      {
		if(xc > (m_map->ximg -1))
		  xc = (m_map->ximg -1);
		if(yc > (m_map->yimg -1))
		  yc = (m_map->yimg-1);
	      }
	      else
	      {
		if(yc > (m_map->ximg -1))
		  yc = (m_map->ximg -1);
		if(xc > (m_map->yimg -1))
		  xc = (m_map->yimg-1);
	      }
	      QRgb pixel;
	      if(!m_horizontal_view)
	      {
		pixel = m_map->image->pixel(xc,yc);
	      }
	      else
		pixel = m_map->image->pixel(yc, m_map->x_win-xc);
	      int green = qGreen(pixel);
	      if(green > 1) green = green -20;
	      if(green < 0) green = 0;
	      if(!m_horizontal_view)
		m_map->image->setPixel(xc,yc,green);
	      else
		m_map->image->setPixel(yc, m_map->x_win-xc,green);
	    }
	  }//not separated
	  numberOfDigis++;
	}//digis
      }
    }//endcap
    VisCuTkSubLayer* sl = mod->getOwner();
    
    if(sl->isStereo()&& m_map->separated)
    {
      if(mod->getId()>100 )
      {
	for(int j=0;j<3;j++)
	{
	  x = m_map->xpixel(xp[j]);
	  y = m_map->ypixel(yp[j]);
	  if(!m_horizontal_view)a.setPoint(j,x,y);
	  else
	    a.setPoint(j,y, m_map->x_win-x);
	}
	if(!m_horizontal_view)
	  a.setPoint(3,x,y);
	else
	  a.setPoint(3,y, m_map->x_win-x);
      }
      else
      {
	x = m_map->xpixel(xp[2]);
	y = m_map->ypixel(yp[2]);
	if(!m_horizontal_view)
	  a.setPoint(0,x,y);
	else
	  a.setPoint(0, y, m_map->x_win-x);
	x = m_map->xpixel(xp[3]);
	y = m_map->ypixel(yp[3]);
	if(!m_horizontal_view)
	  a.setPoint(1,x,y);
	else
	  a.setPoint(1, y, m_map->x_win-x);
	x = m_map->xpixel(xp[0]);
	y = m_map->ypixel(yp[0]);
	if(!m_horizontal_view)
	  a.setPoint(2,x,y);
	else
	  a.setPoint(2, y, m_map->x_win-x);
	if(!m_horizontal_view)
	  a.setPoint(3,x,y);
	else
	  a.setPoint(3, y, m_map->x_win-x);
      }
    }
    else
    {
      for(int j=0;j<4;j++)
      {
	x = m_map->xpixel(xp[j]);
	y = m_map->ypixel(yp[j]);
	if(!m_horizontal_view)
	  a.setPoint(j,x,y);
	else
	  a.setPoint(j, y, m_map->x_win-x);
      }
    }                 
    if(m_map->acc)
    {
      mod->value = mod->value + mod->cluster_size;
      mod->bufvalue = mod->bufvalue + mod->cluster_size;
    }
    else
      mod->value = mod->cluster_size; 
    if(m_map->separated)
    {
      p->setPen(QColor( 158, 185, 225));
      int green = 255-mod->value;
      if (green < 0)
	green = 0;
      if(green > 255)
	green=255;
      p->setBrush(QColor(255,green,0));
      if(mod->value==0)
	p->setBrush(QColor(255,255,255));
      p->drawPolygon(a);
    }
    p->setPen(QColor(0,0,0));    
    p->setBrush(Qt::NoBrush); 
  }//firsteventloaded
}
std::string VisCuTrackerCluster::updatetext (VisCuTkModule * mod)
{
  const RectangularPixelTopology * theTopol = 0;
  const StripTopology * theStripTopol = 0;
  const PixelGeomDetUnit* theDet;
  const StripGeomDetUnit* theStripDet;
  std::ostringstream  text;
  
  if(mod->cluster_size>0){
    unsigned int detType = mod->detUnit->geographicalId().subdetId();
    if(detType<3) {//pixel
      theDet = dynamic_cast<const PixelGeomDetUnit*>(mod->detUnit);
      theTopol = dynamic_cast<const RectangularPixelTopology *>( &(theDet->specificTopology ()));
      text << mod->cluster_size << " pixel clusters : ---------------------------------------------------- <p>" ;
    } else { //SiStrip
      theStripDet = dynamic_cast<const StripGeomDetUnit*>(mod->detUnit);
      theStripTopol = dynamic_cast<const StripTopology *>( &(theStripDet->specificTopology ()));
      text << mod->cluster_size << " strip clusters : --------------------------------------- <p>" ;
    }
    int ncluster=0;
    for(int idigi=mod->cluster_start;idigi!=(mod->cluster_start+mod->cluster_size);idigi++){ 
      text << " Number=" << ncluster++ ;
      if(detType<3) {//pixel
	int row = m_cpdigi[idigi].minPixelRow();  
	int col = m_cpdigi[idigi].minPixelCol();  
	GlobalPoint pos =  mod->detUnit->surface().toGlobal(theTopol->localPosition(MeasurementPoint(row,col)));
	text << " Position=" << pos.x () << ", " << pos.y () << ", " << pos.z () 
	     << " Row=" << row
	     << " Column=" << col <<"<p>"  ;
      } else { //SiStrip
	short strip =m_cdigi[idigi].firstStrip();
	GlobalPoint pos =  mod->detUnit->surface().toGlobal(theStripTopol->localPosition(strip));
	text << " Position=" << pos.x () << ", " << pos.y () << ", " << pos.z ()  
	     << " ClusterSize=" << m_cdigi[idigi].amplitudes().size()
	     << " Barycenter=" << std::setprecision (5) << m_cdigi[idigi].barycenter() << "<p>";
      }
    }
    text <<  " -----------------------------" ;
  }
  return text.str();
}
