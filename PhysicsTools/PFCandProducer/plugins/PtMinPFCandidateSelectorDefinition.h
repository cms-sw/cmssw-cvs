#ifndef PhysicsTools_PFCandProducer_PtMinPFCandidateSelectorDefinition
#define PhysicsTools_PFCandProducer_PtMinPFCandidateSelectorDefinition

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "PhysicsTools/PFCandProducer/plugins/PFCandidateSelectorDefinition.h"

struct PtMinPFCandidateSelectorDefinition : public PFCandidateSelectorDefinition {

  PtMinPFCandidateSelectorDefinition ( const edm::ParameterSet & cfg ) :
  ptMin_( cfg.getParameter< double >( "ptMin" ) ) { }

  void select( const HandleToCollection & hc, 
	       const edm::Event & e,
	       const edm::EventSetup& s) {
    selected_.clear();
    
    assert( hc.isValid() );
    
    
    unsigned key=0;
    for( collection::const_iterator pfc = hc->begin(); 
         pfc != hc->end(); ++pfc, ++key) {

      if( pfc->pt() > ptMin_ ) {
	selected_.push_back( reco::PFCandidate(*pfc) );
	reco::PFCandidatePtr ptrToMother( hc, key );
        selected_.back().setSourceCandidatePtr( ptrToMother );

      }
    }
  }

private:
  double ptMin_;
};

#endif
