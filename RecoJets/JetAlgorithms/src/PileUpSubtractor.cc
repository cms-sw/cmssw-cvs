
#include "RecoJets/JetAlgorithms/interface/PileUpSubtractor.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include <map>
using namespace std;

int ieta(const reco::CandidatePtr & in);
int iphi(const reco::CandidatePtr & in);

PileUpSubtractor::PileUpSubtractor(const edm::ParameterSet& iConfig,
				   std::vector<edm::Ptr<reco::Candidate> >& input,
				   std::vector<fastjet::PseudoJet>& towers,
				   std::vector<fastjet::PseudoJet>& output) : 
  inputs_(&input),
  fjInputs_(&towers),
  fjJets_(&output),
  reRunAlgo_ (iConfig.getUntrackedParameter<bool>("reRunAlgo",false)),
  jetPtMin_(iConfig.getParameter<double>       ("jetPtMin")),
  nSigmaPU_(iConfig.getParameter<double>("nSigmaPU")),
  radiusPU_(iConfig.getParameter<double>("radiusPU")),
  geo_(0)
{;}

void PileUpSubtractor::setAlgorithm(ClusterSequencePtr& algorithm){
  fjClusterSeq_ = algorithm;
}

void PileUpSubtractor::setupGeometryMap(edm::Event& iEvent,const edm::EventSetup& iSetup)
{

  LogDebug("PileUpSubtractor")<<"The subtractor setting up geometry...\n";

  if(geo_ == 0) {
    edm::ESHandle<CaloGeometry> pG;
    iSetup.get<CaloGeometryRecord>().get(pG);
    geo_ = pG.product();
    std::vector<DetId> alldid =  geo_->getValidDetIds();

    int ietaold = -10000;
    ietamax_ = -10000;
    ietamin_ = 10000;
    for(std::vector<DetId>::const_iterator did=alldid.begin(); did != alldid.end(); did++){
      if( (*did).det() == DetId::Hcal ){
        HcalDetId hid = HcalDetId(*did);
        if( (hid).depth() == 1 ) {
	  allgeomid_.push_back(*did);

          if((hid).ieta() != ietaold){
            ietaold = (hid).ieta();
            geomtowers_[(hid).ieta()] = 1;
            if((hid).ieta() > ietamax_) ietamax_ = (hid).ieta();
            if((hid).ieta() < ietamin_) ietamin_ = (hid).ieta();
          }
          else{
            geomtowers_[(hid).ieta()]++;
          }
        }
      }
    }
  }

  for (int i = ietamin_; i<ietamax_+1; i++) {
    ntowersWithJets_[i] = 0;
  }
}

//
// Calculate mean E and sigma from jet collection "coll".  
//
void PileUpSubtractor::calculatePedestal( vector<fastjet::PseudoJet> const & coll )
{
  LogDebug("PileUpSubtractor")<<"The subtractor calculating pedestals...\n";

  map<int,double> emean2;
  map<int,int> ntowers;
    
  int ietaold = -10000;
  int ieta0 = -100;
   
  // Initial values for emean_, emean2, esigma_, ntowers

  for(int i = ietamin_; i < ietamax_+1; i++)
    {
      emean_[i] = 0.;
      emean2[i] = 0.;
      esigma_[i] = 0.;
      ntowers[i] = 0;
    }
    
  for (vector<fastjet::PseudoJet>::const_iterator input_object = coll.begin (),
	 fjInputsEnd = coll.end();  
       input_object != fjInputsEnd; ++input_object) {

     const reco::CandidatePtr & originalTower=(*inputs_)[ input_object->user_index()];
    ieta0 = ieta( originalTower );
    double Original_Et = originalTower->et();

  if( ieta0-ietaold != 0 )
      {
        emean_[ieta0] = emean_[ieta0]+Original_Et;
        emean2[ieta0] = emean2[ieta0]+Original_Et*Original_Et;
        ntowers[ieta0] = 1;
        ietaold = ieta0;
      }
  else
      {
        emean_[ieta0] = emean_[ieta0]+Original_Et;
        emean2[ieta0] = emean2[ieta0]+Original_Et*Original_Et;
        ntowers[ieta0]++;
      }

  }

  for(map<int,int>::const_iterator gt = geomtowers_.begin(); gt != geomtowers_.end(); gt++)    
    {

      int it = (*gt).first;
       
      double e1 = (*(emean_.find(it))).second;
      double e2 = (*emean2.find(it)).second;
      int nt = (*gt).second - (*(ntowersWithJets_.find(it))).second;
        
      if(nt > 0) {
	emean_[it] = e1/nt;
	double eee = e2/nt - e1*e1/(nt*nt);    
	if(eee<0.) eee = 0.;
	esigma_[it] = nSigmaPU_*sqrt(eee);
      }
      else
	{
          emean_[it] = 0.;
          esigma_[it] = 0.;
	}
      LogDebug("PileUpSubtractor")<<"Pedestals : "<<emean_[it]<<"  "<<esigma_[it]<<"\n";
    }
}


