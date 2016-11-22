#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include "OnlineDB/EcalCondDB/interface/EcalCondDBInterface.h"
#include "OnlineDB/EcalCondDB/interface/all_lmf_types.h"
#include "OnlineDB/EcalCondDB/interface/RunDat.h"
#include "OnlineDB/EcalCondDB/interface/RunLaserRunDat.h"
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
                                                                                

  RunIOV makeRunIOV(int run_num, std::string& location)
  {
                                                                                
  // The objects necessary to identify a dataset
  LocationDef locdef;
  RunTypeDef rundef;
  RunTag runtag;

  locdef.setLocation(location);

  rundef.setRunType("TEST");

  runtag.setLocationDef(locdef);
  runtag.setRunTypeDef(rundef);

  // Our beginning time will be the current GMT time
  // This is the time zone that should be written to the DB!
  // (Actually UTC)
  Tm startTm;
  startTm.setToCurrentGMTime();

  uint64_t microseconds = startTm.microsTime();
  startmicros = microseconds;
  run_t run = run_num;
  startrun = run;

  cout << "Starting Time:    " << startTm.str() << endl;
  cout << "Starting Micros:  " << startmicros << endl;
  cout << "Starting Run:     " << startrun << endl;

  // Set the properties of the iov
  RunIOV runiov;

  startTm.setToMicrosTime(microseconds);
  cout << "Setting run " << run << " run_start " << startTm.str() << endl;
  runiov.setRunNumber(run);
  runiov.setRunStart(startTm);
  runiov.setRunTag(runtag);

  return runiov;
}


