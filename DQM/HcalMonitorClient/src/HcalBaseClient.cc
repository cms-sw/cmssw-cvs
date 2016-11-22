#include <DQM/HcalMonitorClient/interface/HcalBaseClient.h>
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include <math.h>
#include <iostream>

// This stores the eta binning for depth 2 histograms (with gaps between -15 -> +15)
/*
const int HcalBaseClient::binmapd2[]={-42,-41,-40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,
				       -29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,
				       -16,-15,-9999, 15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,
				       30,31,32,33,34,35,36,37,38,39,40,41,42};

*/

// This stores eta binning in depth 3 (where HE is only present at a few ieta values)

const int HcalBaseClient::binmapd3[]={-28,-27,-9999,-16,-9999,16,-9999,27,28};

HcalBaseClient::HcalBaseClient()
{
  dbe_ =NULL;
  clientName_ = "GenericHcalClient";
}

HcalBaseClient::~HcalBaseClient(){}

// Mapping functions

void HcalBaseClient::mapHist1D(std::string file, std::string name, TH1F *hist[]){
  Hist1DContext *cont = new Hist1DContext();
  cont->file = file;
  cont->name = name;
  cont->hist = hist;
  pair<std::string, Hist1DContext> *newEntry = new pair<std::string, Hist1DContext>(name, *cont);
  histMap1D.insert(*newEntry);
}

void HcalBaseClient::mapHist2D(std::string file, std::string name, TH2F *hist[]){
  Hist2DContext *cont = new Hist2DContext();
  cont->file = file;
  cont->name = name;
  cont->hist = hist;
  pair<std::string, Hist2DContext> *newEntry = new pair<std::string, Hist2DContext>(name, *cont);
  histMap2D.insert(*newEntry);
}

void HcalBaseClient::mapHistProfile(std::string file, std::string name, TProfile *hist[]){
  HistProfileContext *cont = new HistProfileContext();
  cont->file = file;
  cont->name = name;
  cont->hist = hist;
  pair<std::string, HistProfileContext> *newEntry = new pair<std::string, HistProfileContext>(name, *cont);
  histMapProfile.insert(*newEntry);
}

void HcalBaseClient::init(const ParameterSet& ps, DQMStore* dbe, 
			  string clientName)
{
  dqmReportMapErr_.clear(); 
  dqmReportMapWarn_.clear(); 
  dqmReportMapOther_.clear();
  dqmQtests_.clear();
  badCells_.clear();
  dbe_ = dbe;
  ievt_=0; jevt_=0;
  clientName_ = clientName;
  LS_=0; oldLS_=0;

  // cloneME switch
  cloneME_ = ps.getUntrackedParameter<bool>("cloneME", true);
  
  // verbosity switch
  debug_ = ps.getUntrackedParameter<int>("debug", 0);
  if(debug_>0) std::cout << clientName_ <<" debugging switch is on"<<std::endl;

  // Online, offline switch
  Online_ = ps.getUntrackedParameter<bool>("Online",false);
  Nlumiblocks_ = ps.getUntrackedParameter<int>("Nlumiblocks",1000); 

  // timing switch
  showTiming_ = ps.getUntrackedParameter<bool>("showTiming",false); 

  // DQM default process name
  process_ = ps.getUntrackedParameter<string>("processName", "");
  rootFolder_ = ps.getUntrackedParameter<std::string>("subSystemFolder","Hcal");
  //Decide whether or not to fill unphysical iphi cells
  fillUnphysical_ = ps.getUntrackedParameter<bool>("fillUnphysicalIphi",true);
  
  // Known Bad Cells -- don't count?
  badCells_ =  ps.getUntrackedParameter<vector<string> >( "BadCells" );


  vector<string> subdets = ps.getUntrackedParameter<vector<string> >("subDetsOn");
  for(int i=0; i<4; i++)
    {
      subDetsOn_[i] = false;
    }

  for(unsigned int i=0; i<subdets.size(); i++)
    {
      if(subdets[i]=="HB") subDetsOn_[0] = true;
      else if(subdets[i]=="HE") subDetsOn_[1] = true;
      else if(subdets[i]=="HF") subDetsOn_[2] = true;
      else if(subdets[i]=="HO") subDetsOn_[3] = true;
    }
  
  // Define error palette, ranging from yellow for low to red for high. 
  for( int i=0; i<20; ++i )
    {
      //pcol_error_[19-i] = 901+i;
      TColor* color = gROOT->GetColor( 901+i );
      if( ! color ) color = new TColor( 901+i, 0, 0, 0, "" );
      color->SetRGB( 1.,
		     1.-.05*i,
		     0);
      pcol_error_[i]=901+i;
    } // for (int i=0;i<20;++i)

  return; 
} // void HcalBaseClient::init(const ParameterSet& ps, DQMStore* dbe, string clientName)

