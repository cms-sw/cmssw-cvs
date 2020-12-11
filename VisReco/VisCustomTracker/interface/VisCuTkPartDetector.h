#ifndef VisCuTkPartDetector_H
#define VisCuTkPartDetector_H

#include "VisReco/VisCustomTracker/interface/VisCuTkCompositeDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkState.h"
#include <string> 


class VisCuTkSubDetector;

class VisCuTkPartDetector : public VisCuTkCompositeDetector<VisCuTkLayer>, public VisCuTkState
{
      public:
	 VisCuTkPartDetector(int idc,VisCuTkSubDetector* subDtc );
	 virtual ~VisCuTkPartDetector();
	 VisCuTkSubDetector* getOwner(){return subDetector; }
	 int getId(){return idSubDtc; } 
	 bool isVisible(){return visible;}
 	 void setVisible(){if(visible) visible=false; else visible=true;}	
         void setVisible(bool vis){visible=vis;}
         std::string getName();
      private:
	 VisCuTkSubDetector* subDetector;
	 int idSubDtc;
	 bool visible;
};
#endif
