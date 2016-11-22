import FWCore.ParameterSet.Config as cms

VisConfigurationService = cms.Service("VisConfigurationService",
    ContentProxies = cms.untracked.vstring('Reco/MuonCSC', 'Reco/Detector', 'Simulation/Hits')
)


