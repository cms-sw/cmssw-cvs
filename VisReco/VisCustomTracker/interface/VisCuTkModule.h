#ifndef VisCuTkModule_H
#define VisCuTkModule_H

#include "VisReco/VisCustomTracker/interface/VisCuTkState.h"
#include <string>


class GeomDetUnit;
class VisCuTkSubLayer;

class VisCuTkModule : public VisCuTkState {
      public:
	VisCuTkModule(int idc, VisCuTkSubLayer* subLayer);
	virtual ~VisCuTkModule();
	float posx, posy, posz;
	float length, width, thickness, widthAtHalfLength;
	int value;
        int bufvalue;
	GeomDetUnit* detUnit;
	int getId(){return idModule; }
	VisCuTkSubLayer* getOwner(){return subLayer;}
	bool isVisible(){return visible;}
	void setVisible(){if(visible) visible=false; else visible=true;}
        void setVisible(bool vis){visible=vis;}
	bool notInUse(){return notused;}
        void setUsed(){notused=false;}
	std::string getName(){return name;}
	void setName(std::string s){name = s;}
        int digi_start,digi_size;
        int cluster_start,cluster_size;
        int rechit_start,rechit_size;
        int simhit_start,simhit_size;
      private:
	std::string name;
        int idModule;
        bool visible;	
        bool notused;	
	VisCuTkSubLayer* subLayer;
};
#endif
