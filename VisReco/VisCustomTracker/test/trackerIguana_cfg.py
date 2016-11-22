


import FWCore.ParameterSet.Config as cms

from FWCore.MessageLogger.MessageLogger_cfi import *
process = cms.Process("IGUANA")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'IDEAL_V5::All'
process.load("RecoTracker.TrackProducer.RefitterWithMaterial_cff")

process.load("VisFramework.VisFrameworkBase.VisConfigurationService_cff")
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:TTbar_219.root')
)
process.VisConfigurationService.VisExceptionMessage = "none"
process.VisConfigurationService.EnabledTwigs = (
                                               '*Pixel Digi*',
                                               '*SiStrip Digi*',
                                               '*SiStrip Cluster*')
process.VisConfigurationService.ContentProxies = ('Framework/EventSetup',
                                                  'Reco/Tracker',
                                                  'Reco/CustomTracker',
                                                  'Reco/Muon',
                                                  'Simulation/Hits')
process.VisConfigurationService.VisPhiDelta = 1.5708
process.VisConfigurationService.VisCuTkStripDigiProducer = "simSiStripDigis"
process.VisConfigurationService.VisCuTkStripDigiLabel  = "ZeroSuppressed"
process.VisConfigurationService.VisCuTkPixelDigiProducer = "simSiPixelDigis"
process.VisConfigurationService.VisCuTkPixelDigiLabel = "" 
process.VisConfigurationService.VisCuTkStripClusterProducer = "siStripClusters"
process.VisConfigurationService.VisCuTkStripClusterLabel = ""
process.VisConfigurationService.VisCuTkStripClusterLabel = ""
#   process.VisConfigurationService.VisCuTkPixelRechitProducer = "ignore"
#   process.VisConfigurationService.VisCuTkStripRechitProducer = "ignore"
#   process.VisConfigurationService.VisCuTkSimhitProducer = "ignore"
process.VisConfigurationService.detailedTrackPrintout = cms.untracked.string('false')
process.VisConfigurationService.VisCuTkCrossedModulesOnly = True
process.VisConfigurationService.VisCuTkTrackProducer = "TrackRefitter"
process.VisConfigurationService.trackLineThickness = cms.untracked.double(6.0)
process.VisConfigurationService.VisCuTkXSize = 250
process.VisConfigurationService.VisCuTkYSize = 200
process.VisConfigurationService.VisSiStripClusterPointSize = 10
process.VisConfigurationService.VisSiStripClusterRep = "strip"
process.VisConfigurationService.VisSiStripCluster2dRep = "strip"

process.mypath = cms.Path(process.siPixelRecHits*process.siStripMatchedRecHits*process.TrackRefitter)
