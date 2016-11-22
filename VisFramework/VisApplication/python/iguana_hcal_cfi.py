import FWCore.ParameterSet.Config as cms

VisConfigurationService = cms.Service("VisConfigurationService",
    Views = cms.untracked.vstring('RPhi Window', 'RZ Window', 'Lego Window'),
    ContentProxies = cms.untracked.vstring('Reco/Hcal', 'Reco/Calorimetry')
)