//
// Subtract mean and sigma from fjInputs_
//    
void PileUpSubtractor::subtractPedestal(vector<fastjet::PseudoJet> & coll)
{

  LogDebug("PileUpSubtractor")<<"The subtractor subtracting pedestals...\n";

  int it = -100;
  int ip = -100;
        
  for (vector<fastjet::PseudoJet>::iterator input_object = coll.begin (),
	 fjInputsEnd = coll.end(); 
       input_object != fjInputsEnd; ++input_object) {
    
     reco::CandidatePtr const & itow =  (*inputs_)[ input_object->user_index() ];
    
    it = ieta( itow );
    ip = iphi( itow );
    
    double etnew = itow->et() - (*(emean_.find(it))).second - (*(esigma_.find(it))).second;
    float mScale = etnew/input_object->Et(); 
    
    if(etnew < 0.) mScale = 0.;
    
    math::XYZTLorentzVectorD towP4(input_object->px()*mScale, input_object->py()*mScale,
				   input_object->pz()*mScale, input_object->e()*mScale);
    
    int index = input_object->user_index();
    input_object->reset ( towP4.px(),
			  towP4.py(),
			  towP4.pz(),
			  towP4.energy() );
    input_object->set_user_index(index);
  }
}



void PileUpSubtractor::calculateOrphanInput(vector<fastjet::PseudoJet> & orphanInput) 
{

  LogDebug("PileUpSubtractor")<<"The subtractor calculating orphan input...\n";

  vector<int> jettowers; // vector of towers indexed by "user_index"
  vector <fastjet::PseudoJet>::iterator pseudojetTMP = fjJets_->begin (),
    fjJetsEnd = fjJets_->end();
    
  for (; pseudojetTMP != fjJetsEnd ; ++pseudojetTMP) {
      
    vector<fastjet::PseudoJet> newtowers;
      
    // get eta, phi of this jet
    double eta2 = pseudojetTMP->eta();
    double phi2 = pseudojetTMP->phi();
    // find towers within radiusPU_ of this jet
    for(vector<HcalDetId>::const_iterator im = allgeomid_.begin(); im != allgeomid_.end(); im++)
      {
	double eta1 = geo_->getPosition((DetId)(*im)).eta();
	double phi1 = geo_->getPosition((DetId)(*im)).phi();
	double dphi = fabs(phi1-phi2);
	double deta = eta1-eta2;
	if (dphi > 4.*atan(1.)) dphi = 8.*atan(1.) - dphi;
	double dr = sqrt(dphi*dphi+deta*deta);  
	if( dr < radiusPU_) {
	  ntowersWithJets_[(*im).ieta()]++;     
	}
      }

    vector<fastjet::PseudoJet>::const_iterator it = fjInputs_->begin(),
      fjInputsEnd = fjInputs_->end();
      
    // 
    for (; it != fjInputsEnd; ++it ) {
      
      double eta1 = it->eta();
      double phi1 = it->phi();
      
      double dphi = fabs(phi1-phi2);
      double deta = eta1-eta2;
      if (dphi > 4.*atan(1.)) dphi = 8.*atan(1.) - dphi;
      double dr = sqrt(dphi*dphi+deta*deta);
      
      if( dr < radiusPU_) {
	newtowers.push_back(*it);
	jettowers.push_back(it->user_index());
      } //dr < 0.5

    } // initial input collection  

  } // pseudojets

  //
  // Create a new collections from the towers not included in jets 
  //
  for(vector<fastjet::PseudoJet>::const_iterator it = fjInputs_->begin(),
	fjInputsEnd = fjInputs_->end(); it != fjInputsEnd; ++it ) {
    vector<int>::const_iterator itjet = find(jettowers.begin(),jettowers.end(),it->user_index());
    if( itjet == jettowers.end() ) orphanInput.push_back(*it); 
  }

}


