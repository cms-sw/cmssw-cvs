#ifndef ANALYZER_ISPY_MET_H
# define ANALYZER_ISPY_MET_H

# include "FWCore/Framework/interface/EDAnalyzer.h"

class ISpyMET : public edm::EDAnalyzer
{
public:
  explicit ISpyMET(const edm::ParameterSet&);
  virtual ~ISpyMET(void) {}
       
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
private:
  edm::InputTag	inputTag_;
};

#endif // ANALYZER_ISPY_MET_H
