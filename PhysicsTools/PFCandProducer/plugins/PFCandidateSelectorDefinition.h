#ifndef PhysicsTools_PFCandProducer_PFCandidateSelectorDefinition
#define PhysicsTools_PFCandProducer_PFCandidateSelectorDefinition

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "boost/iterator/transform_iterator.hpp"
#include <functional>

class PFCandidateSelectorDefinition {
public:
  typedef reco::PFCandidateCollection collection;
  typedef edm::Handle< collection > HandleToCollection;
  typedef std::vector<reco::PFCandidate>  container;

  struct Pointer : public std::unary_function<reco::PFCandidate,const reco::PFCandidate *> { 
    const reco::PFCandidate * operator()(const reco::PFCandidate &c) const { return &c; } 
  };

  typedef boost::transform_iterator<Pointer,container::const_iterator> const_iterator;

  PFCandidateSelectorDefinition () {}

  const_iterator begin() const { return const_iterator(selected_.begin()); }

  const_iterator end() const { return const_iterator(selected_.end()); }

  size_t size() const { return selected_.size(); }

protected:
  container selected_;
};

#endif
