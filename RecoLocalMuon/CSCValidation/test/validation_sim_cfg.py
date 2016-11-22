import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration/StandardSequences/RawToDigi_cff")

# this may be necessary if want to reconstruct rechits but get a 
# "product not found" error about the digis when running on simulation
#process.csc2DRecHits = cms.InputTag("simMuonCSCDigis","MuonCSCWireDigi")
#process.csc2DRecHits = cms.InputTag("simMuonCSCDigis","MuonCSCStripDigi")

# specify the global tag to use..
# more info and a list of current tags can be found at
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.GlobalTag.globaltag = 'IDEAL_30X::All'


# points to CMSSW_3_1_0_pre2 single muon (Pt = 100) relval sample.  Sim data must contain
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_1_0_pre2/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_30X_v1/0000/9C6DB92D-7203-DE11-9143-000423D98AF0.root',
        '/store/relval/CMSSW_3_1_0_pre2/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_30X_v1/0000/C6419202-7103-DE11-A5AA-000423D94534.root',
        '/store/relval/CMSSW_3_1_0_pre2/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_30X_v1/0001/F482F867-DB03-DE11-B212-000423D6CA72.root'
)
)

# recommend at least 10k events (single Muon Simulation)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

process.cscValidation = cms.EDFilter("CSCValidation",
    # name of file which will contain output
    rootFileName = cms.untracked.string('validationHists_sim.root'),
    # basically turns on/off residual plots which use simhits
    isSimulation = cms.untracked.bool(True),
    # stores a tree of info for the first 1.5M rechits and 2M segments
    # used to make 2D scatter plots of global positions.  Significantly increases
    # size of output root file, so beware...
    writeTreeToFile = cms.untracked.bool(True),
    # mostly for MC and RECO files which may have dropped the digis
    useDigis = cms.untracked.bool(True),
    # lots of extra, more detailed plots
    detailedAnalysis = cms.untracked.bool(False),
    # Input tags for various collections CSCValidation looks at
    stripDigiTag = cms.InputTag("simMuonCSCDigis","MuonCSCStripDigi"),
    wireDigiTag = cms.InputTag("simMuonCSCDigis","MuonCSCWireDigi"),
    compDigiTag = cms.InputTag("simMuonCSCDigis","MuonCSCComparatorDigi"),
    cscRecHitTag = cms.InputTag("csc2DRecHits"),
    cscSegTag = cms.InputTag("cscSegments"),
    # set to true to only look at events with CSC L1A
    useTrigger = cms.untracked.bool(False),
    # set to true to skip "messy" events
    filterCSCEvents = cms.untracked.bool(False),
    # do you want to look at STA muons?
    makeStandalonePlots = cms.untracked.bool(False),
    # STA tag for cosmics
    saMuonTag = cms.InputTag("cosmicMuonsEndCapsOnly"),
    l1aTag = cms.InputTag("gtDigis"),
    simHitTag = cms.InputTag("g4SimHits", "MuonCSCHits")
)

# for RECO or SIM  (if digis were not saved, make sure to set useDigis = False)
#process.p = cms.Path(process.cscValidation)
# for RAW with just local level CSC Stuff or SIM with RAW included
process.p = cms.Path(process.muonCSCDigis * process.csc2DRecHits * process.cscSegments *
                     process.cscValidation)
# for RAW (Cosmics) if you want to look at Trigger and Standalone info
#process.p = cms.Path(process.gtDigis *
#                     process.muonCSCDigis * process.csc2DRecHits * process.cscSegments *
#                     process.muonRPCDigis * process.rpcRecHits *
#                     process.offlineBeamSpot * process.CosmicMuonSeedEndCapsOnly*process.cosmicMuonsEndCapsOnly *
#                     process.cscValidation)

