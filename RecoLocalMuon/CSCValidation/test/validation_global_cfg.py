import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

# Standard stuff needed for the unpacking and local reco
process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
# for Beam
#process.load("Configuration.StandardSequences.Reconstruction_cff")
# for Cosmics
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

# specify the global tag to use..
# more info and a list of current tags can be found at
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.GlobalTag.globaltag = 'CRAFT_30X::All'

# this points to CRAFT '08 data (r68100) as an example
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/0004DBD7-DBA6-DD11-BB52-001D09F2841C.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/0020C9A5-EAA6-DD11-B9F3-001617C3B66C.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/004D59CB-C1A6-DD11-ABB2-0019B9F72BFF.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/0055CF70-DAA6-DD11-B72C-001D09F24664.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/0087E3B2-D9A6-DD11-A579-000423D98B6C.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/00942EDE-B0A6-DD11-A502-001D09F2424A.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/00CE4AC7-F8A6-DD11-A160-001D09F23A07.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/00F965E3-91A6-DD11-B8B4-001617E30D40.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/02B0C048-BEA6-DD11-97D9-0019B9F704D6.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/02B20B31-CFA6-DD11-9286-001D09F26C5C.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/02EC6328-C8A6-DD11-BB5A-001D09F23A84.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/02EF6E2D-97A6-DD11-BF26-001D09F2932B.root',
        '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/100/0896252E-A1A6-DD11-9028-001617C3B79A.root'
)
)

# recommend at least 100k events for normal Cosmic running (less if running over CSCSkim)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100000)
)

# This is the CSCValidation package minimum block.  There are more input variables which
# can be set.  Check src/CSCValidation.cc to see what they are.
process.cscValidation = cms.EDFilter("CSCValidation",
    # name of file which will contain output
    rootFileName = cms.untracked.string('validationHists_global.root'),
    # basically turns on/off residual plots which use simhits
    isSimulation = cms.untracked.bool(False),
    # stores a tree of info for the first 1.5M rechits and 2M segments
    # used to make 2D scatter plots of global positions.  Significantly increases
    # size of output root file, so beware...
    writeTreeToFile = cms.untracked.bool(True),
    # mostly for MC and RECO files which may have dropped the digis
    useDigis = cms.untracked.bool(True),
    # lots of extra, more detailed plots
    detailedAnalysis = cms.untracked.bool(False),
    # Input tags for various collections CSCValidation looks at
    stripDigiTag = cms.InputTag("muonCSCDigis","MuonCSCStripDigi"),
    wireDigiTag = cms.InputTag("muonCSCDigis","MuonCSCWireDigi"),
    compDigiTag = cms.InputTag("muonCSCDigis","MuonCSCComparatorDigi"),
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

# for RECO (if digis were not saved, make sure to set useDigis = False)
#process.p = cms.Path(process.cscValidation)
# for RAW with just local level CSC Stuff
process.p = cms.Path(process.muonCSCDigis * process.csc2DRecHits * process.cscSegments *
                     process.cscValidation)
# for RAW (Cosmics) if you want to look at Trigger and Standalone info
#process.p = cms.Path(process.gtDigis *
#                     process.muonCSCDigis * process.csc2DRecHits * process.cscSegments *
#                     process.muonRPCDigis * process.rpcRecHits *
#                     process.offlineBeamSpot * process.CosmicMuonSeedEndCapsOnly*process.cosmicMuonsEndCapsOnly *
#                     process.cscValidation)

