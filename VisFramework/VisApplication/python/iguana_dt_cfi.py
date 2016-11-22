import FWCore.ParameterSet.Config as cms

VisConfigurationService = cms.Service("VisConfigurationService",
    ContentProxies = cms.untracked.vstring('Reco/MuonDT', 'Reco/Detector', 'Simulation/Hits')
)


