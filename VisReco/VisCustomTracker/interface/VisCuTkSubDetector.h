#ifndef VisCuTkSubDetector_H
#define VisCuTkSubDetector_H

#include "VisReco/VisCustomTracker/interface/VisCuTkCompositeDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkPartDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkState.h"

class VisCuCmsTracker;

class VisCuTkSubDetector : public VisCuTkCompositeDetector<VisCuTkPartDetector>,
  public VisCuTkState
{
      public:
	 VisCuTkSubDetector(int idc,VisCuCmsTracker* cmsTk);
	 virtual ~VisCuTkSubDetector();
	 VisCuCmsTracker* getOwner(){return cmsTracker; }
	 int getId(){return id; }
	 bool isVisible(){return visible;}
         void setVisible(){if(visible) visible=false; else visible=true;}
	 void setVisible(bool vis){visible=vis;}
      private:
 	 VisCuCmsTracker* cmsTracker;
	 int id;	 
	 bool visible;
};
#endif
