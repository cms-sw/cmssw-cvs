#include <memory>
#include <string>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Validation/RecoJets/interface/RootSystem.h"
#include "Validation/RecoJets/interface/RootHistograms.h"
#include "Validation/RecoJets/interface/RootPostScript.h"
#include "Validation/RecoJets/interface/ManipHist.h"
#include "Validation/RecoJets/bin/NiceStyle.cc"

using namespace std;

class TopInspectSum : public ManipHist {
 public:
  TopInspectSum(){};
  ~TopInspectSum(){};
  virtual void readConfig( std::string );
  void writeOutput(){CompHist::writeOutput(kLastOnly);};
};

void TopInspectSum::readConfig( std::string name )
{
  ConfigFile cfg( name, "=", "$" );  
  configBlockIO  ( cfg );
  configBlockHist( cfg );
  configBlockSum ( cfg );
}

int main(int argc, char* argv[])
{
  setNiceStyle();
  gStyle->SetOptStat( 0 );

  if( argc<2 ){
    cerr << "ERROR:" 
	 << " Missing argument" << endl;
    return 1;
  }

  TopInspectSum plots;
  try{
    plots.readConfig( argv[1] );
    plots.loadHistograms();

    //histogram manipulations
    plots.sumHistograms();

    //depending on style draw ps/eps/jpg
    if( !strcmp(plots.writeAs().c_str(), "ps") ){
      plots.drawPs();
    } else if( !strcmp(plots.writeAs().c_str(), "eps") ){
      plots.drawEps();
    } else{
      cerr << "ERROR:"
	   << " Unknown file format requested: "
	   << plots.writeAs() << endl; 
      return -1;
    }
    plots.writeOutput();
  }
  catch(char* str){
    cerr << "ERROR: " << str << endl;
    return 1;
  }
  catch(...){
    cerr << "ERROR: this one is new...";
      return 1;
  }
  cout << "Thanx and GoodBye " << endl;
  return 0;
}
