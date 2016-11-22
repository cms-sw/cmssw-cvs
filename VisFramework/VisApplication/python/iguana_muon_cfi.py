import FWCore.ParameterSet.Config as cms

VisConfigurationService = cms.Service("VisConfigurationService",
    ContentProxies = cms.untracked.vstring('Reco/MuonDT', 'Reco/MuonCSC', 'Reco/MuonRPC', 'Reco/Detector', 'Simulation/Hits')
)


