#ifndef SimG4Core_PhysicsLists_QGSBCMS_BERT_NOLEP2_EMV_H
#define SimG4Core_PhysicsLists_QGSBCMS_BERT_NOLEP2_EMV_H
 
#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
class QGSBCMS_BERT_NOLEP2_EMV: public PhysicsList {

public:
  QGSBCMS_BERT_NOLEP2_EMV(G4LogicalVolumeToDDLogicalPartMap& map, const HepPDT::ParticleDataTable * table_, const edm::ParameterSet & p);
};

#endif


