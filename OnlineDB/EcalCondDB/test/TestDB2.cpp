#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include "OnlineDB/EcalCondDB/interface/EcalCondDBInterface.h"
#include "OnlineDB/EcalCondDB/interface/all_lmf_types.h"
#include "OnlineDB/EcalCondDB/interface/RunDat.h"
#include "OnlineDB/EcalCondDB/interface/RunList.h"
#include "OnlineDB/EcalCondDB/interface/RunPTMTempDat.h"
#include "TROOT.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH2F.h"
#include "TF1.h"


using namespace std;

class CondDBApp {
public:

  /**
   *   App constructor; Makes the database connection
   */
  CondDBApp(string host, string sid, string user, string pass, int port)
  {
    try {
      cout << "Making connection...to " << port << flush;
      econn = new EcalCondDBInterface( host, sid, user, pass, port );
      cout << "Done." << endl;
    } catch (runtime_error &e) {
      cerr << e.what() << endl;
      exit(-1);
    }

 
  }
  CondDBApp(string sid, string user, string pass)
  {
    try {
      cout << "Making connection...to " << sid << endl;
      econn = new EcalCondDBInterface(  sid, user, pass );
      cout << "Done." << endl;
    } catch (runtime_error &e) {
      cerr << e.what() << endl;
      exit(-1);
    }

 
  }

  CondDBApp(string host, string sid, string user, string pass)
  {
    try {
      cout << "Making connection..." << flush;
      econn = new EcalCondDBInterface( host, sid, user, pass );
      cout << "Done." << endl;
    } catch (runtime_error &e) {
      cerr << e.what() << endl;
      exit(-1);
    } 
  }

  /**
   *  App destructor;  Cleans up database connection
   */
  ~CondDBApp() 
  {
    delete econn;
  }

  inline std::string to_string( char value[])
  {
    std::ostringstream streamOut;
    streamOut << value;
    return streamOut.str();    
  }
  

