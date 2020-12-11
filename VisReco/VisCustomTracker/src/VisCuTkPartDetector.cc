#include "VisReco/VisCustomTracker/interface/VisCuTkPartDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubDetector.h"

#include <iostream>

using namespace std;

VisCuTkPartDetector::VisCuTkPartDetector(int idc, VisCuTkSubDetector* subDtc){
	idSubDtc = idc;
	subDetector = subDtc;
	int ncomponent = 0;
	visible = false;

	if(subDetector->getId()== 1){ //1=pixel
	  if(idSubDtc == 1 || idSubDtc == 3){ //1-3=encap
	    ncomponent = 3;
            //ncomponent = 2;
	  }
	  else { ncomponent = 3; } //barrel
	}
	if(subDetector->getId()== 2){ //2=inner silicon
	   if(idSubDtc == 1 || idSubDtc == 3){ //1-3=encap
	    ncomponent = 3;
	   }
	   else { ncomponent = 4; } //barrel
	}
	if(subDetector->getId()== 3){ //3=outer silicon
	   if(idSubDtc == 1 || idSubDtc == 3){ //1-3=encap
	     ncomponent = 9;
           }
	   else { ncomponent = 6; } //barrel
	}
	for(int i=1; i<(ncomponent+1); i++){
            addComponent(i,new VisCuTkLayer(i,this));	
	}
}

VisCuTkPartDetector::~VisCuTkPartDetector(){
}
string VisCuTkPartDetector::getName(){
	string name;
	if(getOwner()->getId()==1){
		if (getId()==1)name="PXE -z";
		if (getId()==2)name="PXB";
		if (getId()==3)name="PXE +z";
	}
	if(getOwner()->getId()==2){
		if (getId()==1)name="TID -z";
		if (getId()==2)name="TIB";
		if (getId()==3)name="TID +z";
	}
	if(getOwner()->getId()==3){
		if (getId()==1)name="TEC -z";
		if (getId()==2)name="TOB";
		if (getId()==3)name="TEC +z";
	}
	return name;
}

