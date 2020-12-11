#include "RecoEgamma/EgammaHFProducers/interface/HFClusterAlgo.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <list> 
#include "FWCore/MessageLogger/interface/MessageLogger.h"
using namespace std;
using namespace reco;
/** \class HFClusterAlgo
 *
 *  \author Kevin Klapoetke (Minnesota)
 *
 * $Id:version 1.2
 */


HFClusterAlgo::HFClusterAlgo() 
{}

class CompareHFCompleteHitET {
public:
  bool operator()(const HFClusterAlgo::HFCompleteHit& h1,const HFClusterAlgo::HFCompleteHit& h2) const {
    return h1.et>h2.et;
  }
};

class CompareHFCore {
public:
  bool operator()(const double c1,const double c2) const {
    return c1>c2;
  }
};

void HFClusterAlgo::setup(double minTowerEnergy, double seedThreshold,double maximumSL,double maximumRenergy,bool usePMTflag,bool usePulseflag){
  m_seedThreshold=seedThreshold;
  m_minTowerEnergy=minTowerEnergy;
  m_maximumSL=maximumSL;
  m_usePMTFlag=usePMTflag;
  m_usePulseFlag=usePulseflag;
  m_maximumRenergy=maximumRenergy;
  for(int ii=0;ii<13;ii++){
    m_cutByEta.push_back(-1);
  }
}

/** Analyze the hits */
void HFClusterAlgo::clusterize(const HFRecHitCollection& hf, 
			       const CaloGeometry& geom,
			       HFEMClusterShapeCollection& clusterShapes,
			       SuperClusterCollection& SuperClusters) {
  
  std::vector<HFCompleteHit> protoseeds, seeds;
  HFRecHitCollection::const_iterator j,j2;
  std::vector<HFCompleteHit>::iterator i;
  std::vector<HFCompleteHit>::iterator k;
  int dP, dE, PWrap;
  bool isok=true;
  HFEMClusterShape clusShp;
 
  SuperCluster Sclus;
  bool doCluster=false;
  
  for (j=hf.begin(); j!= hf.end(); j++)  {
    const int aieta=j->id().ietaAbs();
    int iz=(aieta-29);
    // only long fibers and not 29,40,41 allowed to be considered as seeds
    if (j->id().depth()!=1) continue;
    if (aieta==40 || aieta==41 || aieta==29) continue;
   

    if (iz<0 || iz>12) {
      edm::LogWarning("HFClusterAlgo") << "Strange invalid HF hit: " << j->id();
      continue;
    }

    if (m_cutByEta[iz]<0) {
      double eta=geom.getPosition(j->id()).eta();
      m_cutByEta[iz]=m_seedThreshold*cosh(eta); // convert ET to E for this ring
    }
  
    if (j->energy()>m_cutByEta[iz]) {
      j2=hf.find(HcalDetId(HcalForward,j->id().ieta(),j->id().iphi(),2));
      double eshort=(j2==hf.end())?(0):(j2->energy());
      if (((j->energy()-eshort)/(j->energy()+eshort))>m_maximumSL) continue;

      HFCompleteHit ahit;
      double eta=geom.getPosition(j->id()).eta();
      ahit.id=j->id();
      ahit.energy=j->energy();
      ahit.et=ahit.energy/cosh(eta);
      protoseeds.push_back(ahit);
    }
  }

  if(!protoseeds.empty()){   
    std::sort(protoseeds.begin(), protoseeds.end(), CompareHFCompleteHitET());
    for (i=protoseeds.begin(); i!= protoseeds.end(); i++)  {
      isok=true;
      doCluster=false;

      if ( (i==protoseeds.begin()) && (isok) ) {
	doCluster=true;
      }else {
	// check for overlap with existing clusters 
	for (k=seeds.begin(); isok && k!=seeds.end(); k++) { //i->hits, k->seeds
	  
	  for (dE=-2; dE<=2; dE++)
	    for (dP=-4;dP<=4; dP+=2) {
	      PWrap=k->id.iphi()+dP;	
	      if (PWrap<0) 
		PWrap+=72;
	      if (PWrap>72)
		PWrap-=72;
	      
	      if ( (i->id.iphi()==PWrap) && (i->id.ieta()==k->id.ieta()+dE))
		isok = false;
	    }
	}
	if (isok) {
	  doCluster=true;
	}
      }
      if (doCluster) { 
	seeds.push_back(*i);

	bool clusterOk=makeCluster( i->id(),hf, geom,clusShp,Sclus);
	if (clusterOk) { // cluster is _not_ ok if seed is rejected due to other cuts
	  clusterShapes.push_back(clusShp);
	  SuperClusters.push_back(Sclus);
	}

      }
    }//end protoseed loop
  }//end if seeCount
}


