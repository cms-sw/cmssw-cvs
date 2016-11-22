#ifndef HLTMuonL1DQMSource_H
#define HLTMuonL1DQMSource_H

/** \class HLTMuonL1DQMSource
 * *
 *  DQM Test Client
 *
 *  $Date: 2009/10/15 11:31:28 $
 *  $Revision: 1.2 $
 *  \author  M. Vander Donckt CERN
 *   
 */
#include <memory>
#include <unistd.h>

#include <FWCore/Framework/interface/EDAnalyzer.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//
// class declaration
//

class HLTMuonL1DQMSource : public edm::EDAnalyzer {
public:
  HLTMuonL1DQMSource( const edm::ParameterSet& );
  ~HLTMuonL1DQMSource();

protected:
   
  /// BeginJob
  void beginJob();

  /// BeginRun
  void beginRun(const edm::Run& r, const edm::EventSetup& c);

  /// Fake Analyze
  void analyze(const edm::Event& e, const edm::EventSetup& c) ;

  void beginLuminosityBlock(const edm::LuminosityBlock& lumiSeg, 
                            const edm::EventSetup& context) ;

  /// DQM Client Diagnostic
  void endLuminosityBlock(const edm::LuminosityBlock& lumiSeg, 
                          const edm::EventSetup& c);

  /// EndRun
  void endRun(const edm::Run& r, const edm::EventSetup& c);

  /// Endjob
  void endJob();

private:
 
  edm::ParameterSet parameters_;

  DQMStore* dbe_;  
  std::string monitorName_;
  std::string outputFile_;
  int level_;      ///counter
  int counterEvt_;      ///counter
  int prescaleEvt_;     ///every n events

  edm::InputTag l1muCollectionTag_;

  // ----------member data ---------------------------
  bool verbose_;

  MonitorElement * hNMu;
  MonitorElement * hcharge;
  MonitorElement * hpt;
  MonitorElement * heta;
  MonitorElement * hphi;
  MonitorElement * hptphi;
  MonitorElement * hpteta;
  MonitorElement * hetaphi;
  MonitorElement * hl1quality;

};

#endif

