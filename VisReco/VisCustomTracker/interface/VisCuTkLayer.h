#ifndef Vis_CuTkLayer_H
#define Vis_CuTkLayer_H

#include "VisReco/VisCustomTracker/interface/VisCuTkCompositeDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkState.h"

class VisCuTkPartDetector;
class VisCuTkLayerSlWindow;

class VisCuTkLayer : public VisCuTkCompositeDetector<VisCuTkSubLayer>, public VisCuTkState
{
     public:
	VisCuTkLayer(int idc, VisCuTkPartDetector* partDtc);
	virtual ~VisCuTkLayer();
	VisCuTkPartDetector* getOwner(){return partDetector;}
	int getId(){return idLayer; }
	bool isVisible(){return visible;}
	void setVisible(){if(visible) visible=false; else visible=true;}
	void setVisible(bool vis){visible=vis;}	
	VisCuTkLayerSlWindow * getSlWindow(){return slWindow;}
	void setSlWindow( VisCuTkLayerSlWindow * sl){slWindow=sl;}

     private:
        int idLayer;
        VisCuTkPartDetector* partDetector;
        VisCuTkLayerSlWindow* slWindow;
	bool visible;
};
#endif
