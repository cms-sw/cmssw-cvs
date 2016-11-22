#include "RecoBTag/SoftLepton/plugins/SoftElectronCandProducer.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include <cmath>

SoftElectronCandProducer::SoftElectronCandProducer (const edm::ParameterSet& conf) :
    gsfElectronTag_                  (conf.getParameter<edm::InputTag>("electrons")),

    barrelPtCuts_                    (conf.getParameter<std::vector<double> >("BarrelPtCuts")),
    barreldRGsfTrackElectronCuts_    (conf.getParameter<std::vector<double> >("BarreldRGsfTrackElectronCuts")),
    barrelEemPinRatioCuts_           (conf.getParameter<std::vector<double> >("BarrelEemPinRatioCuts")),
    barrelMVACuts_                   (conf.getParameter<std::vector<double> >("BarrelMVACuts")),
    barrelInversedRFirstLastHitCuts_ (conf.getParameter<std::vector<double> >("BarrelInversedRFirstLastHitCuts")),
    barrelRadiusFirstHitCuts_        (conf.getParameter<std::vector<double> >("BarrelRadiusFirstHitCuts")),
    barrelZFirstHitCuts_             (conf.getParameter<std::vector<double> >("BarrelZFirstHitCuts")),

    forwardPtCuts_                   (conf.getParameter<std::vector<double> >("ForwardPtCuts")),
    forwardInverseFBremCuts_         (conf.getParameter<std::vector<double> >("ForwardInverseFBremCuts")),
    forwarddRGsfTrackElectronCuts_   (conf.getParameter<std::vector<double> >("ForwarddRGsfTrackElectronCuts")),
    forwardRadiusFirstHitCuts_       (conf.getParameter<std::vector<double> >("ForwardRadiusFirstHitCuts")),
    forwardZFirstHitCuts_            (conf.getParameter<std::vector<double> >("ForwardZFirstHitCuts")),
    forwardMVACuts_                  (conf.getParameter<std::vector<double> >("ForwardMVACuts"))
{
    produces< edm::ValueMap<float> >();
}

SoftElectronCandProducer::~SoftElectronCandProducer()
{

}

void SoftElectronCandProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<reco::GsfElectronCollection> gsfCandidates;
  iEvent.getByLabel(gsfElectronTag_, gsfCandidates);

  std::vector<float> values;
  values.reserve(gsfCandidates->size());

  for(reco::GsfElectronCollection::const_iterator iter = gsfCandidates->begin();
      iter != gsfCandidates->end(); ++iter)
    values.push_back(isClean(*iter) ? 1.0 : 0.0);

  std::auto_ptr< edm::ValueMap<float> > output(new edm::ValueMap<float>);
  edm::ValueMap<float>::Filler filler(*output);
  filler.insert(gsfCandidates, values.begin(), values.end());
  filler.fill();
  iEvent.put(output);
}

bool SoftElectronCandProducer::isClean(const reco::GsfElectron& gsfcandidate)
{
  const double ecalEnergy            = gsfcandidate.superCluster()->energy();
  const double pIn                   = gsfcandidate.gsfTrack()->innerMomentum().R();
  const double pOut                  = gsfcandidate.gsfTrack()->outerMomentum().R();
  const double EemPinRatio           = (ecalEnergy - pIn)/(ecalEnergy + pIn);
  //const double EemPoutRatio          = (ecalEnergy - pOut)/(ecalEnergy + pOut);
  const double pt                    = gsfcandidate.pt();
  const double fbrem                 = (pIn - pOut)/pIn;
  const double dRGsfTrackElectron    = deltaR(gsfcandidate.gsfTrack()->eta(), gsfcandidate.gsfTrack()->phi(), gsfcandidate.eta(), gsfcandidate.phi());
  const double mva                   = gsfcandidate.mva();
  const math::XYZPoint firstHit      = gsfcandidate.gsfTrack()->innerPosition();
  const math::XYZPoint lastHit       = gsfcandidate.gsfTrack()->outerPosition();
  const double inversedRFirstLastHit = 1.0/deltaR(firstHit.eta(), firstHit.phi(), lastHit.eta(), lastHit.phi());
  const double radiusFirstHit        = firstHit.Rho();
  const double zFirstHit             = firstHit.Z();
  /*std::cout << "This particle has " << pt << " "
                                    << 1.0/fbrem << " "
                                    << dRGsfTrackElectron << " "
                                    << EemPoutRatio << " "
                                    << mva << std::endl;*/
  if(fabs(gsfcandidate.eta()) < 1.5)
  {
    // use barrel cuts
    if( barrelPtCuts_.front()                    > pt                    || barrelPtCuts_.back()                    < pt)                    return false;
    if( barreldRGsfTrackElectronCuts_.front()    > dRGsfTrackElectron    || barreldRGsfTrackElectronCuts_.back()    < dRGsfTrackElectron)    return false;
    if( barrelEemPinRatioCuts_.front()           > EemPinRatio           || barrelEemPinRatioCuts_.back()           < EemPinRatio)           return false;
    if( barrelMVACuts_.front()                   > mva                   || barrelMVACuts_.back()                   < mva)                   return false;
    if( barrelInversedRFirstLastHitCuts_.front() > inversedRFirstLastHit || barrelInversedRFirstLastHitCuts_.back() < inversedRFirstLastHit) return false;
    if( barrelRadiusFirstHitCuts_.front()        > radiusFirstHit        || barrelRadiusFirstHitCuts_.back()        < radiusFirstHit)        return false;
    if( barrelZFirstHitCuts_.front()             > zFirstHit             || barrelZFirstHitCuts_.back()             < zFirstHit)             return false;
  }
  else
  {
    // use endcap cuts
    if( forwardPtCuts_.front()                 > pt                 || forwardPtCuts_.back()                 < pt)                 return false;
    if( forwardInverseFBremCuts_.front()       > 1.0/fbrem          || forwardInverseFBremCuts_.back()       < 1.0/fbrem)          return false;
    if( forwarddRGsfTrackElectronCuts_.front() > dRGsfTrackElectron || forwarddRGsfTrackElectronCuts_.back() < dRGsfTrackElectron) return false;
    if( forwardRadiusFirstHitCuts_.front()     > radiusFirstHit     || forwardRadiusFirstHitCuts_.back()     < radiusFirstHit)     return false;
    if( forwardZFirstHitCuts_.front()          > zFirstHit          || forwardZFirstHitCuts_.back()          < zFirstHit)          return false;
    if( forwardMVACuts_.front()                > mva                || forwardMVACuts_.back()                < mva)                return false;
  }

  return true;
}
