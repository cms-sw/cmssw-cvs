#include "VisReco/VisCustomTracker/interface/VisCuTkLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkPartDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubDetector.h"

#include <iostream>

using namespace std;

VisCuTkLayer::VisCuTkLayer(int idc, VisCuTkPartDetector* PartDtc){
	idLayer = idc;
	partDetector = PartDtc;
	int ncomponent = 0;
	visible = false;
	int id0 = 0;
	slWindow = 0;

	if(partDetector->getOwner()->getId()== 1){ //1=pixel
	  if(partDetector->getId()== 1 || partDetector->getId()== 3){ //end-cap
	    ncomponent = 7;
	  }
	  else{ncomponent = 8;} //barrel
	}	
	if(partDetector->getOwner()->getId()== 2){ //inner-silicon
	  if(partDetector->getId()== 1 || partDetector->getId()== 3){ //end-cap
	     ncomponent = 3;
	  }
	  else{ncomponent = 12;} //barrel
	}
	if(partDetector->getOwner()->getId()== 3){ //outer-silicon
	  if(partDetector->getId()== 1){ //end-cap-z
	    if (idLayer== 1) ncomponent = 4;
	    if (idLayer== 2 || idLayer== 3) ncomponent = 5;
	    if (idLayer== 4 || idLayer== 5 || idLayer== 6) ncomponent = 6;
	    if (idLayer== 7 || idLayer== 8 || idLayer== 9) ncomponent = 7;
	    id0=7-ncomponent;
	  }
	  if(partDetector->getId()== 3){ //endcap+z
	    if (idLayer== 9) ncomponent = 4;
	    if (idLayer== 8 || idLayer== 7) ncomponent = 5;
	    if (idLayer== 6 || idLayer== 5 || idLayer== 4) ncomponent = 6;
	    if (idLayer== 3 || idLayer== 2 || idLayer== 1) ncomponent = 7;
	    id0=7-ncomponent;
	  }
	  if(partDetector->getId()== 2){ //barrel
	    ncomponent = 12;
	  }
	}
	for(int i=1; i<(ncomponent+1); i++){
            addComponent(i,new VisCuTkSubLayer(id0+i,this));
        }
}
VisCuTkLayer::~VisCuTkLayer(){
}

