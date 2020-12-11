#ifndef ANALYZER_ISPY_RPCREC_HIT_H
#define ANALYZER_ISPY_RPCREC_HIT_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"

class ISpyRPCRecHit : public edm::EDAnalyzer
{
public:
  explicit ISpyRPCRecHit(const edm::ParameterSet&);
  virtual ~ISpyRPCRecHit(void) {}
       
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
private:
  edm::InputTag	inputTag_;
};

#endif // ANALYZER_ISPY_RPCREC_HIT_H
