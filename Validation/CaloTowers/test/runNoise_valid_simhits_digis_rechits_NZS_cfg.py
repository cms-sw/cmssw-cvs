import FWCore.ParameterSet.Config as cms

process = cms.Process("VALID")
process.load("Validation.CaloTowers.Validation_hcalonly_cfi")

process.load("Configuration.StandardSequences.GeometryHCAL_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.g4SimHits.UseMagneticField = False

process.PoolSource.fileNames = ['file:/afs/cern.ch/cms/data/CMSSW/Validation/HcalHits/data/3_1_X/mc_nue.root']

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.hcalDigiAnalyzer.outputFile = 'HcalDigisValidation_noise_NZS.root'
process.hcalDigiAnalyzer.hcalselector = 'noise'

process.hcalRecoAnalyzer.outputFile = 'HcalRecHitValidation_noise_NZS.root'
process.hcalRecoAnalyzer.hcalselector = 'noise'
process.hcalRecoAnalyzer.ecalselector = 'no'
process.hcalRecoAnalyzer.eventype = 'single'

process.simHcalDigis.HBlevel = -1000
process.simHcalDigis.HElevel = -1000
process.simHcalDigis.HOlevel = -1000
process.simHcalDigis.HFlevel = -1000
process.simHcalDigis.useConfigZSvalues = 1

process.p = cms.Path(process.VtxSmeared*process.g4SimHits*process.mix*process.simHcalUnsuppressedDigis*process.simHcalDigis*process.hbhereco*process.horeco*process.hfreco*process.hcalDigiAnalyzer*process.hcalRecoAnalyzer)
