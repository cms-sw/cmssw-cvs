#include "VisReco/VisCustomTracker/interface/VisCuTkModule.h"

#include <iostream>
using namespace std;

VisCuTkModule::VisCuTkModule(int idc, VisCuTkSubLayer* subLayer){
 visible=false;
 idModule = idc;
 this->subLayer=subLayer;
 notused=true;
 name = "";
 digi_start=0; 
 digi_size=0; 
 cluster_start=0; 
 cluster_size=0; 
 rechit_start=0; 
 rechit_size=0; 
 simhit_start=0;
 simhit_size=0;
}

VisCuTkModule::~VisCuTkModule(){
}
