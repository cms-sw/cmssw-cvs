import FWCore.ParameterSet.Config as cms

process = cms.Process("IGUANA")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.source = cms.Source("PoolSource",
    firstEvent = cms.untracked.uint32(55),
    firstRun = cms.untracked.uint32(30),
    fileNames = cms.untracked.vstring('/store/relval/2008/6/9/RelVal-RelValH200ZZ4L-1212539383-STARTUP-2nd-02/0001/00C61FC1-0B37-DD11-9823-0018F3C3E3A6.root')
)

from FWCore.MessageLogger.MessageLogger_cfi import *

process.add_( cms.Service("VisConfigurationService",
    EnabledTwigs = cms.untracked.vstring('/Objects/CMS Event and Detector/DTs/Wheel 0', 
        '/Objects/CMS Event and Detector/Muon DT Event/DT 4DSegment'),
    Views = cms.untracked.vstring('3D Window'),
    ContentProxies = cms.untracked.vstring('Framework/EventSetup', 
        'Reco/MuonDT', 
        'Reco/MuonCSC', 
        'Reco/MuonRPC', 
        'Reco/Ecal', 
        'Reco/Hcal', 
        'Reco/Tracker', 
        'Reco/Muon', 
        'Reco/Jets' 
        'Simulation/Data Formats', 
        'Simulation/Hits'),
    VisAnimate = cms.untracked.bool(False),
    VisAnnotateGeometry = cms.untracked.bool(False),
    VisAutoPrint = cms.untracked.bool(False),
    VisAutoStart = cms.untracked.bool(False),
    VisBField = cms.untracked.double(4),
    VisCaloAnnotation = cms.untracked.bool(True),
    VisCaloEnergyScale = cms.untracked.double(1),
    VisCounterFrequency = cms.untracked.double(0.5),
    VisCounterMax = cms.untracked.double(12500),
    VisCounterMin = cms.untracked.double(-2500),
    VisEcalEnergyCut = cms.untracked.double(0.1),
    VisEnergyCut = cms.untracked.double(1),
    VisEventTimer = cms.untracked.uint32(30000),
    VisExceptionMessage = cms.untracked.string("GUI"),
    VisGsfTrackDirection = cms.untracked.bool(True),
    VisGsfTrackShape = cms.untracked.string("spline"),
    VisLegoScale = cms.untracked.double(10),
    VisMuonAnnotation = cms.untracked.bool(True),
    VisMuonDt = cms.untracked.double(5000),
    VisMuonT0 = cms.untracked.double(0),
    VisPhiDelta = cms.untracked.double(0.15708),
    VisPhiStart = cms.untracked.double(1.5708),
    VisPrintTimer = cms.untracked.uint32(30000),
    VisPropagatorIn = cms.untracked.double(0),
    VisPropagatorOut = cms.untracked.double(0),
    VisPropagatorStep = cms.untracked.double(0.05),
    VisPtCut = cms.untracked.double(1),
    VisRMax = cms.untracked.double(7.4),
    VisSelectedTrackNumber = cms.untracked.int32(-1),
    VisShowHitDets = cms.untracked.bool(True),
    VisSimTrackAnnotation = cms.untracked.bool(True),
    VisSimHitConnect = cms.untracked.bool(True),
    VisSimTrackTime = cms.untracked.double(10),
    VisVolatileGeometry = cms.untracked.bool(False),
    VisZMax = cms.untracked.double(11)
  )
)
