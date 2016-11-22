// -*- C++ -*-

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace edmtest
{
  class EventCounterTester : public edm::EDAnalyzer 
  {
  public:
    explicit EventCounterTester(edm::ParameterSet const& ps);
    virtual ~EventCounterTester();

    virtual void analyze(edm::Event const&, edm::EventSetup const&);
    virtual void endJob();

  private:
    int current_count_;
    int expected_count_;
  };

  EventCounterTester::EventCounterTester(edm::ParameterSet const& ps) :
    edm::EDAnalyzer(),
    current_count_(0),
    expected_count_(ps.getParameter<int>("expected"))
  { }

  EventCounterTester::~EventCounterTester()
  { }

  void
  EventCounterTester::analyze(edm::Event const& , edm::EventSetup const&)
  {
    ++current_count_;    
  }

  void
  EventCounterTester::endJob()
  {

    std::cout << " current_cout_ = " << current_count_ << std::endl;
    std::cout << " expected_cout_ = " << expected_count_ << std::endl;
    
    assert(current_count_ == expected_count_);
  }
    
}


using edmtest::EventCounterTester;
DEFINE_FWK_MODULE(EventCounterTester);

