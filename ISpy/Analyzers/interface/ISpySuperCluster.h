#ifndef ANALYZER_ISPY_SUPERCLUSTER_H
#define ANALYZER_ISPY_SUPERCLUSTER_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"

class ISpySuperCluster : public edm::EDAnalyzer
{
public:
  explicit ISpySuperCluster(const edm::ParameterSet&);
  virtual ~ISpySuperCluster(void) {}

  virtual void analyze(const edm::Event&, const edm::EventSetup&);
private:
  edm::InputTag inputTag_;
  const std::string algoName(reco::CaloCluster::AlgoId key);
};

#endif // ANALYZER_ISPY_SUPERCLUSTER_H
