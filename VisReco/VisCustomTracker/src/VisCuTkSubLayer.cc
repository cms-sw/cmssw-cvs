#include "VisReco/VisCustomTracker/interface/VisCuTkSubLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkPartDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubDetector.h"

#include <iostream>

using namespace std;

VisCuTkSubLayer::VisCuTkSubLayer(int idc, VisCuTkLayer* layer){
  idSubLayer = idc;
  Layer = layer;
  int ncomponent = 0;
  visible = false;
  stereo = false;
  
  int spicchif[] ={24,24,40,56,40,56,80};
  int spicchib[] ={20,32,44,30,38,46,56,42,48,54,60,66,74};
  
  int numero_layer = 0;
  
  if(Layer->getOwner()->getId()== 2){ //barrel
    numero_layer = layer->getId()-1;
    if(Layer->getOwner()->getOwner()->getId()== 2){ //inner
      numero_layer = numero_layer+3;	   
    }
    if(Layer->getOwner()->getOwner()->getId()== 3){ //outer
      numero_layer = numero_layer+7;
    } 
    ncomponent = spicchib[numero_layer];	   
    /*if(Layer->getOwner()->getOwner()->getId()== 2){ //inner
      if(Layer->getId()==1&& (idc%2)==0)ncomponent=ncomponent-4;
      if(Layer->getId()==2&& (idc%2)!=0)ncomponent=ncomponent-4;
      if(Layer->getId()==3&& (idc%2)==0)ncomponent=ncomponent-2;
      if(Layer->getId()==4&& (idc%2)!=0)ncomponent=ncomponent-4;
      }*/
  }
  if(Layer->getOwner()->getId()!= 2){ //endcap 

    if(Layer->getOwner()->getOwner()->getId()== 1)ncomponent=24;//pixel
    else 
      ncomponent = spicchif[idc-1];	   
  }
  
  
  //cout << "FowBarModS= "<<Layer->getOwner()->getId()<<"Layer= "<<layer->getId() << "nanello= " <<idc<<" ncomponent= "<<ncomponent<<endl;
  
  for(int i=1; i<(ncomponent+1); i++){
    addComponent(i,new VisCuTkModule(i,this));
  }
  if((Layer->getOwner()->getId()== 2 && Layer->getOwner()->getOwner()->getId()!= 1 && layer->getId()<3) || (Layer->getOwner()->getId()!= 2 && Layer->getOwner()->getOwner()->getId()!= 1 && (idc == 1 || idc == 2 || idc == 5))){
    
    //cout << "FowBar= "<<Layer->getOwner()->getId()<<"Layer= "<<layer->getId() << "nanello= " <<idc<<" ncomponent= "<<ncomponent<<endl;
    stereo=true; 
    for(int i=1; i<(ncomponent+1); i++){
      addComponent((i+ncomponent),new VisCuTkModule(i+100,this));
    }
  }
  //cout << "Size= " <<components()<<endl;
}

VisCuTkSubLayer::~VisCuTkSubLayer(){
}

