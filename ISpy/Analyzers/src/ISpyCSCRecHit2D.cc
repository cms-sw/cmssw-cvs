#include "ISpy/Analyzers/interface/ISpyCSCRecHit2D.h"
#include "ISpy/Analyzers/interface/ISpyService.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "ISpy/Services/interface/IgCollection.h"

#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2D.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CSCGeometry/interface/CSCLayer.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"

using namespace edm::service;
using namespace edm;

ISpyCSCRecHit2D::ISpyCSCRecHit2D(const edm::ParameterSet& iConfig)
: inputTag_(iConfig.getParameter<edm::InputTag>("iSpyCSCRecHit2DTag")){}

void ISpyCSCRecHit2D::analyze(const edm::Event& event, const edm::EventSetup& eventSetup)
{
  edm::Service<ISpyService> config;
  
  if ( ! config.isAvailable() )
  {
    throw cms::Exception ("Configuration")
      << "ISpyCSCRecHit2D requires the ISpyService\n"
      "which is not present in the configuration file.\n"
      "You must add the service in the configuration file\n"
      "or remove the module that requires it";
  }

  IgDataStorage *storage = config->storage();

  edm::ESHandle<CSCGeometry> geom;
  eventSetup.get<MuonGeometryRecord> ().get (geom);
 
  if ( ! geom.isValid() )
  {
    std::string error = 
      "### Error: ISpyCSCRecHit2D::analyze: Invalid MuonGeometryRecord ";
    
    config->error (error);
    return;
  }

  edm::Handle<CSCRecHit2DCollection> collection;
  event.getByLabel(inputTag_, collection);
   
  if ( collection.isValid() )
  {
    std::string product = "CSCRecHit2Ds "
                          + edm::TypeID (typeid (CSCRecHit2DCollection)).friendlyClassName() + ":"
                          + inputTag_.label() + ":"
                          + inputTag_.instance() + ":"
                          + inputTag_.process();

    IgCollection& products = storage->getCollection("Products_V1");
    IgProperty PROD = products.addProperty("Product", std::string ());
    IgCollectionItem item = products.create();
    item[PROD] = product;

    IgCollection& recHits = storage->getCollection("CSCRecHit2Ds_V2");

    IgProperty U1 = recHits.addProperty("u1", IgV3d());
    IgProperty U2 = recHits.addProperty("u2", IgV3d());
    IgProperty V1 = recHits.addProperty("v1", IgV3d());
    IgProperty V2 = recHits.addProperty("v2", IgV3d());
    IgProperty W1 = recHits.addProperty("w1", IgV3d());
    IgProperty W2 = recHits.addProperty("w2", IgV3d());

    IgProperty EC = recHits.addProperty("endcap", int(0));
    IgProperty ST = recHits.addProperty("station", int(0));
    IgProperty RG = recHits.addProperty("ring", int(0));
    IgProperty CH = recHits.addProperty("chamber", int(0));
    IgProperty LA = recHits.addProperty("layer", int(0));

    IgProperty  T  = recHits.addProperty("tpeak", 0.0);
    IgProperty POS = recHits.addProperty("positionWithinStrip", 0.0);
    IgProperty ERR = recHits.addProperty("errorWithinStrip", 0.0);
    IgProperty CHS = recHits.addProperty("strips", std::string());
    IgProperty WIS = recHits.addProperty("wireGroups", std::string());

    for ( CSCRecHit2DCollection::const_iterator it = collection->begin(), itEnd = collection->end(); 
          it != itEnd; ++it )
    {
      const GeomDetUnit *det = geom->idToDetUnit((*it).cscDetId());

      LocalPoint xyzLocal = it->localPosition();
      float x = xyzLocal.x();
      float y = xyzLocal.y();
      float z = xyzLocal.z();
          
      float dx = sqrt(it->localPositionError().xx());
      float dy = sqrt(it->localPositionError().yy());
          
      GlobalPoint gp;
      IgCollectionItem irechit = recHits.create();

      gp = det->surface().toGlobal(LocalPoint((x - dx), y, z));
      irechit[U1] = IgV3d(gp.x()/100.0, gp.y()/100.0, gp.z()/100.0);

      gp = det->surface().toGlobal (LocalPoint((x + dx), y, z));
      irechit[U2] = IgV3d(gp.x()/100.0, gp.y()/100.0, gp.z()/100.0);
      
      gp = det->surface().toGlobal(LocalPoint(x, (y - dy), z));
      irechit[V1] = IgV3d(gp.x()/100.0, gp.y()/100.0, gp.z()/100.0);

      gp = det->surface().toGlobal(LocalPoint(x, (y + dy), z));
      irechit[V2] = IgV3d(gp.x()/100.0, gp.y()/100.0, gp.z()/100.0);
      
      gp = det->surface().toGlobal(xyzLocal); // no error in z
      irechit[W1] = IgV3d(gp.x()/100.0, gp.y()/100.0, gp.z()/100.0);
      irechit[W2] = IgV3d(gp.x()/100.0, gp.y()/100.0, gp.z()/100.0); 

      CSCDetId id = (*it).cscDetId();
      
      irechit[T] = static_cast<double>(it->tpeak());

      irechit[EC] = id.endcap();
      irechit[ST] = id.station();
      irechit[RG] = id.ring();
      irechit[CH] = id.chamber();
      irechit[LA] = id.layer();

      // Need to cast since they return floats
      irechit[POS] = static_cast<double>((*it).positionWithinStrip());
      irechit[ERR] = static_cast<double>((*it).errorWithinStrip());

      std::stringstream chs;

      for ( std::vector<int>::const_iterator cc = 
              (*it).channels().begin(), ccEnd = (*it).channels().end(); 
            cc != ccEnd; ++cc )
        chs<< *cc <<" ";
      
      irechit[CHS] = chs.str(); 

      std::stringstream wis;

      for ( std::vector<int>::const_iterator cc 
              = (*it).wgroups().begin(), ccEnd = (*it).wgroups().end();
            cc != ccEnd; ++cc )
        wis<< *cc <<" ";
      
      irechit[WIS] = wis.str(); 
    }
  }

  else
  {
    std::string error = "### Error: CSCRecHit2Ds "
                        + edm::TypeID (typeid (CSCRecHit2DCollection)).friendlyClassName() + ":"
                        + inputTag_.label() + ":"
                        + inputTag_.instance() + ":"
                        + inputTag_.process() + " are not found.";
    config->error (error);
  }
}

DEFINE_FWK_MODULE(ISpyCSCRecHit2D);
