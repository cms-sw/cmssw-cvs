// -*- C++ -*-

// class MockHadronizer is an example of a class that models the
// Hadronizer concept.

#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

namespace HepMC
{
  class GenEvent;
}

namespace edm
{
  class Event;
}

namespace edmtest
{
  class MockHadronizer
  {
  public:
    explicit MockHadronizer(edm::ParameterSet const& ps);

    // bool generatePartons();
    bool generatePartonsAndHadronize();
    bool hadronize();
    bool decay();
    bool residualDecay();
    bool initializeForExternalPartons();
    bool initializeForInternalPartons();
    bool declareStableParticles(const std::vector<int>);
    
    void finalizeEvent() { return; }
    void statistics();

    const char* classname() const;
    
    void resetEvent( const HepMC::GenEvent* ) { return; } 

    HepMC::GenEvent* getGenEvent() { return fGenEvent; }
    GenEventInfoProduct* getGenEventInfo() { return fGenEvtInfo; }
    const GenRunInfoProduct& getGenRunInfo() const { return fGenRunInfo; }

    void setEDMEvent(edm::Event &ev) {}
  
  private:
    HepMC::GenEvent*     fGenEvent;
    GenEventInfoProduct* fGenEvtInfo;
    GenRunInfoProduct    fGenRunInfo;
  
  };
}
