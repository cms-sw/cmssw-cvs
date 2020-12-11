#include "FWCore/Framework/interface/ESHandle.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkBuilder.h"
#include "VisReco/VisCustomTracker/interface/VisCuTracker.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModuleMap.h"
#include "VisReco/VisCustomTracker/interface/VisCuCmsTracker.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModule.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include <fstream>
#include <iostream>

VisCuCmsTracker* VisCuTkBuilder::tracker()
{
	return theTracker;
}

void  VisCuTkBuilder::create(){
    ntotmod = 0;
    theTracker = new VisCuCmsTracker();
}

void VisCuTkBuilder::getPos(int pix_sil,int fow_bar,int layer, int ring,int nmod,int &idetector,int &ipart,int &ilayer, int &iring,int &module)
{
  int spicchif[] ={24,24,40,56,40,56,80};
  int spicchib[] ={20,32,44,30,38,46,56,42,48,54,60,66,74};
  
  idetector = pix_sil;	
  if(idetector==2){
    if (((layer<=12 && layer>=10) || (layer<=21 && layer>=19) || (layer<=37 && layer>=34))){
      idetector=2;
    }
    else {idetector=3;}
  }
  
  ipart = fow_bar;
  ilayer = layer;
  //inner silicon encap
  if(idetector==2 && ipart==1){
    ilayer=ilayer-9;
  } 
  if(idetector==2 && ipart==3){
    ilayer=ilayer-18;
  }	 
  //pixel encap
  if(idetector==1 && ipart==1){
    ilayer=ilayer-12;
  }
  if(idetector==1 && ipart==3){
    ilayer=ilayer-15; 
  }
  //outer silicon encap
  if(idetector==3 && ipart==3){
    ilayer=ilayer-21;
  }
  //pixel barrel
  if(idetector==1 && ipart==2){
    ilayer=ilayer-30;
  }
  //inner silicon barrel
  if(idetector==2 && ipart==2){
    ilayer=ilayer-33;
  }
  //outer silicon barrel
  if(idetector==3 && ipart==2){
    ilayer=ilayer-37;
  }
  iring = ring;
  //outer silicon endcap
  if(idetector==3 && (ipart==3 || ipart==1)){
    if(layer==1 || layer==30) iring =ring-3;
    if(layer==2 || layer==29) iring =ring-2;
    if(layer==3 || layer==28) iring =ring-2;
    if(layer==4 || layer==27) iring =ring-1;
    if(layer==5 || layer==26) iring =ring-1;
    if(layer==6 || layer==25) iring =ring-1;
    if(layer==7 || layer==24) iring =ring;
    if(layer==8 || layer==23) iring =ring;
    if(layer==9 || layer==22) iring =ring;
  }
  if(idetector>1 && ipart==2 && ring < 7)iring=7-ring; //TIB,TOB ring numbering in barrel -z should be reversed
if(idetector==2 && ipart==2 && ring < 7){ if((iring%2)==0)iring=iring-1; else iring=iring+1;}//more corrections for the TIB numbering
 if(theTracker->type==2)//correction only for MTCC  
  {
if(idetector==2 && ipart==2 && ring > 6){ if((iring%2)==0)iring=iring-1; else iring=iring+1;}
  }
  module = nmod;
  if(module >100){
    if(ipart==2){ //barrel
      module=(module-100)+spicchib[layer-31];
      if(theTracker->type==2&&layer==35)module=(module-100)+spicchib[layer-30];
    }
    if(ipart!=2){//endcap
      module=(module-100)+spicchif[ring-1];
    }
  }
}


void VisCuTkBuilder::fill(const edm::ESHandle<TrackerGeometry> pDD){
  std::cout << "domenico ESHANDle " << pDD.product() << std::endl;

  fill( pDD.product() );
}