bool HcalBaseClient::vetoCell(HcalDetId& id)
{
  /*
    Function identifies whether cell with HcalDetId 'id' should be vetoed, 
    based on elements stored in  badCells_ array.
  */

  if(badCells_.size()==0) return false;
  for(unsigned int i = 0; i< badCells_.size(); ++i)
    {
      
      unsigned int badc = atoi(badCells_[i].c_str());
      if(id.rawId() == badc) return true;
    }
  return false;
} // bool HcalBaseClient::vetoCell(HcalDetId id)

void HcalBaseClient::SetLS(int ls)
{
  LS_ = ls;
  return;
}

void HcalBaseClient::errorOutput(){
  
  if(!dbe_) return;

  dqmReportMapErr_.clear(); dqmReportMapWarn_.clear(); dqmReportMapOther_.clear();
  
  for (map<string, string>::iterator testsMap=dqmQtests_.begin(); 
       testsMap!=dqmQtests_.end();testsMap++){
    string testName = testsMap->first;
    string meName = testsMap->second;
    MonitorElement* me = dbe_->get(meName);
    if(me){
      if (me->hasError()){
	vector<QReport*> report =  me->getQErrors();
	dqmReportMapErr_[meName] = report;
      }
      if (me->hasWarning()){
	vector<QReport*> report =  me->getQWarnings();
	dqmReportMapWarn_[meName] = report;
      }
      if(me->hasOtherReport()){
	vector<QReport*> report= me->getQOthers();
	dqmReportMapOther_[meName] = report;
      }
    }
  }

  if (debug_>0) std::cout << clientName_ << " Error Report: "<< dqmQtests_.size() << " tests, "<<dqmReportMapErr_.size() << " errors, " <<dqmReportMapWarn_.size() << " warnings, "<< dqmReportMapOther_.size() << " others" << std::endl;

  return;
}

void HcalBaseClient::getTestResults(int& totalTests, 
				    map<string, vector<QReport*> >& outE, 
				    map<string, vector<QReport*> >& outW, 
				    map<string, vector<QReport*> >& outO){
  this->errorOutput();
  //  outE.clear(); outW.clear(); outO.clear();

  for(map<string, vector<QReport*> >::iterator i=dqmReportMapErr_.begin(); i!=dqmReportMapErr_.end(); i++){
    outE[i->first] = i->second;
  }
  for(map<string, vector<QReport*> >::iterator i=dqmReportMapWarn_.begin(); i!=dqmReportMapWarn_.end(); i++){
    outW[i->first] = i->second;
  }
  for(map<string, vector<QReport*> >::iterator i=dqmReportMapOther_.begin(); i!=dqmReportMapOther_.end(); i++){
    outO[i->first] = i->second;
  }

  totalTests += dqmQtests_.size();

  return;
}


// ************************************************************************************************************ //