void PileUpSubtractor::offsetCorrectJets() 
{

  LogDebug("PileUpSubtractor")<<"The subtractor correcting jets...\n";

  jetOffset_.clear();

  using namespace reco;

  if(reRunAlgo_){
    subtractPedestal(*fjInputs_);
    const fastjet::JetDefinition def = fjClusterSeq_->jet_def();
    fjClusterSeq_.reset(new fastjet::ClusterSequence( *fjInputs_, def ));
    *fjJets_ = fastjet::sorted_by_pt(fjClusterSeq_->inclusive_jets(jetPtMin_));
  }

  //    
  // Reestimate energy of jet (energy of jet with initial map)
  //

  jetOffset_.reserve(fjJets_->size());

     vector<fastjet::PseudoJet>::iterator pseudojetTMP = fjJets_->begin (),
	jetsEnd = fjJets_->end();
     for (; pseudojetTMP != jetsEnd; ++pseudojetTMP) {

	int ijet = pseudojetTMP - fjJets_->begin();
	jetOffset_[ijet] = 0;

	std::vector<fastjet::PseudoJet> towers =
	   sorted_by_pt(fjClusterSeq_->constituents(*pseudojetTMP));

	double newjetet = 0.;	
	for(vector<fastjet::PseudoJet>::const_iterator ito = towers.begin(),
	    towEnd = towers.end(); 
	    ito != towEnd; 
	    ++ito)
	   {
	      const reco::CandidatePtr& originalTower = (*inputs_)[ito->user_index()];
	      int it = ieta( originalTower );
	      double Original_Et = originalTower->et();
	      double etnew = Original_Et - (*emean_.find(it)).second - (*esigma_.find(it)).second; 
	      if(etnew < 0.) etnew = 0;
	      newjetet = newjetet + etnew;
	      jetOffset_[ijet] += Original_Et - etnew;
	   }

	if(!reRunAlgo_){
	  double mScale = newjetet/pseudojetTMP->Et();
	  LogDebug("PileUpSubtractor")<<"pseudojetTMP->Et() : "<<pseudojetTMP->Et()<<"\n";
	  LogDebug("PileUpSubtractor")<<"newjetet : "<<newjetet<<"\n";
	  LogDebug("PileUpSubtractor")<<"jetOffset_[ijet] : "<<jetOffset_[ijet]<<"\n";
	  LogDebug("PileUpSubtractor")<<"pseudojetTMP->Et() - jetOffset_[ijet] : "<<pseudojetTMP->Et() - jetOffset_[ijet]<<"\n";
	  LogDebug("PileUpSubtractor")<<"Scale is : "<<mScale<<"\n";

	   int cshist = pseudojetTMP->cluster_hist_index();
	   pseudojetTMP->reset(pseudojetTMP->px()*mScale, pseudojetTMP->py()*mScale,
			       pseudojetTMP->pz()*mScale, pseudojetTMP->e()*mScale);
	   pseudojetTMP->set_cluster_hist_index(cshist);
	   
	}
     }

}

double PileUpSubtractor::getPileUpAtTower(const reco::CandidatePtr & in){
  int it = ieta(in);
  return (*emean_.find(it)).second + (*esigma_.find(it)).second;
}

int ieta(const reco::CandidatePtr & in)
{
  int it = 0;
  const CaloTower* ctc = dynamic_cast<const CaloTower*>(in.get());

  if(ctc)
    {
      it = ctc->id().ieta();
    } else
      {
	throw cms::Exception("Invalid Constituent") << "CaloJet constituent is not of CaloTower type";
      }

  return it;
}

int iphi(const reco::CandidatePtr & in)
{
  int it = 0;
  const CaloTower* ctc = dynamic_cast<const CaloTower*>(in.get());
  if(ctc)
    {
      it = ctc->id().iphi();
    } else
      {
	throw cms::Exception("Invalid Constituent") << "CaloJet constituent is not of CaloTower type";
      }

  return it;
}


