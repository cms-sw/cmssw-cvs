//<<<<<< INCLUDES                                                       >>>>>>
#include "VisFramework/VisFrameworkBase/interface/debug.h"
                  
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "VisReco/VisCustomTracker/interface/VisCuDetectorRep.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkGeometryTwig.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkMapWindow.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSlWindow.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayerSlWindow.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkBuilder.h"
#include "VisReco/VisCustomTracker/interface/VisCuCmsTracker.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModuleMap.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModule.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerDigi.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerCluster.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerSimHit.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerRecHit.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include <Inventor/nodes/SoSeparator.h>

#include <iostream>
#include <iomanip>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace edm;

VisCuTkGeometryTwig::VisCuTkGeometryTwig(IgState *state, IgTwig *parent,
					 const std::string &name /* = "" */)
  : VisQueuedTwig(state, parent, name),
    m_TrackerGeometry(0),
    m_cacheID_geo(0),
    m_cacheID_cab(0),
    m_name (name),
    m_done (false),
    m_3Ddone (false),
    alreadySelected (false),
    m_rzdone (false),
    m_rphidone (false),
    tr(0),
    tkMap(0),
    winTkMap(0),
    winTkSl(0),
    timesDispatchChangesCalled(0),
    nevToIntegrate(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    autoIntegrate(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    signalToIntegrate(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    accessingDb(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    crossedModulesOnly(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    trackProducerName(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    xSize(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    ySize(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    simhitProducer(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    striprechitProducer(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    pixelrechitProducer(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    pixeldigiProducer(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    pixeldigiLabel(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    stripdigiProducer(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    stripdigiLabel(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    pixelclusterProducer(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    pixelclusterLabel(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    stripclusterProducer(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    stripclusterLabel(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    stripclusterPointSize(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    stripcluster2dRep(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::dispatchChanges)),
    m_track(state, lat::CreateCallback (this, &VisCuTkGeometryTwig::configChanged))
{
  digidraw=0;
  clusterdraw=0;
  rechitdraw=0;
  simhitdraw=0;
  pset=VisConfigurationService::pSet();
  if(!pset)
  {
    edm::ParameterSet p;
    VisConfigurationService *visService = new VisConfigurationService(p);
    pset = visService->pSet();
    delete visService;
  } 
}

VisCuTkGeometryTwig::~VisCuTkGeometryTwig(void) 
{
  if(m_3Ddone)
  {
    m_3dDet->unref();
    digi3d->unref();
    cluster3d->unref();
    rechit3d->unref();
    simhit3d->unref();
  }
  if(m_rzdone)
    m_rzDet->unref();
  if(m_rphidone)
    m_rphiDet->unref();
}

void
VisCuTkGeometryTwig::onNewEvent (const edm::Event &event,
				 const edm::EventSetup &eventSetup)
{
  LOG (1, trace, LFfwvis, "VisCuTkGeometryTwig[" << name ()
       << "]::onNewEvent()\n");
  
  //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  //Code modified by Domenico to 
  // A: make faster the display
  // B: allow display of only active modules
  // 2007-01-25
  // domenico.giordano@cern.ch
  
  trackLabelName = pset->getUntrackedParameter<std::string>("trackLabel", "");
  
  eventSetup.get<TrackerDigiGeometryRecord> ().get (m_pDD);
  if(timesDispatchChangesCalled>0)dispatchChanges();
  
  if (!accessingDb.value()){
    m_TrackerGeometry = const_cast<TrackerGeometry*> (m_pDD.product()); 
  }
  else{
    if (m_cacheID_cab != eventSetup.get<SiStripDetCablingRcd>().cacheIdentifier() 
	|| 
	m_cacheID_geo != eventSetup.get<TrackerDigiGeometryRecord>().cacheIdentifier()){
      m_done=false;
      m_3Ddone=false;
      m_rzdone=false;
      m_rphidone=false;
      if(tkMap!=0 ){ 
	// Kill all open selection windows 
	for(unsigned int i=0; i<tr->components(); i++){
	  VisCuTkSubDetector* subDetector = tr->getComponent(i+1);
	  for(unsigned int j=0; j<subDetector->components(); j++ ){
	    VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
	    for(unsigned int k=0; k<partDetector->components(); k++ ){
	      VisCuTkLayer* layer = partDetector->getComponent(k+1);
	      if(layer->getSlWindow()!=0 )delete layer->getSlWindow();
	    }
	  }
	}
        delete tr;
	//delete tkMap; 
      }
      tkMap=0; 
      //if(digidraw!=0)delete digidraw; 
      digidraw=0;
      //if(clusterdraw!=0)delete clusterdraw; 
      clusterdraw=0;
      //if(rechitdraw!=0)delete rechitdraw; 
      rechitdraw=0;
      //if(simhitdraw!=0)delete simhitdraw; 
      simhitdraw=0;            
      VisCuTkModuleMap::moduleMap.clear(); //@ clear previous moduleMap
      
      m_cacheID_cab = eventSetup.get<SiStripDetCablingRcd>().cacheIdentifier();
      m_cacheID_geo = eventSetup.get<TrackerDigiGeometryRecord>().cacheIdentifier();	
     
      eventSetup.get<SiStripDetCablingRcd>().get( m_SiStripDetCabling );
      std::vector<uint32_t> vdetId;
      m_SiStripDetCabling->addActiveDetectorsRawIds(vdetId);
      //for (size_t i=0;i<vdetId.size();i++)
      //std::cout << "Active DetIds from cabling " << vdetId[i] << std::endl;
      
      if (m_TrackerGeometry!=0)
	delete m_TrackerGeometry;

      m_TrackerGeometry = new TrackerGeometry;
      for (TrackingGeometry::DetUnitContainer::const_iterator idet = m_pDD->detUnits().begin();
	   idet != m_pDD->detUnits().end(); idet++){
	//std::cout << "DetIds from geometry " <<  (*idet)->geographicalId().rawId() << std::endl;
	if (binary_search(vdetId.begin(),vdetId.end(),(*idet)->geographicalId().rawId())){
	  m_TrackerGeometry->addDetUnit(*idet);
	  //std::cout << "added detid " << (*idet)->geographicalId().rawId() << std::endl;
	}
      }
    }
  }
     
  if (! m_done)
  {
    VisQueuedTwig::onBaseInvalidate ();
	
    if(tkMap==0){
      tkMap= new VisCuTkBuilder();
      tkMap->create();
      //tkMap->fill(m_pDD); //@@ This method is now substituded by fill(const TrackerGeometry*)
      tkMap->fill(m_TrackerGeometry);
      tr = tkMap->tracker();
      ASSERT (tr);
      std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod;
      for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
	VisCuTkModule* mod = imod->second;
	mod->digi_start=0;
	mod->digi_size=0;
	mod->cluster_start=0;
	mod->cluster_size=0;
	mod->rechit_start=0;
	mod->rechit_size=0;
	mod->simhit_start=0;
	mod->simhit_size=0;
      }
      if(winTkMap!=0){
	winTkMap->setTracker(tr);
	winTkMap->setEvent(event,eventSetup);
	std::cout << "winTkMap updated "<<std::endl; 
      }
      if(winTkSl!=0){
	winTkSl->setTracker(tr);
	winTkSl->setMap(winTkMap->map);
	winTkSl->drawall();
	std::cout << "winTkSl update "<<std::endl; 
	if(tr->type==2)winTkSl->drawall();//if MTCC layout draw all modules by default
      }
    }
    if(digidraw==0){
      digidraw = new VisCuTrackerDigi();
      digidraw->setTracker(tr);
      dispatchChanges();
      digidraw->setEvent(event,eventSetup);
      std::cout << "digidraw created "<<std::endl; 
    }
    if(clusterdraw==0){
      clusterdraw = new VisCuTrackerCluster();
      clusterdraw->setTracker(tr);
      dispatchChanges();
      clusterdraw->setEvent(event,eventSetup);
      std::cout << "clusterdraw created "<<std::endl; 
    }
    if(rechitdraw==0){
      rechitdraw = new VisCuTrackerRecHit();
      rechitdraw->setTracker(tr);
      dispatchChanges();
      rechitdraw->setEvent(event,eventSetup);
      std::cout << "rechitdraw created "<<std::endl; 
    }
    if(simhitdraw==0){
      simhitdraw = new VisCuTrackerSimHit();
      simhitdraw->setTracker(tr);
      dispatchChanges();
      simhitdraw->setEvent(event,eventSetup);
      std::cout << "simhitdraw created "<<std::endl; 
    }
    if(winTkMap==0){
      winTkMap = new VisCuTkMapWindow(0,0,0, this, simhitdraw, digidraw, clusterdraw, rechitdraw);
      winTkMap->setTracker(tr);
      dispatchChanges();
      winTkMap->setEvent(event,eventSetup);
      winTkMap->setGeometry(500,0,2000,1200);
      ASSERT (winTkMap);
      std::cout << "winTkMap created "<<std::endl; 
      winTkMap->show();
    }
    if(winTkSl==0){
      winTkSl = new VisCuTkSlWindow(0,0,0,this);
      winTkSl->setTracker(tr);
      winTkSl->setMap(winTkMap->map);
      ASSERT (winTkSl);
      std::cout << "winTkSl created "<<std::endl; 
      winTkSl->show();
      if(tr->type==2)winTkSl->drawall();//if MTCC layout draw all modules by default
      if(autoIntegrate.value()){winTkSl->drawall(); winTkMap->drawall(); winTkMap->separate(); winTkMap->integSign(); 
	if(signalToIntegrate.value()=="digi")winTkMap->drawDigi();else winTkMap->drawCluster();} 
    }
    previous_event = event.id().event();
    m_done = true;
    storeTracks(event);
    IgRepSet::invalidate (this, SELF_MASK);
  }else {
    int pe = event.id().event();
    if(previous_event!=pe){
      previous_event = pe;
      std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod;
      for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
	VisCuTkModule* mod = imod->second;
	mod->digi_start=0;
	mod->digi_size=0;
	mod->cluster_start=0;
	mod->cluster_size=0;
	mod->rechit_start=0;
	mod->rechit_size=0;
	mod->simhit_start=0;
	mod->simhit_size=0;
      }
      //Draw only crossed modules
      if(tr->type==3)winTkSl->notifyEvent();//clear everything from previous event
      storeTracks(event);
      if(tr->type==3) selectCrossedModules();
      if(digidraw!=0) digidraw->setEvent(event,eventSetup);
      if(clusterdraw!=0) clusterdraw->setEvent(event,eventSetup);
      if(rechitdraw!=0) rechitdraw->setEvent(event,eventSetup);
      if(simhitdraw!=0) simhitdraw->setEvent(event,eventSetup);
      if(winTkMap!=0) winTkMap->setEvent(event,eventSetup);
      IgRepSet::invalidate (this, SELF_MASK);
    }//if new event
  }
}

void
VisCuTkGeometryTwig::update (IgTextRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  
  if (! m_done) return;
  if (! m_pDD.isValid ()) return;
  if(tr==0) return;
  int count=0;
  std::map<const GeomDetUnit * , VisCuTkModule *>::iterator imod; 
  for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
    VisCuTkModule* mod = imod->second;
    if(mod->isVisible()&&!mod->notInUse())count++;
  } 
  // Prepare property description.
  std::ostringstream  text;
  
  text << "<b>CMS Tracker Geometry: </b>";
  
  text << m_name << "<br>";
  
  text << " Geometry node for CmsDigiTracker is  " << &(*m_pDD) << "<br>";
  
  for (imod=VisCuTkModuleMap::moduleMap.begin();imod != VisCuTkModuleMap::moduleMap.end(); imod++){
    VisCuTkModule* mod = imod->second;
    if(mod->isVisible()&&!mod->notInUse())
    {
      Surface::GlobalPoint pos =  mod->detUnit->surface ().position ();
      text << "<br>" <<mod->getName() << "<br>";
      if(winTkMap!=0&&winTkMap->drawdigi){
	std::string  signaltext;
	signaltext = digidraw->updatetext(mod);
	text << signaltext;}
      if(winTkMap!=0&&winTkMap->drawcluster){
	std::string  signaltext;
	signaltext = clusterdraw->updatetext(mod);
	text << signaltext;}
      if(winTkMap!=0&&winTkMap->drawrechit){
	std::string  signaltext;
	signaltext = rechitdraw->updatetext(mod);
	text << signaltext;}
      if(winTkMap!=0&&winTkMap->drawsimhits){
	std::string  signaltext;
	signaltext = simhitdraw->updatetext(mod);
	text << signaltext;}
    }
  }
  // Send it over.
  rep->setText (text.str ());
}

void
VisCuTkGeometryTwig::update (Ig3DRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  if (! m_done) return;
  if (! m_pDD.isValid ()) return;
  if(tr->type!=3)alreadySelected=false;
  else { if(!alreadySelected) {alreadySelected=true;
	   for (std::vector<VisCuTkModule *>::iterator imod = selectedModules.begin(); imod != selectedModules.end(); ++imod)
	   {  
	     VisCuTkModule * mod = (*imod);
	     if (winTkSl!=0)if(!mod->isVisible())mod->setVisible();
	   }
    }
  }
  rep->clear ();
  m_3dDet=0;digi3d=0;cluster3d=0;rechit3d=0;simhit3d=0;
  m_3dDet = buildDet (VisCuDetectorRep::FullDet, m_TrackerGeometry, VisCuTracker::subDetector (m_name)); 
  if(winTkMap!=0&&winTkMap->drawdigi)digi3d = digidraw->update(VisCuDetectorRep::FullDet);
  if(winTkMap!=0&&winTkMap->drawcluster)cluster3d = clusterdraw->update(VisCuDetectorRep::FullDet);
  if(winTkMap!=0&&winTkMap->drawrechit)rechit3d = rechitdraw->update(VisCuDetectorRep::FullDet);
  if(winTkMap!=0&&winTkMap->drawsimhits)simhit3d = simhitdraw->update(VisCuDetectorRep::FullDet);
  if(!m_3Ddone){
    m_3dDet->ref();
    if(winTkMap!=0&&winTkMap->drawdigi)digi3d->ref ();
    if(winTkMap!=0&&winTkMap->drawcluster)cluster3d->ref ();
    if(winTkMap!=0&&winTkMap->drawrechit)rechit3d->ref ();
    if(winTkMap!=0&&winTkMap->drawsimhits)simhit3d->ref ();
    m_3Ddone = true;
  }
  if(m_3dDet!=0)rep->node ()->addChild (m_3dDet);
  if(winTkMap!=0&&winTkMap->drawdigi&&digi3d!=0)rep->node ()->addChild (digi3d);
  if(winTkMap!=0&&winTkMap->drawcluster&&cluster3d!=0)rep->node ()->addChild (cluster3d);
  if(winTkMap!=0&&winTkMap->drawrechit&&rechit3d!=0)rep->node ()->addChild (rechit3d);
  if(winTkMap!=0&&winTkMap->drawsimhits&&simhit3d!=0)rep->node ()->addChild (simhit3d);
}

void
VisCuTkGeometryTwig::update (IgRPhiRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  
  if (! m_done) return;
  if (! m_pDD.isValid ()) return;
  
  rep->clear ();
  m_rphiDet=0;digi3d=0;cluster3d=0;rechit3d=0;simhit3d=0;
  m_rphiDet = buildDet (VisCuDetectorRep::RPhiDet, m_TrackerGeometry, VisCuTracker::subDetector (m_name)); 
  if(winTkMap!=0&&winTkMap->drawdigi)digi3d = digidraw->update(VisCuDetectorRep::RPhiDet);
  if(winTkMap!=0&&winTkMap->drawcluster)cluster3d = clusterdraw->update(VisCuDetectorRep::RPhiDet);
  if(winTkMap!=0&&winTkMap->drawrechit)rechit3d = rechitdraw->update(VisCuDetectorRep::RPhiDet);
  if(winTkMap!=0&&winTkMap->drawsimhits)simhit3d = simhitdraw->update(VisCuDetectorRep::RPhiDet); 
  if (! m_rphidone)
  {
    m_rphiDet->ref ();
    if(winTkMap!=0&&winTkMap->drawdigi)digi3d->ref ();
    if(winTkMap!=0&&winTkMap->drawcluster)cluster3d->ref ();
    if(winTkMap!=0&&winTkMap->drawrechit)rechit3d->ref ();
    if(winTkMap!=0&&winTkMap->drawsimhits)simhit3d->ref ();
    m_rphidone = true;
  }   
  if(m_3dDet!=0)rep->node ()->addChild (m_rphiDet);
  if(winTkMap!=0&&winTkMap->drawdigi&&digi3d!=0)rep->node ()->addChild (digi3d);
  if(winTkMap!=0&&winTkMap->drawcluster&&cluster3d!=0)rep->node ()->addChild (cluster3d);
  if(winTkMap!=0&&winTkMap->drawrechit&&rechit3d!=0)rep->node ()->addChild (rechit3d);
  if(winTkMap!=0&&winTkMap->drawsimhits&&simhit3d!=0)rep->node ()->addChild (simhit3d);
}


void
VisCuTkGeometryTwig::update (IgRZRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);
  
  if (! m_done) return;
  if (! m_pDD.isValid ()) return;
  
  rep->clear ();
  m_rzDet=0;digi3d=0;cluster3d=0;rechit3d=0;simhit3d=0;
  m_rzDet = buildDet (VisCuDetectorRep::RZDet, m_TrackerGeometry, VisCuTracker::subDetector (m_name)); 
  if(winTkMap!=0&&winTkMap->drawdigi)digi3d = digidraw->update(VisCuDetectorRep::RZDet);
  if(winTkMap!=0&&winTkMap->drawcluster)cluster3d = clusterdraw->update(VisCuDetectorRep::RZDet);
  if(winTkMap!=0&&winTkMap->drawrechit)rechit3d = rechitdraw->update(VisCuDetectorRep::RZDet);
  if(winTkMap!=0&&winTkMap->drawsimhits)simhit3d = simhitdraw->update(VisCuDetectorRep::RZDet); 
  
  if (! m_rzdone)
  {
    m_rzDet->ref ();
    if(winTkMap!=0&&winTkMap->drawdigi)digi3d->ref ();
    if(winTkMap!=0&&winTkMap->drawcluster)cluster3d->ref ();
    if(winTkMap!=0&&winTkMap->drawrechit)rechit3d->ref ();
    if(winTkMap!=0&&winTkMap->drawsimhits)simhit3d->ref ();
    m_rzdone = true;
  }
  if(m_3dDet!=0)rep->node ()->addChild (m_rzDet);
  if(winTkMap!=0&&winTkMap->drawdigi&&digi3d!=0)rep->node ()->addChild (digi3d);
  if(winTkMap!=0&&winTkMap->drawcluster&&cluster3d!=0)rep->node ()->addChild (cluster3d);
  if(winTkMap!=0&&winTkMap->drawrechit&&rechit3d!=0)rep->node ()->addChild (rechit3d);
  if(winTkMap!=0&&winTkMap->drawsimhits&&simhit3d!=0)rep->node ()->addChild (simhit3d);
}
void
VisCuTkGeometryTwig::dispatchChanges (void)
{ 
  timesDispatchChangesCalled++;
  if(timesDispatchChangesCalled<=7) return;
  if(winTkMap!=0){
    winTkMap->map->nevToIntegrate=nevToIntegrate.value();
    winTkMap->map->xsize=xSize.value();
    winTkMap->map->ysize=ySize.value();
  }
  if(digidraw!=0){
    digidraw->pixeldigiProducerName=pixeldigiProducer.value();
    digidraw->pixeldigiLabelName=pixeldigiLabel.value();
    digidraw->stripdigiProducerName=stripdigiProducer.value();
    digidraw->stripdigiLabelName=stripdigiLabel.value();
  }
  if(clusterdraw!=0){
    clusterdraw->pixelclusterProducerName=pixelclusterProducer.value();
    clusterdraw->pixelclusterLabelName=pixelclusterLabel.value();
    clusterdraw->stripclusterProducerName=stripclusterProducer.value();
    clusterdraw->stripclusterLabelName=stripclusterLabel.value();
    clusterdraw->clusterPointSize=stripclusterPointSize.value();
    clusterdraw->siStripClusterRepresentation=stripcluster2dRep.value();
  }
  if(rechitdraw!=0){
    rechitdraw->striprechitProducerName=striprechitProducer.value();
    rechitdraw->pixelrechitProducerName=pixelrechitProducer.value();
  }
  if(simhitdraw!=0){
    simhitdraw->simhitProducerName=simhitProducer.value();
  }
  //if(winTkSl!=0)winTkSl->crossedModulesOnly=crossedModulesOnly.value();
}

void
VisCuTkGeometryTwig::configChanged (void)
{ 
  if(tkMap==0 ||winTkSl==0)return;
  selectCrossedModules();
  winTkSl->repaintAll();
  //IgRepSet::invalidate (this, SELF_MASK);
}

void
VisCuTkGeometryTwig::selectCrossedModules ()
{ 
  if(tkMap==0 ||winTkSl==0)return;
  if(crossedModulesOnly.value() && tr->type==3){
    //clear everything
    winTkSl->notifyEvent();
    selectedModules.clear();
    unsigned int tracks_size = m_tracks.size ();
    int nselected = 0;
    if (tracks_size > 0) 
    {
      for (unsigned int nt = 0; nt < tracks_size; nt++)
      {
	if (m_track.value() < 0 || m_track.value() == (int)nt){
	  std::vector<VisCuTkModule * > myModules = m_trackModules[nt];
	  for (std::vector<VisCuTkModule *>::iterator imod = myModules.begin(); imod != myModules.end(); ++imod)
	  {
	    VisCuTkModule * mod = (*imod);
	    nselected++; 
	    if (winTkSl!=0)if(!mod->isVisible())mod->setVisible();
	  }

	}
      }
      if(m_track.value() >= 0)std::cout<<nselected<<" modules hit by track" << m_track.value() <<std::endl;else std::cout<<nselected<<" modules hit by tracks" << std::endl;
    }//tracks.size
  }//crossedmodulesonly
}

void
VisCuTkGeometryTwig::storeTracks(const edm::Event &event)

{
  m_tracks.clear ();
  m_trackModules.clear ();
  if(crossedModulesOnly.value()){
    bool notracks=true;
    edm::Handle<reco::TrackCollection>  trackCollection;
    try
    {
      event.getByLabel(trackProducerName.value(),trackLabelName,trackCollection);
      notracks=false;
    }
    catch (...)
    {
      std::cout << "no tracks type"<<trackProducerName.value()<<"/"<<trackLabelName<< " in event" << std::endl;
    }
    if(!notracks){
      int nt = 0;
      for (reco::TrackCollection::const_iterator track = trackCollection->begin (); track != trackCollection->end (); track++)
      {
	m_tracks.push_back (*track);
	std::vector<VisCuTkModule * > myModules;
	for (trackingRecHit_iterator it = track->recHitsBegin ();  it != track->recHitsEnd (); it++)
	{
	  if (!((*it)->geographicalId ().null ())) {
	    VisCuTkModule * mod;
	    DetId detIdObject  = (*it)->geographicalId ();
	    unsigned int id = detIdObject.rawId();
	    const GeomDet *gd = dynamic_cast<const GeomDet*>(m_pDD->idToDet(detIdObject));
	    if(gd !=0){
	      std::vector<const GeomDet*> comp = gd->components();
	      if(comp.empty()){
		//case 1 - only 1 module associated
		const GeomDetUnit *  geoUnit = dynamic_cast<const GeomDetUnit*>(m_pDD->idToDetUnit( detIdObject ));
		if(geoUnit!=0){
		  mod = VisCuTkModuleMap::moduleMap[geoUnit];
		  myModules.push_back(mod);
		}
	      }
	      if(comp.size()==2){
		// case 2 - stereo pair associated
		const GeomDetUnit *  geoUnit1 = dynamic_cast<const GeomDetUnit*>(comp[0]);
		if(geoUnit1!=0){
		  mod = VisCuTkModuleMap::moduleMap[geoUnit1];
		  myModules.push_back(mod);
		}
		const GeomDetUnit *  geoUnit2 = dynamic_cast<const GeomDetUnit*>(comp[1]);
		if(geoUnit2!=0){
		  mod = VisCuTkModuleMap::moduleMap[geoUnit2];
		  myModules.push_back(mod);
		}
	      }
	      if(comp.size()==1 || comp.size()>2) std::cout<<"module " << id << " intersected by rechit not found!"<<std::endl;

	    }else std::cout<<"module " << id << " intersected by rechit not found!"<<std::endl;
	  }
	}
	nt++;
	m_trackModules.push_back (myModules);
      }
    } //if(!notracks)
  }
}