bool HcalBaseClient::validDetId(HcalSubdetector sd, int ies, int ip, int dp)
{
  // inputs are (subdetector, ieta, iphi, depth)
  // stolen from latest version of DataFormats/HcalDetId/src/HcalDetId.cc (not yet available in CMSSW_2_1_9)

  const int ie ( abs( ies ) ) ;

  return ( ( ip >=  1         ) &&
	   ( ip <= 72         ) &&
	   ( dp >=  1         ) &&
	   ( ie >=  1         ) &&
	   ( ( ( sd == HcalBarrel ) &&
	       ( ( ( ie <= 14         ) &&
		   ( dp ==  1         )    ) ||
		 ( ( ( ie == 15 ) || ( ie == 16 ) ) && 
		   ( dp <= 2          )                ) ) ) ||
	     (  ( sd == HcalEndcap ) &&
		( ( ( ie == 16 ) &&
		    ( dp ==  3 )          ) ||
		  ( ( ie == 17 ) &&
		    ( dp ==  1 )          ) ||
		  ( ( ie >= 18 ) &&
		    ( ie <= 20 ) &&
		    ( dp <=  2 )          ) ||
		  ( ( ie >= 21 ) &&
		    ( ie <= 26 ) &&
		    ( dp <=  2 ) &&
		    ( ip%2 == 1 )         ) ||
		  ( ( ie >= 27 ) &&
		    ( ie <= 28 ) &&
		    ( dp <=  3 ) &&
		    ( ip%2 == 1 )         ) ||
		  ( ( ie == 29 ) &&
		    ( dp <=  2 ) &&
		    ( ip%2 == 1 )         )          )      ) ||
	     (  ( sd == HcalOuter ) &&
		( ie <= 15 ) &&
		( dp ==  4 )           ) ||
	     (  ( sd == HcalForward ) &&
		( dp <=  2 )          &&
		( ( ( ie >= 29 ) &&
		    ( ie <= 39 ) &&
		    ( ip%2 == 1 )    ) ||
		  ( ( ie >= 40 ) &&
		    ( ie <= 41 ) &&
		    ( ip%4 == 3 )         )  ) ) ) ) ;



} // bool  HcalBaseClient::validDetId(HcalSubdetector sd, int ies, int ip, int dp)


void HcalBaseClient::getEtaPhiHists(std::string rootdir, std::string dir, std::string name, TH2F* h[4], std::string units)
{
  if (debug_>2) std::cout <<"HcalBaseClient::getting EtaPhiHists (2D)"<<std::endl;

  MonitorElement *me;
  stringstream hname;

  //cout <<"process_ = "<<process_.c_str()<<endl;

  hname <<process_.c_str()<<rootdir<<"/"<<dir<<"HB HE HF Depth 1 "<<name;
  if (!units.empty()) hname<<" "<<units;
  me= dbe_->get(hname.str().c_str());
  if (me!=0) 
    {
      h[0]=me->getTH2F();
      if (debug_>1) cout <<"Got EtaPhi Histogram "<<hname.str().c_str()<<endl;
    }
  else if (debug_>1) cout <<"SORRY, COULD NOT GET ETAPHI HIST "<<hname.str().c_str()<<endl;
  hname.str("");

  hname <<process_.c_str()<<rootdir<<"/"<<dir<<"HB HE HF Depth 2 "<<name;
  if (!units.empty()) hname<<" "<<units;
  me= dbe_->get(hname.str().c_str());
  if (me!=0) h[1]=me->getTH2F();
  hname.str("");

  hname <<process_.c_str()<<rootdir<<"/"<<dir<<"HE Depth 3 "<<name;
  if (!units.empty()) hname<<" "<<units;
  me= dbe_->get(hname.str().c_str());
  if (me!=0) h[2]=me->getTH2F();
  hname.str("");

  hname <<process_.c_str()<<rootdir<<"/"<<dir<<"HO Depth 4 "<<name;
  if (!units.empty()) hname<<" "<<units;
  me= dbe_->get(hname.str().c_str());
  if (me!=0) h[3]=me->getTH2F();
  hname.str("");

  if (debug_>2) std::cout <<"Finished with getEtaPhiHists(2D)"<<std::endl;
  return;
} // void HcalBaseClient::getEtaPhiHists(...)



void HcalBaseClient::getEtaPhiHists(std::string dir, std::string name, TH2F* h[4], std::string units)
{
  if (debug_>2) std::cout <<"HcalBaseClient::getting EtaPhiHists (2D)"<<std::endl;
  // If rootdir not specified, set it to "Hcal" by default
  getEtaPhiHists("Hcal",dir, name, h, units);
  return;
} // void HcalBaseClient::getEtaPhiHists(...)



