import FWCore.ParameterSet.Config as cms

ecalBarrelStatusFlagsTask = cms.EDAnalyzer("EBStatusFlagsTask",
    prefixME = cms.untracked.string('EcalBarrel'),
    enableCleanup = cms.untracked.bool(False),
    mergeRuns = cms.untracked.bool(False),    
    EcalRawDataCollection = cms.InputTag("ecalEBunpacker")
)

