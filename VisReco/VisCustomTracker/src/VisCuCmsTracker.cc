/********************************************************
*
*  VisCuCmsTracker.cc
*
* ********************************************************/

#include "VisReco/VisCustomTracker/interface/VisCuCmsTracker.h"

#include <iostream>

VisCuCmsTracker::VisCuCmsTracker(){
	visible = false;
        type=1;//by default assume it is the full CMS tracker
        for(int i=1;i<4;i++){
           addComponent(i,new VisCuTkSubDetector(i,this));
        }
}

VisCuCmsTracker::~VisCuCmsTracker(){

}

