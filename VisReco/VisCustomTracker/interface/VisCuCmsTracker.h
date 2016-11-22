#ifndef VisCuCmsTracker_H
#define VisCuCmsTracker_H

#include "VisReco/VisCustomTracker/interface/VisCuTkCompositeDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkState.h"

class VisCuCmsTracker : public VisCuTkCompositeDetector<VisCuTkSubDetector>, public VisCuTkState
{
      public:
	VisCuCmsTracker();
	virtual	~VisCuCmsTracker();
	VisCuTkSubDetector* pixel() const{
	    return VisCuTkCompositeDetector<VisCuTkSubDetector>::getComponent(1);} 
	VisCuCmsTracker* getOwner(){return this;} 
	bool isVisible(){return visible;}
	int getId(){return 1; }
	void setVisible(){if(visible) visible=false; else visible=true;}
	void setVisible(bool vis){visible=vis;}	
        int type;


      private:
        bool visible;	
};

#endif
