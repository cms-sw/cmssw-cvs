#include "RecoLocalTracker/SiPixelRecHits/interface/PixelCPEInitialESProducer.h"
#include "RecoLocalTracker/SiPixelRecHits/interface/PixelCPEInitial.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"

#include <string>
#include <memory>

using namespace edm;

PixelCPEInitialESProducer::PixelCPEInitialESProducer(const edm::ParameterSet & p) 
{
  std::string myname = p.getParameter<std::string>("ComponentName");
  pset_ = p;
  setWhatProduced(this,myname);
}

PixelCPEInitialESProducer::~PixelCPEInitialESProducer() {}

boost::shared_ptr<PixelClusterParameterEstimator> 
PixelCPEInitialESProducer::produce(const TkPixelCPERecord & iRecord){ 

  ESHandle<MagneticField> magfield;
  iRecord.getRecord<IdealMagneticFieldRecord>().get(magfield );
  
  ESHandle<SiPixelLorentzAngle> lorentzAngle;
  iRecord.getRecord<SiPixelLorentzAngleRcd>().get(lorentzAngle );

  edm::ESHandle<TrackerGeometry> pDD;
  iRecord.getRecord<TrackerDigiGeometryRecord>().get( pDD );

  cpe_  = boost::shared_ptr<PixelClusterParameterEstimator>(new PixelCPEInitial(pset_,magfield.product(), lorentzAngle.product())  );
  return cpe_;
}


