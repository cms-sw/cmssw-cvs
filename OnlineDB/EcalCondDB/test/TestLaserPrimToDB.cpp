#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include <TROOT.h>
#include <TChain.h>
#include <TH2I.h>
#include <TH2F.h>
#include <TFile.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>

#include "OnlineDB/EcalCondDB/interface/EcalCondDBInterface.h"
#include "OnlineDB/EcalCondDB/interface/all_lmf_types.h"
#include "OnlineDB/EcalCondDB/interface/RunDat.h"
#include "OnlineDB/EcalCondDB/interface/RunLaserRunDat.h"

// fixme
#include "OnlineDB/EcalCondDB/interface/LMFLaserPrimDat.h"
#include "OnlineDB/EcalCondDB/interface/LMFLaserPNPrimDat.h"
#include "OnlineDB/EcalCondDB/interface/LMFLaserPulseDat.h"

#include "MELaserPrim.hh"

class CondDBApp 
{
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
                                                                                

  RunIOV makeRunIOV(int run_num, int ts, std::string& location)
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
    Tm startTm( ((uint64_t)ts)*1000000 );       // translate to microseconds
    Tm   endTm( ((uint64_t)ts+1800)*1000000 );  // half an hour after the start of the run...
    //  startTm.setToCur<rentGMTime();

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
    runiov.setRunEnd(endTm);
    runiov.setRunTag(runtag);

