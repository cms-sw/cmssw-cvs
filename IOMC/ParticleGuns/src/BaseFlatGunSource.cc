/*
 *  $Date: 2010/01/19 16:17:26 $
 *  $Revision: 1.21 $
 *  \author Julia Yarba
 */

#include <ostream>

#include "IOMC/ParticleGuns/interface/BaseFlatGunSource.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

// #include "FWCore/Framework/intercface/ESHandle.h"
// #include "FWCore/Framework/interface/EventSetup.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "FWCore/Utilities/interface/Exception.h"

#include <iostream>

using namespace edm;
using namespace std;
using namespace CLHEP;

namespace {
  CLHEP::HepRandomEngine& getEngineReference()
  {

   Service<RandomNumberGenerator> rng;
   if(!rng.isAvailable()) {
    throw cms::Exception("Configuration")
       << "The RandomNumberProducer module requires the RandomNumberGeneratorService\n"
          "which appears to be absent.  Please add that service to your configuration\n"
          "or remove the modules that require it.";
   }

// The Service has already instantiated an engine.  Make contact with it.
   return (rng->getEngine());
  }
}

BaseFlatGunSource::BaseFlatGunSource( const ParameterSet& pset,
                                      const InputSourceDescription& desc ) : 
   GeneratedInputSource (pset, desc),
   fEvt(0),
   fRandomEngine(getEngineReference()),
   fRandomGenerator(0)
   // fPDGTable( new DefaultConfig::ParticleDataTable("PDG Table") )
{

   ParameterSet defpset ;
   //ParameterSet pgun_params = pset.getParameter<ParameterSet>("PGunParameters") ;
   ParameterSet pgun_params = 
      pset.getUntrackedParameter<ParameterSet>("PGunParameters", defpset ) ;
  
   // although there's the method ParameterSet::empty(),  
   // it looks like it's NOT even necessary to check if it is,
   // before trying to extract parameters - if it is empty,
   // the default values seem to be taken
   vector<int> defids ;
   defids.push_back(13) ;
   fPartIDs    = pgun_params.getUntrackedParameter< vector<int> >("PartID",defids);  
   fMinEta     = pgun_params.getUntrackedParameter<double>("MinEta",-5.5);
   fMaxEta     = pgun_params.getUntrackedParameter<double>("MaxEta",5.5);
   fMinPhi     = pgun_params.getUntrackedParameter<double>("MinPhi",-3.14159265358979323846);
   fMaxPhi     = pgun_params.getUntrackedParameter<double>("MaxPhi", 3.14159265358979323846);

  //
  //fPDGTablePath = "/afs/cern.ch/sw/lcg/external/clhep/1.9.2.1/slc3_ia32_gcc323/data/HepPDT/" ;
/*
  string HepPDTBase( getenv("HEPPDT_PARAM_PATH") ) ; 
  fPDGTablePath = HepPDTBase + "/data/" ;
  fPDGTableName = "PDG_mass_width_2004.mc"; // should it be 2004 table ?

  string TableFullName = fPDGTablePath + fPDGTableName ;
  ifstream PDFile( TableFullName.c_str() ) ;
  if( !PDFile ) 
  {
      throw cms::Exception("FileNotFound", "BaseFlatGunSource::BaseFlatGunSource()")
	<< "File " << TableFullName << " cannot be opened.\n";
  }

  HepPDT::TableBuilder tb(*fPDGTable) ;
  if ( !addPDGParticles( PDFile, tb ) ) { cout << " Error reading PDG !" << endl; }
  // the tb dtor fills fPDGTable
*/

  fVerbosity = pset.getUntrackedParameter<int>( "Verbosity",0 ) ;

// The Service has already instantiated an engine.  Use it.
   fRandomGenerator = new CLHEP::RandFlat(fRandomEngine) ;
   fAddAntiParticle = pset.getUntrackedParameter<bool>("AddAntiParticle",false);
   
}

BaseFlatGunSource::~BaseFlatGunSource()
{
  
  if ( fRandomGenerator != NULL ) delete fRandomGenerator;
  
  // no need to cleanup GenEvent memory - done in HepMCProduct
  // if (fEvt != NULL) delete fEvt ; // double check
  // delete fPDGTable;
  
}


void BaseFlatGunSource::beginRun( edm::Run & r, const EventSetup& es )
{

   es.getData( fPDGTable ) ;

   return ;

}
