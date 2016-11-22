#include "QGSBCMS_BERT_NOLEP1_EML.hh"
#include "SimG4Core/PhysicsLists/interface/CMSEmStandardPhysics92.h"
#include "SimG4Core/PhysicsLists/interface/HadronPhysicsQGSB_BERT_NOLEP1.hh"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "G4DecayPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "G4DataQuestionaire.hh"

#include <string>

QGSBCMS_BERT_NOLEP1_EML::QGSBCMS_BERT_NOLEP1_EML(G4LogicalVolumeToDDLogicalPartMap& map,
						 const HepPDT::ParticleDataTable * table_,
						 const edm::ParameterSet & p) : PhysicsList(map, table_, p) {

  G4DataQuestionaire it(photon);
  
  int  ver     = p.getUntrackedParameter<int>("Verbosity",0);
  bool emPhys  = p.getUntrackedParameter<bool>("EMPhysics",true);
  bool hadPhys = p.getUntrackedParameter<bool>("HadPhysics",true);
  double charge= p.getUntrackedParameter<double>("MonopoleCharge",1.0);
  std::string region = p.getParameter<std::string>("Region");
  edm::LogInfo("PhysicsList") << "You are using the simulation engine: "
			      << "QGSB_BERT_NOLEP1_EML 1.0 with Flags for EM Physics "
			      << emPhys << " and for Hadronic Physics "
			      << hadPhys << " and special region " << region
			      << "\n";

  if (emPhys) {
    // EM Physics
    RegisterPhysics( new CMSEmStandardPhysics92("standard EM EML",table_,ver,region,charge));

    // Synchroton Radiation & GN Physics
    RegisterPhysics( new G4EmExtraPhysics("extra EM"));
  }

  // Decays
  RegisterPhysics( new G4DecayPhysics("decay",ver) );

  if (hadPhys) {
    // Hadron Elastic scattering
    RegisterPhysics( new G4HadronElasticPhysics("elastic",ver,false));

    // Hadron Physics
    G4bool quasiElastic=true;
    RegisterPhysics( new HadronPhysicsQGSB_BERT_NOLEP1("hadron",quasiElastic));
  
    // Stopping Physics
    RegisterPhysics( new G4QStoppingPhysics("stopping"));

    // Ion Physics
    RegisterPhysics( new G4IonBinaryCascadePhysics("ionBIC"));

    // Neutron tracking cut
    RegisterPhysics( new G4NeutronTrackingCut("Neutron tracking cut", ver));
  }
}