void VisCuTkBuilder::fill(const TrackerGeometry* pDD){
  int spicchif[] ={24,24,40,56,40,56,80};
  int spicchib[] ={20,32,44,30,38,46,56,42,48,54,60,66,74};
  pset=VisConfigurationService::pSet();
  if(!pset)
    {
      edm::ParameterSet p;
      VisConfigurationService *visService = new VisConfigurationService(p);
      pset = visService->pSet();
      delete visService;
    } 
  std::string saveTrackerdat = pset->getUntrackedParameter<std::string>("saveTrackerdat", "false");
   std::ofstream *  output = 0;
   if(saveTrackerdat=="true")output = new std::ofstream("tracker.dat",std::ios::out);
  // edm::ESHandle<TrackingGeometry> pDD;
  // iSetup.get<TrackerDigiGeometryRecord>().get( pDD );
  std::cout << " Geometry node for CmsDigiTracker is  "<<pDD<<std::endl;
  std::cout << " theTracker "<<theTracker->components()<<std::endl;
  
  std::vector<GeomDetUnit*>::const_iterator begin = pDD->detUnits().begin();
  std::vector<GeomDetUnit*>::const_iterator end = pDD->detUnits().end();
  std::cout <<"ntotmod="<<  pDD->detUnits().size() << std::endl;

  // Find where each module should be loaded
  buildMap(pDD);

  int iLayer=0;
  int iModule;
  int old_layer=0;
  int ring = 0;
  int nmod = 0;
  int ntotmod = 0;
  float r;
  int bar_fow = 1;
  float phi,phi1;
  float rmedioS[]={0.27665, 0.3671, 0.4474, 0.5617, 0.6768, 0.8189, 0.9907};
  float rmedioP[]={0.0623081, 0.074111,  0.0870344, 0.103416, 0.115766, 0.132728, 0.140506}; 
 
  int det_type;
  int nlay=0;
  int layer=0,subdet,leftright=0,ringno,petalno,moduleno,isStereo;
  subdet = (*begin)->geographicalId().subdetId();
  for ( ; begin != end; ++begin) {
    ntotmod++;
    subdet = (*begin)->geographicalId().subdetId();
    if(subdet==1){//pixel barrel
      layer = ((*begin)->geographicalId().rawId()>>16)&0xF;
      leftright=0;
      }
    if(subdet==3){//TIB
      layer = ((*begin)->geographicalId().rawId()>>14)&0x7;
      leftright=0;
      }
    if(subdet==5){//TOB
      layer = ((*begin)->geographicalId().rawId()>>14)&0x7;
      leftright=0;
      }
    if(subdet==2){//Pixel endcap
      leftright = ((*begin)->geographicalId().rawId()>>23)&0x3;
      layer = ((*begin)->geographicalId().rawId()>>16)&0xF;
    }
    if(subdet==4){//TID
      leftright = ((*begin)->geographicalId().rawId()>>13)&0x3;
      layer = ((*begin)->geographicalId().rawId()>>11)&0x3;
    }
    if(subdet==6){//TEC
      leftright = ((*begin)->geographicalId().rawId()>>18)&0x3;
      layer = ((*begin)->geographicalId().rawId()>>14)&0xF;
    }
    isStereo=(*begin)->geographicalId().rawId()&0x3;
    nlay = layerno(subdet,leftright,layer);
    ringno=0;
    petalno=0;
    moduleno=0;
    if(subdet==1){ringno=((*begin)->geographicalId().rawId()>>8)&0xFF;}
    if(subdet==2){ringno=((*begin)->geographicalId().rawId()>>8)&0xFF;}
    if(subdet==3){ringno=((*begin)->geographicalId().rawId()>>4)&0x3F;}
    if(subdet==5){ringno=((*begin)->geographicalId().rawId()>>5)&0x7F;}
    if(subdet==4){ringno=((*begin)->geographicalId().rawId()>>9)&0x3;}
    if(subdet==6){ringno=((*begin)->geographicalId().rawId()>>5)&0x7;}
    if(subdet==6){ petalno=((*begin)->geographicalId().rawId()>>8)&0xF;}
    if(subdet==2){ petalno=((*begin)->geographicalId().rawId()>>10)&0x3F;}
    if(subdet==1){moduleno=((*begin)->geographicalId().rawId() >>2)&0x3F; }
    if(subdet==3){moduleno=((*begin)->geographicalId().rawId() >>2)&0x3; }
    if(subdet==5){moduleno=((*begin)->geographicalId().rawId() >>2)&0x7; }
    if(subdet==4){moduleno=((*begin)->geographicalId().rawId() >>2)&0x1F; }
    if(subdet==6){moduleno=((*begin)->geographicalId().rawId() >>2)&0x7; }
    if(subdet==2){moduleno=((*begin)->geographicalId().rawId() >>2)&0x3F; }
    iLayer = nlay;
    if(iLayer !=old_layer){old_layer=iLayer;iModule=0;}
    float posx =  (*begin)->surface().position().x()/ 100.0;  // cm -> m
    float posy =  (*begin)->surface().position().y()/ 100.0;  // cm -> m
    float posz =  (*begin)->surface().position().z()/ 100.0;  // cm -> m
    bar_fow=2;
    if(nlay<16)bar_fow=1;
    if(nlay>15&&nlay<31)bar_fow=3;
    r=pow(((double)(posx*posx) + posy*posy),0.5);
    phi1=atan(posy/posx);
    phi=phi1;
    if(posy < 0.&&posx>0)phi=phi1+2.*M_PI;
    if(posx < 0.)phi=phi1+M_PI;
    if(fabs(posy)<0.000001&&posx>0)phi=0;
    if(fabs(posy)<0.000001&&posx<0)phi=M_PI;
    if(fabs(posx)<0.000001&&posy>0)phi=M_PI/2.;
    if(fabs(posx)<0.000001&&posy<0)phi=M_PI + M_PI/2.;
   
    if(bar_fow==2){ //barrel
      if(subdet==1)ring=moduleno;
      if(subdet==5){
	ring=moduleno;
	if((((*begin)->geographicalId().rawId()>>12)&0x3)==2)ring=ring+6;
      }
      if(subdet==3){
	ring=moduleno;
	if(layer==2||layer==4){
          if((((*begin)->geographicalId().rawId()>>10)&0x3)==2){ring=ring*2;} else {ring=ring*2-1;}
	}else{
	  if((((*begin)->geographicalId().rawId()>>10)&0x3)==2){ring=ring*2-1;} else {ring=ring*2;}
	}
	if((((*begin)->geographicalId().rawId()>>12)&0x3)==2)ring=ring+6;
      }

      nmod=(int)((phi/(2.*M_PI))*spicchib[nlay-31]+.1)+1;
      if(theTracker->type==2&&nlay==35)nmod=(int)((phi/(2.*M_PI))*spicchib[nlay-30]+.1)+1;
      if(nlay==40)nmod = nmod-1;
      if(subdet==1)nmod=ringno;
    } else { // endcap
      if (subdet==4||subdet==6) { //endcap silicon strip
	for (int i=0;i<7; i++){
          if (fabs(r-rmedioS[i])<0.015){
            ring =i+1;
            break;
          }
        }
        nmod=(int)((phi/(2.*M_PI))*spicchif[ring-1]+.1)+1;
      } else{ // endcap pixel
        for (int i=0; i<7; i++){
          if(fabs(r -rmedioP[i])<0.0001){
            ring=i+1;
            break;
          }
        }
        nmod=(int)((phi/(2.*M_PI))*24+.1)+1;
      }
    } //end of endcap part
    if (isStereo==1) nmod=nmod+100;

    float  length = (*begin)->surface ().bounds ().length () / 100.0; // cm -> m
    float  width = (*begin)->surface ().bounds ().width () / 100.0;   // cm -> mo
    float  thickness = (*begin)->surface ().bounds ().thickness () / 100.0;  // cm -> m
    float  widthAtHalfLength=(*begin)->surface ().bounds ().widthAtHalfLength () / 100.0;
    
    det_type=1;
    if (subdet> 2)  det_type=2;
    int id = (*begin)->geographicalId().rawId();
    getPos(det_type,bar_fow,nlay,ring,nmod,idetector,ipart,ilayer,iring,module);
   if(saveTrackerdat=="true")(*output) <<ntotmod<< " "<< det_type << " " <<bar_fow<<" "<<nlay << " " << ring << " " << nmod <<" " << posx << " " <<posy << " " <<posz<<" "<<length<<" " <<width<<" "<<thickness<<" "<<widthAtHalfLength<<" "<<" "<<id<< std::endl;
    //output <<ntotmod<<" "<< det_type<<" "<<bar_fow<<" "<<nlay<<" "<<ring<<" "<<nmod<<" "<<idetector<<" "<<ipart<<" "<<ilayer<<" "<<iring<<" "<<module<<std::endl;
    mod = mmap[(*begin)->geographicalId().rawId()];
    if(mod==0){
      std::cout << "error in filling module: idetector="<<idetector <<" ipart="<<ipart<<" ilayer="<<ilayer<<" iring="<<iring<<" module="<< module <<std::endl;
      std::cout << " ind=" <<theTracker->getComponent(idetector)->getComponent(ipart)->getComponent(ilayer)->getComponent(iring)->getComponent(module)<< std::endl;
    }
    if(!mod->notInUse()){
      std::cout << "error in VisCuTkBuilder;  trying to reload module: idetector="<<idetector <<" ipart="<<ipart<<" ilayer="<<ilayer<<" iring="<<iring<<" module="<< module <<std::endl;
      std::cout << " ind=" <<theTracker->getComponent(idetector)->getComponent(ipart)->getComponent(ilayer)->getComponent(iring)->getComponent(module)<< std::endl;
    }	
    iModule++;
    std::string mod_name1 = VisCuTracker::getModuleName((*begin)->geographicalId().rawId());
    std::string mod_name = VisCuTracker::getModuleName((*begin)->geographicalId());
    if(mod_name!=mod_name1)std::cout << mod_name << std::endl << mod_name1 << std::endl;
    if(saveTrackerdat=="true")(*output) << mod_name << std::endl;
    mod->setName(mod_name); 
    mod->posx = posx;
    mod->posy = posy;
    mod->posz = posz;
    mod->length = length;
    mod->width = width;
    mod->thickness = thickness;
    mod->widthAtHalfLength = widthAtHalfLength;
    mod->value = 0;	 
    mod->bufvalue = 0;	 
    mod->detUnit = (*begin);
    VisCuTkModuleMap::moduleMap[(*begin)]=mod;
    mod->setVisible(false);
    mod->setUsed();
  }		
  mmap.clear(); //no more needed 
  }		
