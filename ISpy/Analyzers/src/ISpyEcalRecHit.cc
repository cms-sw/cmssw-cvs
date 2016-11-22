#include "ISpy/Analyzers/interface/ISpyEcalRecHit.h"
#include "ISpy/Analyzers/interface/ISpyService.h"
#include "ISpy/Services/interface/IgCollection.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include <iostream>
#include <sstream>

using namespace edm::service;

ISpyEcalRecHit::ISpyEcalRecHit (const edm::ParameterSet& iConfig)
  : inputTags_(iConfig.getParameter<std::vector<edm::InputTag> >("iSpyEcalRecHitTags"))
{}

void
ISpyEcalRecHit::analyze( const edm::Event& event, const edm::EventSetup& eventSetup)
{
  edm::Service<ISpyService> config;

  if (! config.isAvailable ()) 
  {
    throw cms::Exception ("Configuration")
      << "ISpyEcalRecHit requires the ISpyService\n"
      "which is not present in the configuration file.\n"
      "You must add the service in the configuration file\n"
      "or remove the module that requires it";
  }

  IgDataStorage *storage = config->storage();
    
  edm::ESHandle<CaloGeometry> geom;
  eventSetup.get<CaloGeometryRecord> ().get (geom);

  if ( ! geom.isValid() )
  {
    std::string error = 
      "### Error: ISpyEcalRecHit::analyze: Invalid CaloGeometryRecord ";
    config->error (error);
    return;
  }

  for( VInputTag::const_iterator ti = inputTags_.begin();
	ti != inputTags_.end(); ++ti )
  {
    edm::Handle<EcalRecHitCollection> collection;
    event.getByLabel (*ti, collection);

    if( collection.isValid () )
    {	    
      std::string product = "EcalRecHits "
			    + edm::TypeID(typeid (EcalRecHitCollection)).friendlyClassName () + ":" 
			    + (*ti).label() + ":"
			    + (*ti).instance() + ":" 
			    + (*ti).process();
	    
      IgCollection& products = storage->getCollection("Products_V1");
      IgProperty PROD = products.addProperty("Product", std::string ());
      IgCollectionItem item = products.create();
      item[PROD] = product;
	    
      IgCollection &recHits = storage->getCollection("EcalRecHits_V1");
      IgProperty E = recHits.addProperty("energy", 0.0);
      IgProperty ETA = recHits.addProperty("eta", 0.0);
      IgProperty PHI = recHits.addProperty("phi", 0.0);
      IgProperty DETID = recHits.addProperty("detid", int (0));			
      IgProperty FRONT_1 = recHits.addProperty("front_1", IgV3d());
      IgProperty FRONT_2 = recHits.addProperty("front_2", IgV3d());	
      IgProperty FRONT_3 = recHits.addProperty("front_3", IgV3d());	
      IgProperty FRONT_4 = recHits.addProperty("front_4", IgV3d());
      IgProperty BACK_1  = recHits.addProperty("back_1",  IgV3d());
      IgProperty BACK_2  = recHits.addProperty("back_2",  IgV3d());
      IgProperty BACK_3  = recHits.addProperty("back_3",  IgV3d());
      IgProperty BACK_4  = recHits.addProperty("back_4",  IgV3d());

      for( std::vector<EcalRecHit>::const_iterator it=collection->begin(), itEnd=collection->end(); it!=itEnd; ++it)
      {
	const CaloCellGeometry *cell = (*geom).getGeometry ((*it).detid ());
	const CaloCellGeometry::CornersVec& corners = cell->getCorners ();
	const GlobalPoint& pos = cell->getPosition ();
	float energy = (*it).energy ();
	float eta = pos.eta ();
	float phi = pos.phi ();

	IgCollectionItem irechit = recHits.create();
	irechit[E] = static_cast<double>(energy);
	irechit[ETA] = static_cast<double>(eta);
	irechit[PHI] = static_cast<double>(phi);
	irechit[DETID] = static_cast<int>((*it).detid ());
	irechit[FRONT_1] = IgV3d(static_cast<double>(corners[3].x()/100.0), 
				 static_cast<double>(corners[3].y()/100.0), 
				 static_cast<double>(corners[3].z()/100.0));
	irechit[FRONT_2] = IgV3d(static_cast<double>(corners[2].x()/100.0), 
				 static_cast<double>(corners[2].y()/100.0), 
				 static_cast<double>(corners[2].z()/100.0));
	irechit[FRONT_3] = IgV3d(static_cast<double>(corners[1].x()/100.0), 
				 static_cast<double>(corners[1].y()/100.0), 
				 static_cast<double>(corners[1].z()/100.0));
	irechit[FRONT_4] = IgV3d(static_cast<double>(corners[0].x()/100.0), 
				 static_cast<double>(corners[0].y()/100.0), 
				 static_cast<double>(corners[0].z()/100.0));
	    
	irechit[BACK_1] = IgV3d(static_cast<double>(corners[7].x()/100.0), 
				static_cast<double>(corners[7].y()/100.0), 
				static_cast<double>(corners[7].z()/100.0));
	irechit[BACK_2] = IgV3d(static_cast<double>(corners[6].x()/100.0), 
				static_cast<double>(corners[6].y()/100.0), 
				static_cast<double>(corners[6].z()/100.0));
	irechit[BACK_3] = IgV3d(static_cast<double>(corners[5].x()/100.0), 
				static_cast<double>(corners[5].y()/100.0), 
				static_cast<double>(corners[5].z()/100.0));
	irechit[BACK_4] = IgV3d(static_cast<double>(corners[4].x()/100.0), 
				static_cast<double>(corners[4].y()/100.0), 
				static_cast<double>(corners[4].z()/100.0));	
      }
    }
    
    else 
    {
      std::string error = "### Error: EcalRecHits "
			  + edm::TypeID (typeid (EcalRecHitCollection)).friendlyClassName () + ":" 
			  + (*ti).label() + ":"
			  + (*ti).instance() + ":" 
			  + (*ti).process() + " are not found.";
      config->error (error);
    }
  }
}

DEFINE_FWK_MODULE(ISpyEcalRecHit);
