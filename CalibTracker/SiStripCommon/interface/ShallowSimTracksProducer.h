#ifndef SHALLOW_SIMTRACKS_PRODUCER
#define SHALLOW_SIMTRACKS_PRODUCER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

class ShallowSimTracksProducer : public edm::EDProducer {

 public:

  explicit ShallowSimTracksProducer(const edm::ParameterSet&);

 private:

  const std::string Prefix;
  const std::string Suffix;
  const edm::InputTag trackingParticles_tag;
  const edm::ESInputTag associator_tag;
  const edm::InputTag tracks_tag;
  void produce( edm::Event &, const edm::EventSetup & );

};
#endif
