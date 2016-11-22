#ifndef VIS_MUON_VIS_MUON_FILTER_H
#define VIS_MUON_VIS_MUON_FILTER_H

# include "FWCore/Framework/interface/EDFilter.h"

class VisMuonFilter: public edm::EDFilter {

public:
  // Constructor
  explicit VisMuonFilter (const edm::ParameterSet &pset);

  /// Destructor
  virtual ~VisMuonFilter (void);

  // Operations
  virtual bool filter (edm::Event &event, const edm::EventSetup &eventSetup);

private:

  // Labels
  std::string m_muonLabel;

  // Counters
  int m_numberOfEvents;
  int m_numberMuonTracks;
};

#endif // VIS_MUON_VIS_MUON_FILTER_H
