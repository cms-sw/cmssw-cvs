#include <memory>
#include <string>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Validation/RecoJets/interface/RootSystem.h"
#include "Validation/RecoJets/interface/RootHistograms.h"
#include "Validation/RecoJets/interface/RootPostScript.h"
#include "Validation/RecoJets/interface/CompHist.h"
#include "Validation/RecoJets/bin/NiceStyle.cc"

using namespace std;

class TopInspect : public CompHist {
 public:
  TopInspect(){};
  ~TopInspect(){};
  virtual void readConfig( std::string );
};

void TopInspect::readConfig( std::string name )
{
  ConfigFile cfg( name, "=", "$" );  
  configBlockIO( cfg );
  configBlockHist( cfg );
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

  TopInspect plots;
  try{
    plots.readConfig( argv[1] );
    plots.loadHistograms();
    
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
