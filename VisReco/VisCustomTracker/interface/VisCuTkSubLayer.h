#ifndef VisCuTkSubLayer_H
#define VisCuTkSubLayer_H

#include "VisReco/VisCustomTracker/interface/VisCuTkCompositeDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModule.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkState.h"

class VisCuTkLayer;

class VisCuTkSubLayer : public VisCuTkCompositeDetector<VisCuTkModule>, public VisCuTkState
{
      public:
	 VisCuTkSubLayer(int idc, VisCuTkLayer* layer);
	 virtual ~VisCuTkSubLayer();
	 VisCuTkLayer* getOwner(){return Layer; }
	 int getId(){return idSubLayer; }
	 bool isVisible(){return visible;}
	 bool isStereo(){return stereo;}
 	 void setVisible(){if(visible) visible=false; else visible=true;}
	void setVisible(bool vis){visible=vis;}
      private:
         VisCuTkLayer* Layer;
         int idSubLayer;
         bool visible;
         bool stereo;
};
#endif
