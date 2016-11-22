import FWCore.ParameterSet.Config as cms

process = cms.Process("VALID")


### RANDOM setting (change last digit(s) to make runs different !)
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")
#process.RandomNumberGeneratorService.generator.initialSeed = 12345XXXX

process.load("Configuration.StandardSequences.Simulation_cff")
process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration/StandardSequences/DigiToRaw_cff')
process.load('Configuration/StandardSequences/RawToDigi_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MC_31X_V3::All'

process.load("Configuration.StandardSequences.VtxSmearedGauss_cff")
process.load("Configuration.StandardSequences.GeometryHCAL_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.g4SimHits.UseMagneticField = False

process.load("DQMServices.Core.DQM_cfg")
process.DQM.collectorHost = ''

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

# Input source
process.source = cms.Source("PoolSource",
    firstEvent = cms.untracked.uint32(1),
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(2),
    noEventSort = cms.untracked.bool(True),	
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
'file:/afs/cern.ch/cms/data/CMSSW/Validation/HcalHits/data/3_1_X/mc_pi50_eta05.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)


process.hcalDigiAnalyzer = cms.EDAnalyzer("HcalDigiTester",
    digiLabel = cms.InputTag("simHcalUnsuppressedDigis"),
    outputFile = cms.untracked.string('HcalDigisValidation_HCALonly.root'),
    hcalselector = cms.untracked.string('all'),
    zside = cms.untracked.string('*')
)

process.hcalRecoAnalyzer = cms.EDAnalyzer("HcalRecHitsValidation",
    outputFile = cms.untracked.string('HcalRecHitsValidation_HCALonly.root'),
    eventype = cms.untracked.string('single'),
    mc = cms.untracked.string('yes'),
    sign = cms.untracked.string('*'),
    hcalselector = cms.untracked.string('all'),
    ecalselector = cms.untracked.string('no')
)


#------------------------------------

#process.simHcalDigis.HBlevel = -1000
#process.simHcalDigis.HElevel = -1000
#process.simHcalDigis.HOlevel = -1000
#process.simHcalDigis.HFlevel = -1000


process.VtxSmeared.SigmaX = 0.00001
process.VtxSmeared.SigmaY = 0.00001
process.VtxSmeared.SigmaZ = 0.00001


### Special - HcalOnly ---------------------------------------------------
process.hbhereco.digiLabel = cms.InputTag("hcalDigis")
process.horeco.digiLabel   = cms.InputTag("hcalDigis")
process.hfreco.digiLabel   = cms.InputTag("hcalDigis")


process.p = cms.Path(
 process.VtxSmeared * process.g4SimHits * process.mix *
 process.simHcalUnsuppressedDigis *
 process.simHcalDigis *
 process.hcalRawData *
 process.rawDataCollector *
 process.hcalDigis *
 process.hbhereco * process.horeco * process.hfreco *
 process.hcalDigiAnalyzer *
 process.hcalRecoAnalyzer 
)