bool HFClusterAlgo::makeCluster(const HcalDetId& seedid,
				const HFRecHitCollection& hf, 
				const CaloGeometry& geom,
				HFEMClusterShape& clusShp,
				SuperCluster& Sclus)  {
			

  double w=0;//sum over all log E's
  double wgt=0;
  double w_e=0;//sum over ieat*energy
  double w_x=0;
  double w_y=0;
  double w_z=0;
  double wp_e=0;//sum over iphi*energy
  double e_e=0;//nonwieghted eta sum
  double e_ep=0; //nonweighted phi sum
 
  double l_3=0;//sum for enenergy in 3x3 long fibers etc.
  double s_3=0;
  double l_5=0;
  double s_5=0;
  double l_1=0;
  double s_1=0;
  int de, dp, phiWrap;
  double l_1e=0;
  GlobalPoint sp=geom.getPosition(seedid);
  std::vector<double> coreCanid;
  std::vector<double>::const_iterator ci;
  HFRecHitCollection::const_iterator i,is,il;
  std::vector<DetId> usedHits; 
 
  HFRecHitCollection::const_iterator si;
  HcalDetId sid(HcalForward,seedid.ieta(),seedid.iphi(),1);
  si=hf.find(sid);  

  bool clusterOk=true; // assume the best to start...

  // lots happens here
  // edge type 1 has 40/41 in 3x3 and 5x5
  bool edge_type1=seedid.ietaAbs()==39 && (seedid.iphi()%4)==3;
  
  for (de=-2; de<=2; de++)
    for (dp=-4;dp<=4; dp+=2) {
      phiWrap=seedid.iphi()+dp;	
      if (phiWrap<0) 
        phiWrap+=72;
      if (phiWrap>72)
        phiWrap-=72;

  
        /* Handling of phi-width change problems */
        if (edge_type1 && de==seedid.zside()) {
          if (dp==-2) { // we want it in the 3x3
            phiWrap-=2;
            if (phiWrap<0) 
              phiWrap+=72;
          } 
          else if (dp==-4) {
            continue; // but not double counted in 5x5
          }
        }

	HcalDetId idl(HcalForward,seedid.ieta()+de,phiWrap,1);
	HcalDetId ids(HcalForward,seedid.ieta()+de,phiWrap,2);

	
	il=hf.find(idl);
	is=hf.find(ids);        

	double e_long=1.0; 
	double e_short=0.0; 

	if (il!=hf.end()) e_long=il->energy();
	if (e_long <= m_minTowerEnergy) e_long=0.0;
	if (is!=hf.end()) e_short=is->energy();
	if (e_short <= m_minTowerEnergy) e_short=0.0;
	double eRatio=(e_long-e_short)/std::max(1.0,(e_long+e_short));
	
	// require S/L > a minimum amount for inclusion
	if ((abs(eRatio) > m_maximumSL)&&(std::max(e_long,e_short) > m_maximumRenergy)) {
	  if (dp==0 && de==0) clusterOk=false; // somehow, the seed is hosed
	  continue;
	}
	
	// cut on "PMT HIT" flag
	if ((il!=hf.end())&&(il->flagField(0,1))&&(m_usePMTFlag)) {
	  if (dp==0 && de==0) clusterOk=false; // somehow, the seed is hosed
	  continue;
	}

	// cut on "Pulse shape HIT" flag
	if ((il!=hf.end())&&(il->flagField(1,1))&&(m_usePulseFlag)) {
	  if (dp==0 && de==0) clusterOk=false; // somehow, the seed is hosed
	  continue;
	}
       
	if (e_long > m_minTowerEnergy && il!=hf.end()) {

	  // record usage
	  usedHits.push_back(idl.rawId());
	  // always in the 5x5
	  l_5+=e_long;
	  // maybe in the 3x3
          if ((de>-2)&&(de<2)&&(dp>-4)&&(dp<4)) {
            l_3+=e_long;
          }
	  // sometimes in the 1x1
	  if ((dp==0)&&(de==0)) {
            l_1=e_long;
          }

	  // maybe in the core?
	  if ((de>-2)&&(de<2)&&(dp>-4)&&(dp<4)&&(e_long>(.5*si->energy()))) {
            coreCanid.push_back(e_long);
          }
	  
	  // position calculation
	  GlobalPoint p=geom.getPosition(idl);
          
          double d_p = p.phi()-sp.phi();
          while (d_p < -M_PI)
            d_p+=2*M_PI;
          while (d_p > M_PI)
            d_p-=2*M_PI;
          double d_e = p.eta()-sp.eta();
	  
	  wgt=log((e_long));
	  if (wgt>0){
	    w+=wgt;
	    w_e+=(d_e)*wgt;
	    wp_e+=(d_p)*wgt;
	    e_e+=d_e;
	    e_ep+=d_p;
	   
	    w_x+=(p.x())*wgt;//(p.x()-sp.x())*wgt;
	    w_y+=(p.y())*wgt;
              w_z+=(p.z())*wgt;
	  }
   	} else {
	  if (dp==0 && de==0) clusterOk=false; // somehow, the seed is hosed
	}
	
	if (e_short > m_minTowerEnergy && is!=hf.end()) {
	  // record usage
	  usedHits.push_back(ids.rawId());
	  // always in the 5x5
	  s_5+=e_short;
	  // maybe in the 3x3
	  if ((de>-2)&&(de<2)&&(dp>-4)&&(dp<4)) {
            s_3+=e_short;
          }
	  // sometimes in the 1x1
	  if ((dp==0)&&(de==0)) {
            s_1=e_short;
          }
	}
    }


  if (!clusterOk) return false;

  //Core sorting done here
  std::sort(coreCanid.begin(), coreCanid.end(), CompareHFCore());
  for (ci=coreCanid.begin();ci!=coreCanid.end();ci++){
    if(ci==coreCanid.begin()){
      l_1e=*ci;
    }else if (*ci>.5*l_1e){
      l_1e+=*ci;
    }
  }//core sorting end 
  
  double z_=w_z/w;    
  double x_=w_x/w;
  double y_=w_y/w;
  
  //calcualte position, final
  double eta=w_e/w+sp.eta();
  
  double phi=(wp_e/w)+sp.phi();
  
  while (phi < -M_PI)
    phi+=2*M_PI;
  while (phi > M_PI)
    phi-=2*M_PI;
  
  //calculate cell phi and cell eta
  static const double HFEtaBounds[14] = {2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363, 4.538, 4.716, 4.889, 5.191};
  double RcellEta = fabs(eta);
  double Cphi = (phi>0.)?(fmod((phi),0.087*2)/(0.087*2)):((fmod((phi),0.087*2)/(0.087*2))+1.0);
  double Rbin = -1.0;
  for (int icell = 0; icell < 12; icell++ ){
    if ( (RcellEta>HFEtaBounds[icell]) && (RcellEta<HFEtaBounds[icell+1]) )
      Rbin = (RcellEta - HFEtaBounds[icell])/(HFEtaBounds[icell+1] - HFEtaBounds[icell]);
  }
  double Ceta=Rbin;
  
  while (phi< -M_PI)
    phi+=2*M_PI;
  while (phi > M_PI)
    phi-=2*M_PI;
  
  
  math::XYZPoint xyzclus(x_,y_,z_);
  
  //return  HFEMClusterShape, SuperCluster
  HFEMClusterShape myClusShp(l_1, s_1, l_3, s_3, l_5,s_5, l_1e,Ceta, Cphi,seedid);
  clusShp = myClusShp;
      
  SuperCluster MySclus(l_3,xyzclus);
  Sclus=MySclus;

  return clusterOk;
  
}
