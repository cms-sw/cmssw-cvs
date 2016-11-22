#include "VisReco/VisCustomTracker/interface/VisCuTkSubDetector.h"

VisCuTkSubDetector::VisCuTkSubDetector(int idc, VisCuCmsTracker* cmsTk){
      
      id = idc;
      cmsTracker = cmsTk;      
      visible = false;
      
      for(int i=1;i<4;i++){
         addComponent(i,new VisCuTkPartDetector(i,this));
      }
}

VisCuTkSubDetector::~VisCuTkSubDetector(){
}