    return runiov;
  }


  LMFRunIOV makeLMFRunIOV( RunIOV* runiov, int subrunnumber )
  {
    // LMF Tag and IOV

    LMFRunTag lmftag;

    LMFRunIOV lmfiov;
    lmfiov.setLMFRunTag(lmftag);
    lmfiov.setRunIOV(*runiov);
    lmfiov.setSubRunNumber( subrunnumber );
    lmfiov.setSubRunStart( runiov->getRunStart());

    Tm startTm;
    startTm.setToCurrentGMTime();
    uint64_t microseconds = startTm.microsTime();
    startmicros = microseconds;
    startTm.setToMicrosTime(microseconds);
    
    lmfiov.setSubRunEnd(startTm);
    lmfiov.setSubRunType("Standard");

    return lmfiov;
  }


  // test for GHM
  void testGhm( const char* pathStr_, const char* regionStr_, unsigned int run_, unsigned int ts_ )
  {

    //
    // get logic id for all ECAL
    //
    EcalLogicID ecid_allEcal;
    ecid_allEcal = econn->getEcalLogicID("ECAL");
    
    std::string location="H4"; 
    RunIOV runiov = this->makeRunIOV( run_, ts_, location );
    //    RunTag runtag = runiov.getRunTag();
    cout << "Inserting RunIOV..." << flush;
    econn->insertRunIOV(&runiov);
    cout << "Done." << endl;
    printIOV(&runiov);

    //
    // Laser Run
    //
    map< EcalLogicID, RunLaserRunDat >    dataset;
    RunLaserRunDat rd;
    rd.setLaserSequenceType("STANDARD");
    rd.setLaserSequenceCond("STANDALONE");
    // or eventually this:
    // rd.setSequenceType("IN_THE_GAP");
    dataset[ecid_allEcal]=rd;
    econn->insertDataSet( &dataset, &runiov );

    int logic_id;

    // LMF Tag and IOV
    int subrunnumber = 0;

    // int color=3;  // fixme -- there are only BLUE classes 
    for( int color=0; color<4; color=color+3 )
      {
	TString colorStr;
	switch( color )
	  {
	  case MELaserPrim::iBlue:  colorStr = "BLUE"; break;
	  case MELaserPrim::iIRed:  colorStr = "IRED";  break;
	  default: abort();
	  }

	for( int side=0; side<2; side++ )
	  {

	    subrunnumber = 10*color + side;
	    //subrunnumber ++;
	    LMFRunIOV lmfiov = this->makeLMFRunIOV( &runiov, subrunnumber );

	    // datasets
	    //	    typedef LMFPNBluePrimDat LMFLaserBluePNPrimDat;  // name of class should be fixed
	    map< EcalLogicID, LMFRunDat             >    dataset_lmfrun;
	    map< EcalLogicID, LMFLaserConfigDat     >    dataset_config;
	    //	    map< EcalLogicID, LMFLaserBluePrimDat   >      dataset_prim;
	    map< EcalLogicID, LMFLaserPrimDat   >          dataset_prim;
	    //	    map< EcalLogicID, LMFLaserBluePNPrimDat >    dataset_pnprim; 
	    map< EcalLogicID, LMFLaserPNPrimDat >        dataset_pnprim; 
	    //	    map< EcalLogicID, LMFLaserBluePulseDat  >     dataset_pulse;
	    map< EcalLogicID, LMFLaserPulseDat  >     dataset_pulse;

	    //
	    //  Open the ROOT file (Warning: the data is on pcsaccms03)
	    //
	    TString cur( pathStr_ );
	    if( !cur.EndsWith("/") ) cur+="/";
	    TString rootfile(cur);
	    rootfile += "LMF_";
	    rootfile += regionStr_;
	    rootfile += "-"; rootfile += side;
	    rootfile += "_"; 
	    switch( color )
	      {
	      case MELaserPrim::iBlue: rootfile += "Blue"; break;
	      case MELaserPrim::iIRed: rootfile += "IRed"; break;
	      default: abort();
	      }
	    //	    rootfile += (color==0) ? "Blue":"Red"; 
	    rootfile += "Laser";
	    rootfile += "_Run"; rootfile += run_;
	    rootfile += "_TS"; rootfile += ts_;
	    rootfile += ".root";
	    cout << "Open root file " << rootfile << endl;
	    TDirectory* f = TFile::Open( rootfile );
	    if( f==0 ) abort();
	    
	    TString sep("__");
	    TString hname,  vname;
	    TString table0, table;
	    TH2I* i_h;
	    table0 = "LMF_LASER"; table0 += "_"; table0 += colorStr;

	    //
	    // LMF Run
	    //
	    table = "LMF_RUN_DAT";
	    TTree* lmfrun_t = (TTree*) f->Get(table);
	    map< TString, int    > lmfrun_i;
	    vname = "LOGIC_ID";              lmfrun_t->SetBranchAddress( vname, &lmfrun_i[vname] );
	    vname = "NEVENTS";               lmfrun_t->SetBranchAddress( vname, &lmfrun_i[vname] );
	    vname = "QUALITY_FLAG";          lmfrun_t->SetBranchAddress( vname, &lmfrun_i[vname] );
	    lmfrun_t->LoadTree( 0 );
	    lmfrun_t->GetEntry( 0 );
	    logic_id =  (int)  lmfrun_i["LOGIC_ID"];
	    EcalLogicID ecid_lmfrun = econn->getEcalLogicID( logic_id );
	    // Set the data
	    LMFRunDat lmf_lmfrun;
	    lmf_lmfrun.setNumEvents(      (int) lmfrun_i["NEVENTS"]       );
	    lmf_lmfrun.setQualityFlag(    (int) lmfrun_i["QUALITY_FLAG"] );
	    // Fill the dataset
	    dataset_lmfrun[ecid_lmfrun] = lmf_lmfrun;	    
	    cout << "Inserting " << table << "..." << flush; 
	    econn->insertDataSet( &dataset_lmfrun, &lmfiov );
	    cout << "Done." << endl;

	    //
	    // Laser Configuration
	    //
	    table = "LMF_LASER_CONFIG_DAT";
	    TTree* config_t = (TTree*) f->Get(table);
	    map< TString, int    > config_i;
	    vname = "LOGIC_ID";              config_t->SetBranchAddress( vname, &config_i[vname] );
	    vname = "WAVELENGTH";            config_t->SetBranchAddress( vname, &config_i[vname] );
	    vname = "VFE_GAIN";              config_t->SetBranchAddress( vname, &config_i[vname] );
	    vname = "PN_GAIN";               config_t->SetBranchAddress( vname, &config_i[vname] );
	    vname = "LSR_POWER";             config_t->SetBranchAddress( vname, &config_i[vname] );
	    vname = "LSR_ATTENUATOR";        config_t->SetBranchAddress( vname, &config_i[vname] );
	    vname = "LSR_CURRENT";           config_t->SetBranchAddress( vname, &config_i[vname] );
	    vname = "LSR_DELAY_1";           config_t->SetBranchAddress( vname, &config_i[vname] );
	    vname = "LSR_DELAY_2";           config_t->SetBranchAddress( vname, &config_i[vname] );
	    config_t->LoadTree( 0 );
	    config_t->GetEntry( 0 );
	    logic_id =  (int)  config_i["LOGIC_ID"];
	    EcalLogicID ecid_config = econn->getEcalLogicID( logic_id );
	    // Set the data
	    LMFLaserConfigDat lmf_config;
	    lmf_config.setWavelength( (int) config_i["WAVELENGTH"]     );
	    lmf_config.setVFEGain(    (int) config_i["VFE_GAIN"]       );
	    lmf_config.setPNGain(     (int) config_i["PN_GAIN"]        );
	    lmf_config.setPower(      (int) config_i["LSR_POWER"]      );
	    lmf_config.setAttenuator( (int) config_i["LSR_ATTENUATOR"] );
	    lmf_config.setCurrent(    (int) config_i["LSR_CURRENT"]    );
	    lmf_config.setDelay1(     (int) config_i["LSR_DELAY_1"]    );
	    lmf_config.setDelay2(     (int) config_i["LSR_DELAY_2"]    );
	    // Fill the dataset
	    dataset_config[ecid_config] = lmf_config;	    
 	    cout << "Inserting " << table << "..." << flush; 
 	    econn->insertDataSet( &dataset_config, &lmfiov );
 	    cout << "Done." << endl;

	    //
	    // Laser MATACQ Primitives
	    //
	    table = table0 + "_PULSE_DAT";
	    TTree* pulse_t = (TTree*) f->Get(table);
	    map< TString, int    > pulse_i;
	    map< TString, float > pulse_f;
	    vname = "LOGIC_ID";              pulse_t->SetBranchAddress( vname, &pulse_i[vname] );
	    vname = "FIT_METHOD";            pulse_t->SetBranchAddress( vname, &pulse_i[vname] );
	    vname = "MTQ_AMPL";              pulse_t->SetBranchAddress( vname, &pulse_f[vname] );
	    vname = "MTQ_TIME";              pulse_t->SetBranchAddress( vname, &pulse_f[vname] );
	    vname = "MTQ_RISE";              pulse_t->SetBranchAddress( vname, &pulse_f[vname] );
	    vname = "MTQ_FWHM";              pulse_t->SetBranchAddress( vname, &pulse_f[vname] );
	    vname = "MTQ_FW20";              pulse_t->SetBranchAddress( vname, &pulse_f[vname] );
	    vname = "MTQ_FW80";              pulse_t->SetBranchAddress( vname, &pulse_f[vname] );
	    vname = "MTQ_SLIDING";           pulse_t->SetBranchAddress( vname, &pulse_f[vname] );
	    pulse_t->LoadTree( 0 );
	    pulse_t->GetEntry( 0 );
	    logic_id =  (int)  pulse_i["LOGIC_ID"];
	    EcalLogicID ecid_pulse = econn->getEcalLogicID( logic_id );
	    // Set the data
	    //	    LMFLaserBluePulseDat lmf_pulse;
	    LMFLaserPulseDat::setColor( color ); // set the color
	    LMFLaserPulseDat lmf_pulse;
	    lmf_pulse.setFitMethod( 0 );  // fixme -- is it a string or an int ???
	    lmf_pulse.setAmplitude( (float) pulse_f["MTQ_AMPL"]    );
	    lmf_pulse.setTime(      (float) pulse_f["MTQ_TIME"]    );
	    lmf_pulse.setRise(      (float) pulse_f["MTQ_RISE"]    ); 
	    lmf_pulse.setFWHM(      (float) pulse_f["MTQ_FWHM"]    ); 
	    lmf_pulse.setFW80(      (float) pulse_f["MTQ_FW80"]    );
	    lmf_pulse.setFW20(      (float) pulse_f["MTQ_FW20"]    );
	    lmf_pulse.setSliding(   (float) pulse_f["MTQ_SLIDING"] );
	    // Fill the dataset
	    dataset_pulse[ecid_pulse] = lmf_pulse;	    
	    cout << "Inserting " << table << "..." << flush; 
	    econn->insertDataSet( &dataset_pulse, &lmfiov );
	    cout << "Done." << endl;

	    //
	    // Laser Primitives
	    //
	    table = table0+"_PRIM_DAT";
	    map< TString, TH2* > h_;
	    hname = "LOGIC_ID";          h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "FLAG";              h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "MEAN";              h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "RMS";               h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "PEAK";              h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "APD_OVER_PNA_MEAN"; h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "APD_OVER_PNA_RMS";  h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "APD_OVER_PNA_PEAK"; h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "APD_OVER_PNB_MEAN"; h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "APD_OVER_PNB_RMS";  h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "APD_OVER_PNB_PEAK"; h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "APD_OVER_PN_MEAN";  h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "APD_OVER_PN_RMS";   h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "APD_OVER_PN_PEAK";  h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "ALPHA";             h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "BETA";              h_[hname] = (TH2*) f->Get(table+sep+hname);
	    hname = "SHAPE_COR";         h_[hname] = (TH2*) f->Get(table+sep+hname);
	    i_h = (TH2I*) h_["LOGIC_ID"];
	    TAxis* ax = i_h->GetXaxis();
	    TAxis* ay = i_h->GetYaxis();      
	    int ixbmin = ax->GetFirst();
	    int ixbmax = ax->GetLast();
	    int iybmin = ay->GetFirst();
	    int iybmax = ay->GetLast();
	    LMFLaserPrimDat::setColor( color ); // set the color
	    for( int ixb=ixbmin; ixb<=ixbmax; ixb++ )
	      {
		for( int iyb=iybmin; iyb<=iybmax; iyb++ )
		  {
		    //		    int ix= (int) ax->GetBinLowEdge( ixb );
		    //		    int iy= (int) ay->GetBinLowEdge( iyb );
		    logic_id  = (int) i_h->GetCellContent( ixb, iyb );
		    if( logic_id==0 ) 
		      {
			cout << "WARNING, logic_id==0 -- this should not happen " << endl;
			continue;
		      }
		    if( logic_id<0 ) continue;
		    EcalLogicID ecid_prim = econn->getEcalLogicID( logic_id );
		    // Set the data
		    //		    LMFLaserBluePrimDat bluelaser;	  
		    LMFLaserPrimDat bluelaser;	  
		    bluelaser.setFlag(   (int) h_["FLAG"] -> GetCellContent( ixb, iyb ) );		    
		    bluelaser.setMean( (float) h_["MEAN"] -> GetCellContent( ixb, iyb ) );
		    bluelaser.setRMS(  (float) h_["RMS"]  -> GetCellContent( ixb, iyb ) );
		    bluelaser.setPeak( (float) h_["PEAK"] -> GetCellContent( ixb, iyb ) );		    
		    bluelaser.setAPDOverPNAMean(  (float) h_["APD_OVER_PNA_MEAN"] -> GetCellContent( ixb, iyb ) );
		    bluelaser.setAPDOverPNARMS(   (float) h_["APD_OVER_PNA_RMS"]  -> GetCellContent( ixb, iyb ) );
		    bluelaser.setAPDOverPNAPeak(  (float) h_["APD_OVER_PNA_PEAK"] -> GetCellContent( ixb, iyb ) );		    
		    bluelaser.setAPDOverPNBMean(  (float) h_["APD_OVER_PNB_MEAN"] -> GetCellContent( ixb, iyb ) );
		    bluelaser.setAPDOverPNBRMS(   (float) h_["APD_OVER_PNB_RMS"]  -> GetCellContent( ixb, iyb ) );
		    bluelaser.setAPDOverPNBPeak(  (float) h_["APD_OVER_PNB_PEAK"] -> GetCellContent( ixb, iyb ) );	  
		    bluelaser.setAPDOverPNMean(   (float) h_["APD_OVER_PN_MEAN"]  -> GetCellContent( ixb, iyb ) );
		    bluelaser.setAPDOverPNRMS(    (float) h_["APD_OVER_PN_RMS"]   -> GetCellContent( ixb, iyb ) );
		    bluelaser.setAPDOverPNPeak(   (float) h_["APD_OVER_PN_PEAK"]  -> GetCellContent( ixb, iyb ) );	 
		    bluelaser.setAlpha(           (float) h_["ALPHA"] -> GetCellContent( ixb, iyb ) ); 
		    bluelaser.setBeta(            (float) h_["BETA"]  -> GetCellContent( ixb, iyb )); 
		    bluelaser.setShapeCor(        (float) h_["SHAPE_COR"] -> GetCellContent( ixb, iyb ) );
		    // Fill the dataset
		    dataset_prim[ecid_prim] = bluelaser;
		  }
	      } 

	    //
	    // Laser PN Primitives
	    //
	    table = table0 + "_PN_PRIM_DAT";
	    TTree* pn_t = (TTree*) f->Get(table);
	    map< TString, int    > pn_i;
	    map< TString, float > pn_f;
	    vname = "LOGIC_ID";              pn_t->SetBranchAddress( vname, &pn_i[vname] );
	    vname = "FLAG";                  pn_t->SetBranchAddress( vname, &pn_i[vname] );
	    vname = "MEAN";                  pn_t->SetBranchAddress( vname, &pn_f[vname] );
	    vname = "RMS";                   pn_t->SetBranchAddress( vname, &pn_f[vname] );
	    vname = "PEAK";                  pn_t->SetBranchAddress( vname, &pn_f[vname] );
	    vname = "PNA_OVER_PNB_MEAN";     pn_t->SetBranchAddress( vname, &pn_f[vname] );
	    vname = "PNA_OVER_PNB_RMS";      pn_t->SetBranchAddress( vname, &pn_f[vname] );
	    vname = "PNA_OVER_PNB_PEAK";     pn_t->SetBranchAddress( vname, &pn_f[vname] );
	    Long64_t pn_n = pn_t->GetEntries();
	    LMFLaserPNPrimDat::setColor( color ); // set the color
	    for( Long64_t jj=0; jj<pn_n; jj++ )
	      {
		pn_t->LoadTree( jj );
		pn_t->GetEntry( jj );
		logic_id =  (int)  pn_i["LOGIC_ID"];
		EcalLogicID ecid_pn = econn->getEcalLogicID( logic_id );	  
		// Set the data
		//		LMFLaserBluePNPrimDat bluepn;
		LMFLaserPNPrimDat bluepn;
		bluepn.setFlag(            (int)  pn_i["FLAG"]  );
		bluepn.setMean(           (float) pn_f["MEAN"]  );
		bluepn.setRMS(            (float) pn_f["RMS"]   );
		bluepn.setPeak(           (float) pn_f["PEAK"]  );
		bluepn.setPNAOverPNBMean( (float) pn_f["PNA_OVER_PNB_MEAN"] );
		bluepn.setPNAOverPNBRMS(  (float) pn_f["PNA_OVER_PNB_RMS "] );
		bluepn.setPNAOverPNBPeak( (float) pn_f["PNA_OVER_PNB_PEAK"] );
		// Fill the dataset
		dataset_pnprim[ecid_pn] = bluepn;
	      }

	    // close the root file
	    f->Close();

	    // Inserting the dataset, identified by iov
	    cout << "Inserting " << table0 << "_PRIM_DAT and _PN_PRIM_DAT ..." << flush; 
	    econn->insertDataSet( &dataset_prim,   &lmfiov );
	    econn->insertDataSet( &dataset_pnprim, &lmfiov );
	    cout << "Done." << endl;

	  }
      } 
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
  host = "pccmsecdb"; sid= "ecalh4db"; user="cond04"; pass="oracond04";

  try {

    CondDBApp app(host, sid, user, pass);
    // unsigned int run = 17452;
    // unsigned int ts  = 1162879473;
    TString pathStr   = "/afs/cern.ch/user/g/ghm/scratch0/MusEcal/EBSM06/prim/";
    TString regionStr = "EB-SM19";

    TString runlistfile(pathStr); runlistfile += "runlist_Blue_Laser";

    cout << "read " << runlistfile << endl;
    ifstream fin;
    fin.open(runlistfile);

    int ii = 0;
    int nrunmax = 100000;
    while( fin.peek() != EOF )
      {
	int run_, ts_;
	int dt;
	fin >> run_ >> ts_ >> dt;
	cout << "Run=" << run_ << " TS=" << ts_ << endl;
	app.testGhm( pathStr, regionStr, run_, ts_ );
	ii++;
	if( ii>nrunmax ) break;
      }
  } catch (exception &e) {
    cout << "ERROR:  " << e.what() << endl;
  } catch (...) {
    cout << "Unknown error caught" << endl;
  }

  cout << "All Done." << endl;

  return 0;
}