LMFRunIOV makeLMFRunIOV(RunIOV* runiov)
{
  // LMF Tag and IOV

  LMFRunTag lmftag;

  LMFRunIOV lmfiov;
  lmfiov.setLMFRunTag(lmftag);
  lmfiov.setRunIOV(*runiov);
  lmfiov.setSubRunNumber(1);
  lmfiov.setSubRunStart(runiov->getRunStart());

  Tm startTm;
  startTm.setToCurrentGMTime();
  uint64_t microseconds = startTm.microsTime();
  startmicros = microseconds;
  startTm.setToMicrosTime(microseconds);


  lmfiov.setSubRunEnd(startTm);
  lmfiov.setSubRunType("Standard");





  return lmfiov;
}







  /**
   *  Write LMFLaserBlueRawDat objects with associated RunTag and RunIOVs
   *  IOVs are written using automatic updating of the 'RunEnd', as if
   *  the time of the end of the run was not known at the time of writing.
   */
  void testWrite(int run_number)
  {

    std::string location="P5"; // this is the location 
    
    RunIOV runiov = this->makeRunIOV(run_number, location);
    RunTag runtag = runiov.getRunTag();
    run_t run = runiov.getRunNumber();
    
    // write to the DB
    cout << "Inserting run..." << flush;
    econn->insertRunIOV(&runiov);
    cout << "Done." << endl;
    printIOV(&runiov);
    
    // LMF Tag and IOV
    LMFRunIOV lmfiov = this->makeLMFRunIOV(&runiov);
    // the LMF run iov is inserted together with the data when the data are inserted. 

    // here we write the configuration used for the laser run  
    RunLaserRunDat rd;
    rd.setLaserSequenceType("STANDARD");
    rd.setLaserSequenceCond("STANDALONE");
    // or eventually this:
    // rd.setSequenceType("IN_THE_GAP");
    map<EcalLogicID, RunLaserRunDat> dataset;
    // every _DAT table needs a logic_id here we use the all ECAL one if the 
    // laser run was taken on all ECAL 
    EcalLogicID ecid_allEcal;
    ecid_allEcal = econn->getEcalLogicID("ECAL");
    dataset[ecid_allEcal]=rd;
    econn->insertDataSet(&dataset, &runiov);


    map<EcalLogicID, LMFLaserBluePrimDat> dataset_lmf;
    map<EcalLogicID, LMFLaserBluePulseDat> dataset_mq;
    map<EcalLogicID, LMFPNBluePrimDat> dataset_lpn;

    // here we load all the EcalLogicId we need for the crystals
    // the vector is always ordered by the most internal id to the most external
    // in increasing order  
    cout <<"now loading the channel numbers in the barrel"<< endl; 
    vector<EcalLogicID> ecid;
    ecid = econn->getEcalLogicIDSet("EB_crystal_number", 1, 36, 1, 1700);

    int nullid=EcalLogicID::NULLID;

    vector<EcalLogicID> ecid_pna_map;
    ecid_pna_map = econn->getEcalLogicIDSetOrdered("EB_LM_PNA", 1, 36, 0, 9,nullid, 
					   nullid, "EB_crystal_number",4);
    vector<EcalLogicID> ecid_pnb_map;
    ecid_pnb_map = econn->getEcalLogicIDSetOrdered("EB_LM_PNB", 1, 36, 0, 9,nullid,
					   nullid, "EB_crystal_number",4);

    vector<EcalLogicID> ecid_pn;
    ecid_pn = econn->getEcalLogicIDSet("EB_LM_PN", 1,36, 1,10);
    // done 
    cout <<"done loading the channel numbers in the barrel" << endl; 

    Int_t cx, cy;

    TFile *f;
    TFile *f_mq;
    
    ostringstream Run;
    ostringstream sROOT; 
    ostringstream sROOT_mq; 

    Run.str("");
    Run << "0000" << run;

    sROOT.str("");
    sROOT_mq.str("");
    // this is for when they will store the file with the SM number 
    // for now all are stored fas if Sm22
    //    sROOT << "/data1/daq/crogan/ROOT_files/SM" << sm_num << "/SM" << sm_num  << "-0000";
    sROOT << "./SM22-0000";
    sROOT_mq << "./MATACQ-SM22-0000";
    sROOT << run << ".root";
    sROOT_mq << run << ".root";

    cout << "opening file "<< sROOT.str().c_str() << endl;
    f = (TFile*) new TFile(sROOT.str().c_str());
    //    TDirectory *DIR = (TDirectory*) f->GetDirectory(Run.str().c_str());
    TDirectory *DIR = (TDirectory*) f->GetDirectory(Run.str().c_str());

    int sm_num=1; // we write just 1 SM 
    // make a loop if you want to write more 

    if(DIR != NULL){
      std::cout<< "we are entering the root file "<<endl;
      TH2F *APDPN = (TH2F*) DIR->Get("APDPN");
      TH2F *APD = (TH2F*) DIR->Get("APD");
      TH2F *APD_RMS = (TH2F*) DIR->Get("APD_RMS");
      TH2F *APDPN_RMS = (TH2F*) DIR->Get("APDPN_RMS");
      TH2F *PN = (TH2F*) DIR->Get("PN");
      if(APDPN !=NULL && APD !=NULL && APD_RMS !=NULL && APDPN_RMS!=NULL && PN !=NULL ){

	int pn_chan[]={1,101, 501, 901, 1301, 11, 111, 511, 911, 1311 };

	for (int c=1; c<1701; c++){
	  // the channels are turned in phi and eta 
	  // with respect to the standard numbering scheme
	  cx = 85-(c-1)/20;
	  cy = 20-(c-1)%20;
	  float apdpn = (float) APDPN->GetBinContent(cx,cy);
	  float apd = (float) APD->GetBinContent(cx,cy);
	  float apd_rms = (float) APD_RMS->GetBinContent(cx,cy);
	  float apdpn_rms = (float) APDPN_RMS->GetBinContent(cx,cy);
	  float pn = (float) PN->GetBinContent(cx,cy);
	  
	  if(c%20==0) cout << "channel "<< c<< " value "<< apdpn << endl;
	  int hi=(sm_num-1)*1700+c;
	  // the valid logic_id for this crystal is  ecid[hi] 
	   

	  // Set the data
	  LMFLaserBluePrimDat bluelaser;
	  

	  bluelaser.setFlag(1 );
	  bluelaser.setMean(apd );
	  bluelaser.setRMS( apd_rms );
	  bluelaser.setPeak( apd );
	  
	  bluelaser.setAPDOverPNAMean( apdpn );
	  bluelaser.setAPDOverPNARMS(apdpn_rms );
	  bluelaser.setAPDOverPNAPeak( apdpn );
	  
	  bluelaser.setAPDOverPNBMean(apdpn  );
	  bluelaser.setAPDOverPNBRMS(apdpn_rms  );
	  bluelaser.setAPDOverPNBPeak(apdpn);
	  
	  bluelaser.setAPDOverPNMean( apdpn );
	  bluelaser.setAPDOverPNRMS( apdpn_rms );
	  bluelaser.setAPDOverPNPeak( apdpn );
	  
	  bluelaser.setAlpha(43. ); // this was not in the file 
	  bluelaser.setBeta( 120. ); // this was not in the file
	  
	  // Fill the dataset
	  EcalLogicID ec=ecid[hi];
	  dataset_lmf[ec] = bluelaser;



	  // new: example how to retrieve the right PN 
	  //	  int pna_sm=ecid_pna_map[hi].getID1();
	  int pna_num=ecid_pna_map[hi].getID2();


	  // int pnb_sm=ecid_pnb_map[hi].getID1();
	  int pnb_num=ecid_pnb_map[hi].getID2();
 
	  std::cout << "this is the right PN number xtal:"<<c <<" PNA="<<pna_num<< " PNB="<<pnb_num<<endl;  

	  // unfortunately in the file the PN data were written once per each crystal
	  // so I anyway have to use the trick to save the PN only once per fanout and not 1700 times 


	  // we fill the PN data just once per fanout
	  int ispn=-1;
	  for(int j=0; j<10; j++){
	    if(pn_chan[j]==c) ispn=j;
	  }
	  if(ispn!=-1){
	    LMFPNBluePrimDat bluepn;
	    bluepn.setFlag(1); // set the PN flag
	    bluepn.setMean(pn);
	    bluepn.setPeak(pn);
	    bluepn.setRMS(pn/100.);
	    bluepn.setPNAOverPNBMean(1.);
	    bluepn.setPNAOverPNBRMS(1.);
	    bluepn.setPNAOverPNBPeak(1.);
	    int hipn=(sm_num-1)*10+ispn;
	    EcalLogicID epn=ecid_pn[hipn];
	    dataset_lpn[epn] = bluepn;
	  }


	}
      }

    }
    f->Close();
    cout << "finished processing the APD and PN data"  << endl;

    cout << "opening MATACQ file "<< sROOT_mq.str().c_str() << endl;

    try {
 
      boolean mataq=false; 

      f_mq = (TFile*) new TFile(sROOT_mq.str().c_str());
      if (f_mq->IsZombie()|| !mataq) {
	cout << "Error opening MATACQ file maybe it does not exist" << endl;
      } else {
	
	TH1F *mq_height = (TH1F*) f_mq->Get("Height_Channel_0_blue");
	TH1F *mq_width = (TH1F*) f_mq->Get("Width_Channel_0_blue");
	TH1F *mq_timing = (TH1F*) f_mq->Get("Timing_Channel_0_blue");
	
	mq_height->Fit("gaus");   
	TF1 *fit_height= mq_height->GetFunction("gaus");
	float par_height= (float) fit_height ->GetParameter(1);
	
	mq_width->Fit("gaus");   
	TF1 *fit_width= mq_width->GetFunction("gaus");
	float par_width=(float) fit_width ->GetParameter(1);
	
	mq_timing->Fit("gaus");   
	TF1 *fit_timing= mq_timing->GetFunction("gaus");
	float par_timing=(float) fit_timing ->GetParameter(1);
	
	
	LMFLaserBluePulseDat lmf_mq;
	EcalLogicID ecid_mq;
	ecid_mq = econn->getEcalLogicID("ECAL"); // the full ECAL or to be changed for just a fanout 
	//	lmf_mq.setFitMethod( "POLYNOMIAL" );
	lmf_mq.setFitMethod( 0 );
	lmf_mq.setAmplitude( par_height );
	lmf_mq.setTime( par_timing );
	lmf_mq.setRise( par_timing );
	lmf_mq.setFWHM( par_width );
	lmf_mq.setFW20( par_width );
	lmf_mq.setFW80( par_width );
	lmf_mq.setSliding( par_height );
	dataset_mq[ecid_mq] = lmf_mq;
	econn->insertDataSet(&dataset_mq, &lmfiov);
	
	f_mq->Close();
	
	
	cout << "Done Matacq "  << endl;
	
      } 
    } catch (...) {
      cout << " " << endl ;
    }
    

    // Insert the dataset, identifying by iov
    cout << "Inserting dataset..." << flush;
    econn->insertDataArraySet(&dataset_lmf, &lmfiov);
    econn->insertDataArraySet(&dataset_lpn, &lmfiov);
    cout << "Done." << endl;

 
  };




