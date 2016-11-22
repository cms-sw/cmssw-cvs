#ifndef VisCuTkBuilder_H
#define VisCuTkBuilder_H

#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
class VisCuTkModule;
class VisCuCmsTracker;
class EventSetup;
class TrackerGeometry;
class VisCuTkBuilder
{
 public:
   void create();
   VisCuCmsTracker* tracker();
   void fill(const TrackerGeometry* m_pDD);
   void fill(const edm::ESHandle<TrackerGeometry>  pDD); //@@ only for backword compatibility
   int layerno(int subdet,int leftright,int layer){
        if(subdet==6&&leftright==1)return(10-layer);
        if(subdet==6&&leftright==2)return(layer+21);
        if(subdet==4&&leftright==1)return(4-layer+9);
        if(subdet==4&&leftright==2)return(layer+18);
        if(subdet==2&&leftright==1)return(4-layer+12);
        if(subdet==2&&leftright==2)return(layer+15);
        if(subdet==1)return(layer+30);
        if(subdet==3)return(layer+33);
        if(subdet==5)return(layer+37);
   return -1;
   }

 private:
    VisCuTkModule* mod;
    VisCuCmsTracker* theTracker;
    int idetector, ipart, ilayer, iring, module;
    void getPos(int pix_sil,int fow_bar,int layer,int ring,int nmod,int &idetector,int &ipart,int &ilayer, int &iring,int &module);
    float oldz;
    float posx, posy;
    int ntotmod;
    const edm::ParameterSet *pset;
    void buildMap(const TrackerGeometry* pDD);
    typedef std::map<int, VisCuTkModule*> moduleMap;
    moduleMap mmap;
    std::string infilename;
};
#endif