  void test(int runa, int runb ) {
    cout << "Retrieving run list from DB ... " << endl;
    RunList my_runlist ;
    RunTag  my_runtag;
    LocationDef my_locdef;
    RunTypeDef my_rundef;
    
    my_locdef.setLocation("H4B");
    my_rundef.setRunType("LASER");
    my_runtag.setLocationDef(my_locdef);
    my_runtag.setRunTypeDef(my_rundef);
    my_runtag.setGeneralTag("LASER");

    f = new TFile("./laser.root", "RECREATE");

    mataq_vs_run = new TH2F("MATAQ_vs_run","MATAQ vs Run number", 2500, 13000, 15500, 100, 0., 100);
    apd_pn_mean_vs_run = new TH2F("apd_pn_mean_vs_run","apd_pn_mean_vs_run", 2500, 13000, 15500, 100, .0, 3.0);


    my_runlist=econn->fetchRunList(my_runtag);

    std::vector<RunIOV> run_vec=  my_runlist.getRuns();


    
    /*
      gStyle->SetOptStat(0);
      gStyle->SetOptFit();
      gStyle->SetPalette(1,0);
      c1 = new TCanvas("c1","The Temperatures",200,10,600,400);
      c1->SetGrid();
      
      TDatime da(2005,01,01,00,00,00);
      gStyle->SetTimeOffset(da.Convert());
    */
    


    cout <<"number of runs is : "<< run_vec.size()<< endl;
    int nruns=run_vec.size();
    if(nruns>0){
      
      cout << "here is first run : "<< run_vec[0].getRunNumber() << endl;
      // cout << "here is the run Start for first run ="<< run_vec[0].getRunStart()<< endl;
      // cout << "here is the run End for first run =" <<run_vec[0].getRunEnd()<< endl;

    }

    // here we retrieve the laser Monitoring farm run processing records

   
    LMFRunTag lmf_tag;
    lmf_tag.setGeneralTag("default");
    LMFRunList lmf_list;
    lmf_list.setLMFRunTag(lmf_tag);
    lmf_list.setRunTag(my_runtag);
    //    lmf_list=econn->fetchLMFRunList(my_runtag, lmf_tag);
    int min_run=0;
    int max_run=15000;
    lmf_list=econn->fetchLMFRunList(my_runtag, lmf_tag,min_run,max_run );
    // lmf_list=econn->fetchLMFRunList(my_runtag, lmf_tag);
    
    std::vector<LMFRunIOV> lmf_run_vec=  lmf_list.getRuns();

    cout <<"number of LMF runs is : "<< lmf_run_vec.size()<< endl;
    int lmf_runs=(int)lmf_run_vec.size();
    if(lmf_runs>0){
      
      cout << "here is first sub run : "<< lmf_run_vec[0].getSubRunNumber() << endl;
      cout << "here is the run number: "<< lmf_run_vec[0].getRunIOV().getRunNumber() << endl;
      //  cout << "here is the run Start for first run ="<< lmf_run_vec[0].getSubRunStart()<< endl;
      // cout << "here is the run End for first run =" <<lmf_run_vec[0].getSubRunEnd()<< endl;

    }

    int sm_num=0;  

    if(lmf_runs>0){
      int run=runa;

      cout <<" retrieve the data for a given run"<< endl;
      // retrieve the data for a given run
      cout << "here is the run number: "<< lmf_run_vec[run].getRunIOV().getRunNumber() << endl;
      cout << "intrinsic counter: " << run << endl;
      
      RunIOV runiov_prime = lmf_run_vec[run].getRunIOV();
      
      
      cout <<" retrieve the APD/PN for a given run"<< endl;

      Tm startTm3;
      startTm3.setToCurrentGMTime();
      cout << "query starting at:    " << startTm3.str() << endl;


      // retrieve APD/PN for this run 
      map<EcalLogicID, LMFLaserBlueNormDat> dataset_lmf;
      econn->fetchDataSet(&dataset_lmf, &lmf_run_vec[run]);

      Tm startTm4;
      startTm4.setToCurrentGMTime();
      cout << "query starting at:    " << startTm4.str() << endl;


      typedef map<EcalLogicID, LMFLaserBlueNormDat>::const_iterator CIlmf;
      EcalLogicID ecid_xt;
      LMFLaserBlueNormDat  rd_apdnorm;
      
      sm_num=0;
      float apd_pn[61200];
      for (int i=0; i<61201; i++){
	apd_pn[i]=0;
      }

      float sum_apd_pn=0;
      for (CIlmf p = dataset_lmf.begin(); p != dataset_lmf.end(); p++) {
	ecid_xt = p->first;
	rd_apdnorm  = p->second;
	sm_num=ecid_xt.getID1();
	int xt_num=ecid_xt.getID2();
	apd_pn[xt_num]=rd_apdnorm.getAPDOverPNMean();
	sum_apd_pn+=apd_pn[xt_num];
       if(xt_num==123&&sm_num==10) cout <<"here is one value for XT: "<< xt_num<<" " << apd_pn[xt_num] << endl;
      }
      sum_apd_pn=sum_apd_pn/61200;
      apd_pn_mean_vs_run->Fill(lmf_run_vec[run].getRunIOV().getRunNumber(),sum_apd_pn);
      //cout <<"got APD over PN data" << endl;
      // cout <<"here is one value: " << apd_pn[1] << endl;
      
      // retrieve MATAQ for this run 
      map<EcalLogicID, LMFMatacqBlueDat> dataset_mq;
      econn->fetchDataSet(&dataset_mq, &lmf_run_vec[run]);
      typedef map<EcalLogicID, LMFMatacqBlueDat>::const_iterator CImq;
      LMFMatacqBlueDat  rd_mq;
      float mq_width=0;
      float mq_height=0;
      float mq_time=0;

      for (CImq p = dataset_mq.begin(); p != dataset_mq.end(); p++) {
	ecid_xt = p->first;
	rd_mq  = p->second;
	mq_width=rd_mq.getWidth(); 
	mq_height=rd_mq.getAmplitude();
	mq_time=rd_mq.getTimeOffset();
	mataq_vs_run->Fill(lmf_run_vec[run].getRunIOV().getRunNumber(), mq_width);
      }
      cout <<"got MATAQ data" << endl;
      //  cout <<"here is one value of MQ width: " << mq_width << endl;
      
      // retrieve PTM Temp for this run 

     
    }

    apd_pn_mean_vs_run->Write();
    mataq_vs_run->Write();
    f->Close();

  }



private:
  CondDBApp();  // hidden default constructor
  EcalCondDBInterface* econn;
  string locations[4];
  uint64_t startmicros;
  uint64_t endmicros;
  run_t startrun;
  run_t endrun;

  TFile* f;
  TH2F* mataq_vs_run;
  TH2F* apd_pn_mean_vs_run;

  void printTag( const RunTag* tag) const
  {
    cout << endl;
    cout << "=============RunTag:" << endl;
    cout << "GeneralTag:         " << tag->getGeneralTag() << endl;
    cout << "Location:           " << tag->getLocationDef().getLocation() << endl;
    cout << "Run Type:           " << tag->getRunTypeDef().getRunType() << endl;
    cout << "====================" << endl;
  }

  void printIOV( const RunIOV* iov) const
  {
    cout << endl;
    cout << "=============RunIOV:" << endl;
    RunTag tag = iov->getRunTag();
    printTag(&tag);
    cout << "Run Number:         " << iov->getRunNumber() << endl;
    cout << "Run Start:          " << iov->getRunStart().str() << endl;
    cout << "Run End:            " << iov->getRunEnd().str() << endl;
    cout << "====================" << endl;
  }
};

int main (int argc, char* argv[])
{
  string sid;
  string user;
  string pass;
  string smin_run;
  string sn_run;

  if (argc != 6) {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " <SID> <user> <pass> <min_run> <n_run>" << endl;
    exit(-1);
  }


  sid = argv[1];
  user = argv[2];
  pass = argv[3];
  smin_run = argv[4];
  int min_run=atoi(smin_run.c_str());
  sn_run = argv[5];
  int n_run=atoi(sn_run.c_str())+min_run;


  try {
    CondDBApp app( sid, user, pass);

    app.test(min_run, n_run);
  } catch (exception &e) {
    cout << "ERROR:  " << e.what() << endl;
  } catch (...) {
    cout << "Unknown error caught" << endl;
  }

  cout << "All Done." << endl;

  return 0;
}
