#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "TStopwatch.h"



class WSelector : public EventSelector {
public:
  WSelector( edm::ParameterSet const & params ) :
    muonSrc_(params.getParameter<edm::InputTag>("muonSrc")),
    metSrc_ (params.getParameter<edm::InputTag>("metSrc")) 
  {
    double muonPtMin = params.getParameter<double>("muonPtMin");
    double metMin = params.getParameter<double>("metMin");
    push_back("Muon Pt", muonPtMin );
    push_back("MET", metMin );
    set("Muon Pt");
    set("MET");

    muonPtIndex_ = index_type(&bits_, std::string("Muon Pt") );
    metIndex_    = index_type(&bits_, std::string("MET") );

    wMuon_ = 0;
    met_ = 0;
  }
  virtual ~WSelector() {}


  // Here is where the selection occurs
  virtual bool operator()( edm::EventBase const & event, pat::strbitset & ret){

    ret.set(false);

    // Handle to the muon collection
    edm::Handle<std::vector<pat::Muon> > muons;    
    // Handle to the MET collection
    edm::Handle<std::vector<pat::MET> > met;
    
    // Get the objects from the event
    bool gotMuons = event.getByLabel(muonSrc_, muons);
    bool gotMET = event.getByLabel(metSrc_, met);

    // get the MET, require to be > minimum
    if ( gotMET ) {
      met_ = &met->at(0);
      if ( met_->pt() > cut(metIndex_,   double()) || ignoreCut(metIndex_) ) 
	passCut(ret, metIndex_);
    }

    // get the highest pt muon, require to have pt > minimum
    if ( gotMuons ) {
      if ( !ignoreCut(muonPtIndex_) )  {
	if ( muons->size() > 0 ) {
	  wMuon_ = &muons->at(0);
	  if ( wMuon_->pt() > cut(muonPtIndex_, double()) || ignoreCut(muonPtIndex_) ) 
	    passCut(ret, muonPtIndex_);
	}
      } else {
	passCut( ret, muonPtIndex_);
      }
    }

    setIgnored(ret);
    return (bool)ret;
    
  }

  pat::Muon const & wMuon() const { return *wMuon_;}
  pat::MET  const & met()   const { return *met_;}

protected:
  edm::InputTag muonSrc_;
  edm::InputTag metSrc_;

  index_type muonPtIndex_;
  index_type metIndex_;

  pat::Muon const * wMuon_;
  pat::MET const *  met_;
};

int main(int argc, char* argv[]) 
{
  // ----------------------------------------------------------------------
  // First Part: 
  //
  //  * enable the AutoLibraryLoader 
  //  * book the histograms of interest 
  //  * open the input file
  // ----------------------------------------------------------------------

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  // Get the python configuration
  PythonProcessDesc builder(argv[1]);
  edm::ParameterSet const& fwliteParameters = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("FWLiteParams");
  
  // Now get the W selector
  edm::ParameterSet wSelectorParams = fwliteParameters.getParameter<edm::ParameterSet>("wSelectorParams");
  WSelector wSelector( wSelectorParams );
  pat::strbitset wSelectorReturns = wSelector.getBitTemplate();
  

  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService("analyzePatBasics.root");
  TFileDirectory theDir = fs.mkdir("analyzeBasicPat");
  TH1F* muonPt_  = theDir.make<TH1F>("muonPt", "pt",    100,  0.,300.);
  TH1F* muonEta_ = theDir.make<TH1F>("muonEta","eta",   100, -3.,  3.);
  TH1F* muonPhi_ = theDir.make<TH1F>("muonPhi","phi",   100, -5.,  5.);  
  
  // open input file (can be located on castor)
  TFile* inFile = TFile::Open( "patTuple.root" );

  // ----------------------------------------------------------------------
  // Second Part: 
  //
  //  * loop the events in the input file 
  //  * receive the collections of interest via fwlite::Handle
  //  * fill the histograms
  //  * after the loop close the input file
  // ----------------------------------------------------------------------

  // loop the events
  unsigned int iEvent=0;
  fwlite::Event ev(inFile);
  TStopwatch timer;
  timer.Start();

  unsigned int nEventsAnalyzed = 0;

  for(ev.toBegin(); !ev.atEnd(); ++ev, ++iEvent){
    edm::EventBase const & event = ev;

    
    if ( wSelector(event, wSelectorReturns ) ) {
      

      pat::Muon const & wMuon = wSelector.wMuon();

      muonPt_ ->Fill( wMuon.pt()  );
      muonEta_->Fill( wMuon.eta() );
      muonPhi_->Fill( wMuon.phi() );
      
    } 
    ++nEventsAnalyzed;

  }  
  timer.Stop();

  // close input file
  inFile->Close();

  // print selector
  wSelector.print(std::cout);

  // print some timing statistics
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  printf("Analyzed events: %d \n",nEventsAnalyzed);
  printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);
  printf("%4.2f events / RealTime second .\n", (double)nEventsAnalyzed/rtime);
  printf("%4.2f events / CpuTime second .\n", (double)nEventsAnalyzed/ctime);
  




  // ----------------------------------------------------------------------
  // Third Part: 
  //
  //  * never forget to free the memory of objects you created
  // ----------------------------------------------------------------------

  // in this example there is nothing to do 
  
  // that's it!
  return 0;
}