void HcalBaseClient::getSJ6histos(std::string dir, std::string name, TH2F* h[6], std::string units)
{
  if (debug_>2) std::cout <<"HcalBaseClient::getting SJ6histos (2D)"<<std::endl;
  ostringstream hname;

  hname <<process_.c_str()<<dir<<"HB HF Depth 1 "<<name;
  if (!units.empty()) hname<<" "<<units;
  if (debug_>3) std::cout <<"name = "<<hname.str()<<std::endl;
  h[0]=getTH2F( hname.str(),process_, rootFolder_, dbe_,debug_,cloneME_);
  hname.str("");

  hname <<process_.c_str()<<dir<<"HB HF Depth 2 "<<name;
  if (!units.empty()) hname<<" "<<units;
  h[1]=getTH2F( hname.str(),process_, rootFolder_, dbe_,debug_,cloneME_);
  if (debug_>3) std::cout <<"name = "<<hname.str()<<std::endl;
  hname.str("");

  hname <<process_.c_str()<<dir<<"HE Depth 3 "<<name;
  if (!units.empty()) hname<<" "<<units;
  h[2]=getTH2F( hname.str(),process_, rootFolder_, dbe_,debug_,cloneME_);
  if (debug_>3) std::cout <<"name = "<<hname.str()<<std::endl;
  hname.str("");

  hname <<process_.c_str()<<dir<<"HO ZDC "<<name;
  if (!units.empty()) hname<<" "<<units;
  h[3]=getTH2F( hname.str(),process_, rootFolder_, dbe_,debug_,cloneME_);
  if (debug_>3) std::cout <<"name = "<<hname.str()<<std::endl;
  hname.str("");

  hname <<process_.c_str()<<dir<<"HE Depth 1 "<<name;
  if (!units.empty()) hname<<" "<<units;
  h[4]=getTH2F( hname.str(),process_, rootFolder_, dbe_,debug_,cloneME_);
  if (debug_>3) std::cout <<"name = "<<hname.str()<<std::endl;
  hname.str("");

  hname <<process_.c_str()<<dir<<"HE Depth 2 "<<name;
  if (!units.empty()) hname<<" "<<units;
  h[5]=getTH2F( hname.str(),process_, rootFolder_, dbe_,debug_,cloneME_);
  if (debug_>3) std::cout <<"name = "<<hname.str()<<std::endl;
  hname.str("");

  if (debug_>2) std::cout <<"Finished with getSJ6histos(2D)"<<std::endl;
  return;
} // void HcalBaseClient::getSJ6histos(2D)



void HcalBaseClient::getSJ6histos(std::string dir, std::string name, TH1F* h[4], std::string units)
{
  ostringstream hname;

  hname <<process_.c_str()<<dir<<"HB "<<name;
  if (!units.empty()) hname << " "<<units;
  h[0]=getTH1F( hname.str(),process_, rootFolder_, dbe_,debug_,cloneME_);
  hname.str("");

  hname <<process_.c_str()<<dir<<"HE "<<name;
  if (!units.empty()) hname << " "<<units;
  h[1]=getTH1F( hname.str(),process_, rootFolder_, dbe_,debug_,cloneME_);
  hname.str("");

  hname <<process_.c_str()<<dir<<"HO "<<name;
  if (!units.empty()) hname << " "<<units;
  h[2]=getTH1F( hname.str(),process_, rootFolder_, dbe_,debug_,cloneME_);
  hname.str("");

  hname <<process_.c_str()<<dir<<"HF "<<name;
  if (!units.empty()) hname << " "<<units;
  h[3]=getTH1F( hname.str(),process_, rootFolder_, dbe_,debug_,cloneME_);
  hname.str("");
  return;
} // void HcalBaseClient::getSJ6histos(1D)

