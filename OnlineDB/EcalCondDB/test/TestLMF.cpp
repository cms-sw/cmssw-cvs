#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <cstdlib>
#include "OnlineDB/EcalCondDB/interface/EcalCondDBInterface.h"
#include "OnlineDB/EcalCondDB/interface/all_lmf_types.h"


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
      econn = new EcalCondDBInterface( sid, user, pass );
      cout << "Done." << endl;
    } catch (runtime_error &e) {
      cerr << e.what() << endl;
      exit(-1);
    }

    locations[0] = "H4";
    locations[1] = "867-1";
    locations[2] = "867-2";
    locations[3] = "867-3";
  }



  /**
   *  App destructor;  Cleans up database connection
   */
  ~CondDBApp() 
  {
    delete econn;
  }


  RunIOV makeRunIOV()
  {
    // The objects necessary to identify a dataset
    LocationDef locdef;
    RunTypeDef rundef;
    RunTag runtag;

    locdef.setLocation(locations[3]);

    rundef.setRunType("TEST");
    
    runtag.setLocationDef(locdef);
    runtag.setRunTypeDef(rundef);

    // Our beginning time will be the current GMT time
    // This is the time zone that should be written to the DB!
    // (Actually UTC)
    Tm startTm;
    startTm.setToCurrentGMTime();

    // Our beginning run number will be the seconds representation
    // of that time, and we will increment our IoV based on 
    // a microsecond representation
    uint64_t microseconds = startTm.microsTime();
    startmicros = microseconds;
    run_t run = (int)(microseconds/1000000);
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
    lmfiov.setSubRunNumber(0);
    lmfiov.setSubRunStart(runiov->getRunStart());

    return lmfiov;
  }



  /**
   *  Write LMFLaserBlueRawDat objects with associated RunTag and RunIOVs
   *  IOVs are written using automatic updating of the 'RunEnd', as if
   *  the time of the end of the run was not known at the time of writing.
   */
  void testWrite()
  {
    cout << "Writing LMFLaserBlueRawDat to database..." << endl;
    RunIOV runiov = this->makeRunIOV();
    RunTag runtag = runiov.getRunTag();
    run_t run = runiov.getRunNumber();

    // write to the DB
    cout << "Inserting run..." << flush;
    econn->insertRunIOV(&runiov);
    cout << "Done." << endl;
    printIOV(&runiov);

    // fetch it back
    cout << "Fetching run..." << flush;
    RunIOV runiov_prime = econn->fetchRunIOV(&runtag, run);
    cout << "Done." << endl;
    printIOV(&runiov_prime);

    // LMF Tag and IOV
    LMFRunIOV lmfiov = this->makeLMFRunIOV(&runiov);

    // Get channel ID for SM 10, crystal c      
    int c = 1;
    EcalLogicID ecid;
    ecid = econn->getEcalLogicID("EB_crystal_number", 10, c);

    // Set the data
    LMFLaserBlueRawDat bluelaser;
    map<EcalLogicID, LMFLaserBlueRawDat> dataset;

    int i = 1;
    float val = 0.11111 + i;
    bluelaser.setAPDPeak(val);
    bluelaser.setAPDErr(val);
    
    // Fill the dataset
    dataset[ecid] = bluelaser;
    
    // Insert the dataset, identifying by iov
    cout << "Inserting dataset..." << flush;
    econn->insertDataSet(&dataset, &lmfiov);
    cout << "Done." << endl;

    // Fetch it back
    cout << "Fetching dataset..." << flush;
    dataset.clear();
    econn->fetchDataSet(&dataset, &lmfiov);
    cout << "retrieved " << dataset.size() << " channel-value pairs" << endl;
    printDataSet(&dataset);

    // Fetch back LMFRunIOV just written
    LMFRunTag lmftag = lmfiov.getLMFRunTag();
    subrun_t subrun = lmfiov.getSubRunNumber();
    cout << "Fetching subrun..." << flush;
    LMFRunIOV lmfruniov_prime = econn->fetchLMFRunIOV(&runtag, &lmftag, run, subrun);
    cout << "Done." << endl << endl << endl;
  };



  /**
   *  Write a data set
   */
  template<class DATT, class IOVT>
  void testTable(DATT* data, IOVT* iov, EcalLogicID* ecid)
  {
    tablesTried++;
    try {
      cout << "Table " << tablesTried << "..." << flush;
      map<EcalLogicID, DATT> dataset;
      dataset[*ecid] = *data;
      econn->insertDataSet(&dataset, iov);
      cout << "write OK..." << flush;
      dataset.clear();
      econn->fetchDataSet(&dataset, iov);
      if (!dataset.size()) {
	throw(runtime_error("Zero rows read back"));
      }
      cout << "read OK" << endl;
      tablesOK++;
    } catch (runtime_error &e) {
      cout << "testTable FAILED:  " << e.what() << endl;
    } catch (...) {
      cout << "testTable FAILED:  unknown exception" << endl;
    }
  }



  /**
   *  Write to each of the data tables
   */
  void testAllTables()
  {
    cout << "Testing writing to all tables..." << endl;
    tablesTried = 0;
    tablesOK = 0;
    
    // get a dummy logic ID
    EcalLogicID logicID = econn->getEcalLogicID(-1);

    // Insert a RunIOV
    RunIOV runiov = this->makeRunIOV();
    econn->insertRunIOV(&runiov);

    // LMFRunIOV tables
    LMFRunIOV lmfiov = this->makeLMFRunIOV(&runiov);

    // tables using LMFRunIOV
    LMFLaserBlueRawDat table01;
    testTable(&table01, &lmfiov, &logicID);

    LMFLaserIRedRawDat table02;
    testTable(&table02, &lmfiov, &logicID);

    LMFLaserBlueNormDat table03;
    testTable(&table03, &lmfiov, &logicID);

    LMFLaserIRedNormDat table04;
    testTable(&table04, &lmfiov, &logicID);

    LMFLaserBlueCoeffDat table05;
    testTable(&table05, &lmfiov, &logicID);

    LMFLaserIRedCoeffDat table06;
    testTable(&table06, &lmfiov, &logicID);

    LMFLaserBlueShapeDat table07;
    testTable(&table07, &lmfiov, &logicID);

    LMFLaserIRedShapeDat table08;
    testTable(&table08, &lmfiov, &logicID);

    LMFPNBlueDat table09;
    testTable(&table09, &lmfiov, &logicID);

    LMFPNIRedDat table10;
    testTable(&table10, &lmfiov, &logicID);

    LMFPNTestPulseDat table11;
    testTable(&table11, &lmfiov, &logicID);

    LMFPNConfigDat table12;
    testTable(&table12, &lmfiov, &logicID);


    cout << "Test of writing to all tables complete" << endl;
    cout << tablesOK << " of " << tablesTried << " written to successfully" << endl << endl << endl;
  };



