import FWCore.ParameterSet.Config as cms

from FWCore.MessageLogger.MessageLogger_cfi import *
VisConfigurationService = cms.Service("VisConfigurationService",
    EnabledTwigs = cms.untracked.vstring('/Objects/CMS Event and Detector/Reco Detector/DTs/Wheel 0', 
        '/Objects/CMS Event and Detector/Muon DT Event/DT 4DSegment'),
    VisAutoStart = cms.untracked.bool(False),
    Views = cms.untracked.vstring('3D Window'),
    ContentProxies = cms.untracked.vstring('Framework/EventSetup', 
        'Reco/MuonDT', 
        'Reco/MuonCSC', 
        'Reco/MuonRPC', 
        'Reco/Ecal', 
        'Reco/Hcal', 
        'Reco/Tracker', 
        'Reco/Muon', 
        'Reco/Jets', 
        'Simulation/Data Formats', 
        'Simulation/Hits'),
    VisLegoScale = cms.untracked.double(2.0),
    VisCaloAnnotation = cms.untracked.bool(False),
    VisExceptionMessage = cms.untracked.string('none'),
    VisCaloEnergyScale = cms.untracked.double(1.0),
    VisEventTimer = cms.untracked.uint32(30000),
    VisEnergyCut = cms.untracked.double(1.0)
)