/*
int HcalBaseClient::CalcIeta(int eta, int depth)
{
  // returns ieta value give an eta counter.
  // eta runs from 0...X  (X depends on depth)
  int ieta=-9999;
  if (eta<0) return ieta;
  if (depth==1)
    {
      ieta=eta-42; // default shift: bin 0 corresponds to a histogram ieta of -42 (which is offset by 1 from true HF value of -41)
      if (eta<13) ieta++;
      else if (eta>71) ieta--;
      if (ieta==0) ieta=-9999;
      return ieta;
    }
  else if (depth==2)
    {
      if (eta>57) return -9999;
      else
	{
	  ieta=binmapd2[eta];
	  if (ieta==-9999) return ieta;
	  if (ieta==0) return -9999;
	  if (ieta==17 || ieta == -17) return -9999; // no depth 2 cells at |ieta| = 17
	  else if (ieta<=-30) ieta++;
	  else if (ieta>=30) ieta--;
	  return ieta;
	}
    }
  else if (depth==3)
    {
      if (eta>8) return -9999;
      else
	ieta=binmapd3[eta];
      if (ieta==0) return -9999;
      return ieta;
    }
  else if (depth==4)
    {
      ieta= eta-15;  // bin 0 is ieta=-15, all bins increment normally from there
      if (abs(ieta)>15) return -9999;
      if (ieta==0) return -9999;
      return ieta;
    }
  return ieta; // avoids compilation warning
}

bool HcalBaseClient::isHB(int etabin, int depth)
{
  if (depth>2) return false;
  else if (depth<1) return false;
  else
    {
      int ieta=CalcIeta(etabin,depth);
      if (ieta==-9999) return false;
      if (depth==1)
        {
          if (abs(ieta)<=16 ) return true;
          else return false;
        }
      else if (depth==2)
        {
          if (abs(ieta)==15 || abs(ieta)==16) return true;
          else return false;
        }
    }
  return false;
}

bool HcalBaseClient::isHE(int etabin, int depth)
{
  if (depth>3) return false;
  else if (depth<1) return false;
  else
    {
      int ieta=CalcIeta(etabin,depth);
      if (ieta==-9999) return false;
      if (depth==1)
        {
          if (abs(ieta)>=17 && abs(ieta)<=28 ) return true;
          if (ieta==-29 && etabin==13) return true; // HE -29
          if (ieta==29 && etabin == 71) return true; // HE +29
        }
      else if (depth==2)
        {
          if (abs(ieta)>=17 && abs(ieta)<=28 ) return true;
          if (ieta==-29 && etabin==13) return true; // HE -29
          if (ieta==29 && etabin == 43) return true; // HE +29
        }
      else if (depth==3)
        return true;
    }
  return false;
}

bool HcalBaseClient::isHF(int etabin, int depth)
{
  if (depth>2) return false;
  else if (depth<1) return false;
  else
    {
      int ieta=CalcIeta(etabin,depth);
      if (ieta==-9999) return false;
      if (depth==1)
        {
          if (ieta==-29 && etabin==13) return false; // HE -29
          else if (ieta==29 && etabin == 71) return false; // HE +29
          else if (abs(ieta)>=29 ) return true;
        }
      else if (depth==2)
        {
          if (ieta==-29 && etabin==13) return false; // HE -29
          else if (ieta==29 && etabin==43) return false; // HE +29
          else if (abs(ieta)>=29 ) return true;
        }
    }
  return false;
}

bool HcalBaseClient::isHO(int etabin, int depth)
{
  if (depth!=4) return false;
  int ieta=CalcIeta(etabin,depth);
  if (ieta!=-9999) return true;
  return false;
}
*/


void HcalBaseClient::SetEtaPhiLabels(MonitorElement* h)
{
  std::stringstream label;
  for (int i=-41;i<=-29;i=i+2)
    {
      label<<i;
      h->setBinLabel(i+42,label.str().c_str());
      label.str("");
    }
  h->setBinLabel(14,"-29HE");
    
  // offset by one for HE
  for (int i=-27;i<=27;i=i+2)
    {
      label<<i;
      h->setBinLabel(i+43,label.str().c_str());
      label.str("");
    }
  h->setBinLabel(72,"29HE");
  for (int i=29;i<=41;i=i+2)
    {
      label<<i;
      h->setBinLabel(i+44,label.str().c_str());
      label.str("");
    }
  return;
}
