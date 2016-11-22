   
// -*- C++ -*-

#include "MockHadronizer.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "HepMC/GenEvent.h"

namespace edmtest
{
  MockHadronizer::MockHadronizer(edm::ParameterSet const& ) : fGenEvent(0)
  { }

  bool MockHadronizer::generatePartonsAndHadronize()
  {
    return true;
  }

  bool MockHadronizer::hadronize()
  {
    return true;
  }

  bool MockHadronizer::decay()
  {
    return true;
  }

  bool MockHadronizer::residualDecay()
  {
     return true;
  }
  
  bool MockHadronizer::initializeForExternalPartons()
  {
    return true;
  }

  bool MockHadronizer::initializeForInternalPartons()
  {
    return true;
  }

  bool MockHadronizer::declareStableParticles( const std::vector<int> )
  {
    return true;
  }

  void MockHadronizer::statistics()
  {
     return;
  }
  
  const char* MockHadronizer::classname() const
  {
    return "edmtest::MockHadronizer";
  }

}