private:
  CondDBApp();  // hidden default constructor
  EcalCondDBInterface* econn;

  uint64_t startmicros;
  uint64_t endmicros;
  run_t startrun;
  run_t endrun;

  /**
   *   Iterate through the dataset and print some data
   */
  void printDataSet( const map<EcalLogicID, LMFLaserBluePrimDat>* dataset, int limit = 0 ) const
  {
    cout << "==========printDataSet()" << endl;
    if (dataset->size() == 0) {
      cout << "No data in map!" << endl;
    }
    EcalLogicID ecid;
    LMFLaserBluePrimDat bluelaser;

    int count = 0;
    typedef map< EcalLogicID, LMFLaserBluePrimDat >::const_iterator CI;
    for (CI p = dataset->begin(); p != dataset->end(); p++) {
      count++;
      if (limit && count > limit) { return; }
      ecid = p->first;
      bluelaser  = p->second;
     
      cout << "SM:                     " << ecid.getID1() << endl;
      cout << "Xtal:                   " << ecid.getID2() << endl;
      cout << "APD Peak:               " << bluelaser.getPeak() << endl;
      cout << "========================" << endl;
    }
    cout << endl;
  }


  /**
   *   Print out a RunTag
   */
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
  string host;
  string sid;
  string user;
  string pass;
  string run_num;

  if (argc != 6) {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " <host> <SID> <user> <pass>" << endl;
    exit(-1);
  }

  host = argv[1];
  sid = argv[2];
  user = argv[3];
  pass = argv[4];
  run_num= argv[5];

  try {
    CondDBApp app(host, sid, user, pass);
    int run=atoi(run_num.c_str());
    app.testWrite(run);
  } catch (exception &e) {
    cout << "ERROR:  " << e.what() << endl;
  } catch (...) {
    cout << "Unknown error caught" << endl;
  }

  cout << "All Done." << endl;

  return 0;
}
