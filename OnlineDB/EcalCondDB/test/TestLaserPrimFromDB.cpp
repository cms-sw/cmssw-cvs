#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <cassert>
#include <cstdlib>

#include "OnlineDB/EcalCondDB/interface/EcalCondDBInterface.h"
#include "OnlineDB/EcalCondDB/interface/all_lmf_types.h"
#include "OnlineDB/EcalCondDB/interface/RunDat.h"
#include "OnlineDB/EcalCondDB/interface/RunList.h"
#include "OnlineDB/EcalCondDB/interface/RunPTMTempDat.h"

#include "OnlineDB/EcalCondDB/interface/RunLaserRunDat.h"
#include "OnlineDB/EcalCondDB/interface/LMFLaserPrimDat.h"
#include "OnlineDB/EcalCondDB/interface/LMFLaserPNPrimDat.h"
#include "OnlineDB/EcalCondDB/interface/LMFLaserPulseDat.h"

#include <TROOT.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TH2F.h>
#include <TF1.h>

#include "MELaserPrim.cc"


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
  
  void testGhm( const string& location, const string& runType, const string& generalTag,
		int min_run,  int max_run ) 
  {
    cout << "Entering testGhm " << endl;
    
    LocationDef my_locdef;
    my_locdef.setLocation( location );

    RunTypeDef my_rundef;    
    my_rundef.setRunType( runType );

    RunTag  my_runtag;
    my_runtag.setLocationDef( my_locdef );
    my_runtag.setRunTypeDef(  my_rundef );
    my_runtag.setGeneralTag( generalTag );

    RunList my_runlist = econn->fetchRunList( my_runtag );

    std::vector< RunIOV > run_vec =  my_runlist.getRuns();

    int nruns=run_vec.size();
    if( nruns==0 ) 
      {      
	cout << "No run found -- exit" << endl;
	return;
      }
    cout << "Number of runs = " << nruns << endl;

    LMFRunTag lmf_tag;
    lmf_tag.setGeneralTag("default");

    LMFRunList lmf_list;
    lmf_list.setLMFRunTag( lmf_tag );
    lmf_list.setRunTag(my_runtag);
    //    lmf_list=econn->fetchLMFRunList(my_runtag, lmf_tag);
    lmf_list=econn->fetchLMFRunList( my_runtag, lmf_tag, min_run, max_run );
    // lmf_list=econn->fetchLMFRunList(my_runtag, lmf_tag);
    
    std::vector< LMFRunIOV > lmf_run_vec =  lmf_list.getRuns();

    int nlmfruns = lmf_run_vec.size();
    if( nlmfruns==0 ) 
      {      
	cout << "No LMF run found -- exit" << endl;
	return;
      }
    cout << "Number of LMF runs = " << nlmfruns << endl;

    //    int ii=0;
    for( int ii = 0; ii<nlmfruns; ii++ )
      {
 
	Tm startTm3;
	startTm3.setToCurrentGMTime();
	cout << "\nQuery starting at:    " << startTm3.str() << endl;

	LMFRunIOV lmfruniov  = lmf_run_vec[ii];
	RunIOV runiov        = lmfruniov.getRunIOV();
	int run = runiov.getRunNumber();

	cout << "---> run[" << ii << "]=" << run << endl;

	map< EcalLogicID, RunLaserRunDat > dataset_runconfig;
   	econn->fetchDataSet( &dataset_runconfig, &runiov );
	assert( dataset_runconfig.size()==1 );
	{
	  EcalLogicID ecalid  = dataset_runconfig.begin()->first;
	  //	  int id = ecalid.getLogicID();

	  RunLaserRunDat dat = dataset_runconfig.begin()->second;
	  cout << "---> for id1/id2/id3=" << ecalid.getID1() << "/" << ecalid.getID2() << "/" << ecalid.getID3() << " type=" << dat.getLaserSequenceType() << " cond=" << dat.getLaserSequenceCond() << endl; 
	}
	
	int ts     = lmfruniov.getSubRunStart().microsTime()/1000000;
	int subrun = lmfruniov.getSubRunNumber();
	int color  =  subrun/10; // fixme
	int side   =  subrun%10; // fixme

	LMFLaserPrimDat::setColor( color );   // set the color
	LMFLaserPNPrimDat::setColor( color ); // set the color
	LMFLaserPulseDat::setColor( color );  // set the color

	map< EcalLogicID, LMFRunDat > dataset_rundat;
	econn->fetchDataSet( &dataset_rundat, &lmfruniov );
	assert( dataset_rundat.size()==1 );
    
	EcalLogicID id_rundat = dataset_rundat.begin()->first;
	LMFRunDat rundat = dataset_rundat.begin()->second;
	int nevents     = rundat.getNumEvents();
	int qualityflag = rundat.getQualityFlag();
	int sm  = id_rundat.getID1(); // fixme !!!
	int dcc = 609+sm; // fixme !!!

	cout << "--->  id1/id2/id3 " << id_rundat.getID1() << "/" << id_rundat.getID2() << "/" << id_rundat.getID3() 
	     << "\tNevents/Qualityflag " << nevents << "/" << qualityflag
	     << " -- subrun SM=" << sm << " -- side=" << side << " -- color=" << color 
	     << endl;  

	// output path
	TString outpath = "/tmp/ghm/MusEcal/primFromDB/"; // fixme, only valid on pcsaccms03
	
	MELaserPrim prim( dcc, side, color, run, ts, "./", outpath );
	prim.bookHistograms();
	prim.print( cout );

	TString tname;

	//
	// retrieve the laser configuration 
	//
	map< EcalLogicID, LMFLaserConfigDat > dataset_config;
	econn->fetchDataSet( &dataset_config, &lmfruniov );
	assert( dataset_config.size()==1 );

	tname = MELaserPrim::lmfLaserName( MELaserPrim::iLmfLaserConfig );
	{  
	  EcalLogicID ecalid  = dataset_config.begin()->first;
	  int id = ecalid.getLogicID();
      
	  LMFLaserConfigDat dat = dataset_config.begin()->second;
      
	  int wavelength = dat.getWavelength();
	  assert( wavelength==color ); // fixme

	  prim.setInt( tname, "LOGIC_ID",        id );
	  prim.setInt( tname, "WAVELENGTH",      wavelength );
	  prim.setInt( tname, "VFE_GAIN",        dat.getVFEGain() );
	  prim.setInt( tname, "PN_GAIN",         dat.getPNGain() );
	  prim.setInt( tname, "LSR_POWER",       (int) dat.getPower() );      // fixme ! should be integers...
	  prim.setInt( tname, "LSR_ATTENUATOR",  (int) dat.getAttenuator() );
	  prim.setInt( tname, "LSR_CURRENT",     (int) dat.getCurrent() );
	  prim.setInt( tname, "LSR_DELAY_1",     (int) dat.getDelay1() );
	  prim.setInt( tname, "LSR_DELAY_2",     (int) dat.getDelay2() );
       
	  prim.fill( tname );
	}

	//
	// retrieve the laser pulse primitives 
	//
	map< EcalLogicID, LMFLaserPulseDat > dataset_pulse;
	econn->fetchDataSet( &dataset_pulse, &lmfruniov );
	assert( dataset_pulse.size()==1 );

	tname = MELaserPrim::lmfLaserName( MELaserPrim::iLmfLaserPulse, color );
	{  
	  EcalLogicID ecalid  = dataset_pulse.begin()->first;
	  int id = ecalid.getLogicID();
      
	  LMFLaserPulseDat dat = dataset_pulse.begin()->second;
      
	  prim.setInt( tname, "LOGIC_ID",     id );
	  prim.setInt( tname, "FIT_METHOD",   dat.getFitMethod() );
	  prim.setVal( tname, "MTQ_AMPL",     dat.getAmplitude() );
	  prim.setVal( tname, "MTQ_TIME",     dat.getTime()      );
	  prim.setVal( tname, "MTQ_RISE",     dat.getRise()      );
	  prim.setVal( tname, "MTQ_FWHM",     dat.getFWHM()      );
	  prim.setVal( tname, "MTQ_FW20",     dat.getFW20()      );
	  prim.setVal( tname, "MTQ_FW80",     dat.getFW80()      );
	  prim.setVal( tname, "MTQ_SLIDING",  dat.getSliding()   );
      
	  prim.fill( tname );
	}

	//
	// retrieve laser APD/VPT primitives
	//
	map< EcalLogicID, LMFLaserPrimDat > dataset_prim;
	econn->fetchDataSet( &dataset_prim, &lmfruniov );

	map< EcalLogicID, LMFLaserPrimDat >::const_iterator it;
	for( it=dataset_prim.begin(); it!=dataset_prim.end(); it++ )
	  {

	    EcalLogicID ecalid  = it->first;
	    int id  = ecalid.getLogicID();
	    int id1 = ecalid.getID1();
	    assert( id1==sm );
	    int id2 = ecalid.getID2();
	    int ix  = (id2-1)/20;
	    int iy  = 19-(id2-1)%20;
	  
	    LMFLaserPrimDat dat = it->second;

	    prim.setInt( "LOGIC_ID",           ix, iy, id );
	    prim.setInt( "FLAG",               ix, iy, dat.getFlag() );
	    prim.setVal( "MEAN",               ix, iy, dat.getMean() );
	    prim.setVal( "RMS",                ix, iy, dat.getRMS()  );
	    prim.setVal( "PEAK",               ix, iy, dat.getPeak() );
	    prim.setVal( "APD_OVER_PNA_MEAN",  ix, iy, dat.getAPDOverPNAMean()  );
	    prim.setVal( "APD_OVER_PNA_RMS",   ix, iy, dat.getAPDOverPNARMS()   );
	    prim.setVal( "APD_OVER_PNA_PEAK",  ix, iy, dat.getAPDOverPNAPeak()  );
	    prim.setVal( "APD_OVER_PNB_MEAN",  ix, iy, dat.getAPDOverPNBMean()  );
	    prim.setVal( "APD_OVER_PNB_RMS",   ix, iy, dat.getAPDOverPNBRMS()   );
	    prim.setVal( "APD_OVER_PNB_PEAK",  ix, iy, dat.getAPDOverPNBPeak()  );
	    prim.setVal( "APD_OVER_PN_MEAN",   ix, iy, dat.getAPDOverPNMean()   );
	    prim.setVal( "APD_OVER_PN_RMS",    ix, iy, dat.getAPDOverPNRMS()    );
	    prim.setVal( "APD_OVER_PN_PEAK",   ix, iy, dat.getAPDOverPNPeak()   );
	    prim.setVal( "ALPHA",              ix, iy, dat.getAlpha()           );
	    prim.setVal( "BETA",               ix, iy, dat.getBeta()            );
	    prim.setVal( "SHAPE_COR",          ix, iy, dat.getShapeCor()        );

	  }

	//
	// retrieve PN primitives
	//
	map< EcalLogicID, LMFLaserPNPrimDat > dataset_pnprim;
	econn->fetchDataSet( &dataset_pnprim, &lmfruniov );

	tname = MELaserPrim::lmfLaserName( MELaserPrim::iLmfLaserPnPrim, color );

	map< EcalLogicID, LMFLaserPNPrimDat >::const_iterator it_pn;
	for( it_pn=dataset_pnprim.begin(); it_pn!=dataset_pnprim.end(); it_pn++ )
	  {
	    EcalLogicID ecalid  = it_pn->first;
	    int id = ecalid.getLogicID();

	    LMFLaserPNPrimDat dat = it_pn->second;

	    prim.setInt( tname, "LOGIC_ID", id );
	    prim.setVal( tname, "MEAN", dat.getMean() );
	    prim.setVal( tname, "RMS",  dat.getRMS()  );
	    prim.setVal( tname, "PEAK", dat.getPeak() );
	    prim.setVal( tname, "PNA_OVER_PNB_MEAN",  dat.getPNAOverPNBMean()  );
	    prim.setVal( tname, "PNA_OVER_PNB_RMS",   dat.getPNAOverPNBRMS()   );
	    prim.setVal( tname, "PNA_OVER_PNB_PEAK",  dat.getPNAOverPNBPeak()  );

	    prim.fill( tname );
	  }
    
	prim.writeHistograms();
      }
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

int main( int argc, char* argv[] )
{
  string host;
  string sid;
  string user;
  string pass;
  host = "pccmsecdb"; sid = "ecalh4db"; user = "cond04"; pass = "oracond04";

  string location;
  string runType;
  string generalTag;
  location = "H4"; runType = "TEST"; generalTag = "default";

  int min_run=17100;
  int max_run=17200;

  try {

    CondDBApp app( host, sid, user, pass );

    app.testGhm( location, runType, generalTag, min_run, max_run );

  } catch (exception &e) {
    cout << "ERROR:  " << e.what() << endl;
  } catch (...) {
    cout << "Unknown error caught" << endl;
  }

  cout << "All Done." << endl;

  return 0;
}