void VisCuTkBuilder::buildMap(const TrackerGeometry* geom){
  //  ifstream* infile;
  infilename="CommonTools/TrackerMap/data/tracker.dat";
  int det_type, bar_fow, nlay,  ring, nmod;
  int nmods, idetector, ipart, iring, module, ilayer;
  unsigned int idex;
  float posx, posy, posz, length, width, thickness, widthAtHalfLength;
  int iModule=0,old_layer=0, ntotMod =0, number_modules;
  std::string name,dummys;
  std::ifstream infile(edm::FileInPath(infilename).fullPath().c_str(),std::ios::in);
  while(!infile.eof()) {
    infile >> nmods >> det_type >> bar_fow >> nlay  >> ring >> nmod >> posx >> posy
	   >> posz>> length >> width >> thickness
	   >> widthAtHalfLength >> idex ;
    getline(infile,dummys); //necessary to reach end of record
    getline(infile,name); 
    if(old_layer!=ilayer){old_layer=ilayer;iModule=0;}
    iModule++;
    ntotMod++;
    getPos(det_type,bar_fow,nlay,ring,nmod,idetector,ipart,ilayer,iring,module);
    mod = theTracker->getComponent(idetector)->getComponent(ipart)->getComponent(ilayer)->getComponent(iring)->getComponent(module);
    if(mod==0){
      std::cout << "error in module: idetector="<<idetector <<" ipart="<<ipart<<" ilayer="<<ilayer<<" iring="<<iring<<" module="<< module <<std::endl;
      std::cout << " ind=" <<theTracker->getComponent(idetector)->getComponent(ipart)->getComponent(ilayer)->getComponent(iring)->getComponent(module)<< std::endl;
    }
    else
      {
        mmap[idex]=mod;
      }
  }
  infile.close();
  number_modules = ntotMod-1;
  std::cout <<"ntotmod1="<<  number_modules << std::endl;
}
