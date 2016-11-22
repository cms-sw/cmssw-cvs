#ifndef SimG4Core_PhysicsLists_QGSPCMS_BERT_NOLEP2_EMV_H
#define SimG4Core_PhysicsLists_QGSPCMS_BERT_NOLEP2_EMV_H
 
#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
class QGSPCMS_BERT_NOLEP2_EMV: public PhysicsList {

public:
  QGSPCMS_BERT_NOLEP2_EMV(G4LogicalVolumeToDDLogicalPartMap& map, const HepPDT::ParticleDataTable * table_, const edm::ParameterSet & p);
};

#endif