private:
  CondDBApp();  // hidden default constructor
  EcalCondDBInterface* econn;
  string locations[4];
  uint64_t startmicros;
  uint64_t endmicros;
  run_t startrun;
  run_t endrun;

  int tablesTried;
  int tablesOK;

  /**
   *   Iterate through the dataset and print some data
   */
  void printDataSet( const map<EcalLogicID, LMFLaserBlueRawDat>* dataset, int limit = 0 ) const
  {
    cout << "==========printDataSet()" << endl;
    if (dataset->size() == 0) {
      cout << "No data in map!" << endl;
    }
    EcalLogicID ecid;
    LMFLaserBlueRawDat bluelaser;

    int count = 0;
    typedef map< EcalLogicID, LMFLaserBlueRawDat >::const_iterator CI;
    for (CI p = dataset->begin(); p != dataset->end(); p++) {
      count++;
      if (limit && count > limit) { return; }
      ecid = p->first;
      bluelaser  = p->second;
     
      cout << "SM:                     " << ecid.getID1() << endl;
      cout << "Xtal:                   " << ecid.getID2() << endl;
      cout << "APD Peak:               " << bluelaser.getAPDPeak() << endl;
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

  if (argc != 5) {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " <host> <SID> <user> <pass>" << endl;
    exit(-1);
  }

  host = argv[1];
  sid = argv[2];
  user = argv[3];
  pass = argv[4];

  try {
    CondDBApp app(host, sid, user, pass);

    app.testWrite();
    app.testAllTables();
  } catch (exception &e) {
    cout << "ERROR:  " << e.what() << endl;
  } catch (...) {
    cout << "Unknown error caught" << endl;
  }

  cout << "All Done." << endl;

  return 0;
}
