#include "AnalysisDataFormats/TopObjects/interface/TtSemiLepEvtPartons.h"
#include "TopQuarkAnalysis/TopJetCombination/plugins/TtSemiLepHypGeom.h"

TtSemiLepHypGeom::TtSemiLepHypGeom(const edm::ParameterSet& cfg):
  TtSemiLepHypothesis( cfg ) { }

TtSemiLepHypGeom::~TtSemiLepHypGeom() { }

void
TtSemiLepHypGeom::buildHypo(edm::Event& evt,
			    const edm::Handle<edm::View<reco::RecoCandidate> >& leps,
			    const edm::Handle<std::vector<pat::MET> >& mets, 
			    const edm::Handle<std::vector<pat::Jet> >& jets, 
			    std::vector<int>& match, const unsigned int iComb)
{
  // -----------------------------------------------------
  // add jets
  // -----------------------------------------------------
  for(unsigned idx=0; idx<match.size(); ++idx){
    if( isValid(match[idx], jets) ){
      switch(idx){
      case TtSemiLepEvtPartons::LightQ:
	setCandidate(jets, match[idx], lightQ_,    jetCorrectionLevel("wQuarkMix")); break;
      case TtSemiLepEvtPartons::LightQBar:
	setCandidate(jets, match[idx], lightQBar_, jetCorrectionLevel("wQuarkMix")); break;
      case TtSemiLepEvtPartons::HadB:
	setCandidate(jets, match[idx], hadronicB_, jetCorrectionLevel("bQuark")); break;
      case TtSemiLepEvtPartons::LepB: 
	setCandidate(jets, match[idx], leptonicB_, jetCorrectionLevel("bQuark")); break;
      }
    }
  }

  // -----------------------------------------------------
  // add lepton
  // -----------------------------------------------------
  if( !leps->empty() )
    setCandidate(leps, 0, lepton_);
  match.push_back( 0 );
  
  // -----------------------------------------------------
  // add neutrino
  // -----------------------------------------------------
  if( !mets->empty() )
    setCandidate(mets, 0, neutrino_);
}
