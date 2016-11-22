import FWCore.ParameterSet.Config as cms

process = cms.Process("IGUANA")
process.load("CondCore.DBCommon.CondDBSetup_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.prefer("GlobalTag")
process.GlobalTag.globaltag = "CRUZET4_V6P::All"
process.GlobalTag.connect = "frontier://PromptProd/CMS_COND_21X_GLOBALTAG"

# this is for reconstruction
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_30T_cff")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")
# end reconstruction

process.source = cms.Source("PoolSource",
   #firstEvent = cms.untracked.uint32(55),
   #firstRun = cms.untracked.uint32(30),
   #fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/630/0029CA89-9B71-DD11-8B56-001617C3B6FE.root')
   #fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/627/1694EC93-9371-DD11-A72E-0030487A18F2.root')
   fileNames = cms.untracked.vstring('file:/data/Run66714.root')
   #fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/638/024557CD-DE71-DD11-AB4B-001617C3B76E.root')
   #fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/723/0A4FAECE-1972-DD11-A578-001617DBD230.root')
   #fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/622/02C5AE67-8171-DD11-8D15-001D09F2983F.root')
)


from FWCore.MessageLogger.MessageLogger_cfi import *

process.load("VisFramework.VisFrameworkBase.VisConfigurationService_cff")

process.VisConfigurationService.VisExceptionMessage = "none"
process.VisConfigurationService.VisAutoStart =  True
process.VisConfigurationService.VisEventTimer = 0 
process.VisConfigurationService.Views = ('RPhi Window', '3D Window', 'RZ Window') 
process.VisConfigurationService.EnabledTwigs = (
                                               '*Pixel Digi*',
                                               '*Pixel Cluster*',
                                               '*Pixel Rechit*',
                                               '*SiStrip Rechit*',
                                               '*SiStrip Digi*',
                                               '*ctfWithMaterialTracksP5*',
                                               '*SiStrip Cluster*')

process.VisConfigurationService.ContentProxies = ('Framework/EventSetup',
                                                  'Reco/Tracker',
                                                  'Reco/CustomTracker',
                                                  'Reco/Muon')
                             
process.VisConfigurationService.VisPhiDelta = 1.5708 
process.VisConfigurationService.VisCuTkAutoIntegrate = True 
process.VisConfigurationService.VisCuTkTrackProducer = "ctfWithMaterialTracksP5"
process.VisConfigurationService.VisCuTkSimhitProducer = "ignore"
process.VisConfigurationService.VisCuTkStripDigiProducer = "siStripDigis"
process.VisConfigurationService.VisCuTkStripDigiLabel = "ZeroSuppressed"
process.VisConfigurationService.VisCuTkPixelDigiProducer = "siPixelDigis"
process.VisConfigurationService.VisCuTkPixelDigiLabel = ""
process.VisConfigurationService.VisCuTkStripClusterProducer = "siStripClusters"
process.VisConfigurationService.VisCuTkStripClusterLabel =  ""
process.VisConfigurationService.VisSiStripClusterPointSize = 10
process.VisConfigurationService.VisSiStripCluster2dRep = "strip"
process.VisConfigurationService.trackLineThickness = cms.untracked.double(6.0)
process.VisConfigurationService.detailedTrackPrintout = cms.untracked.string('false')

process.p = cms.Path( process.RawToDigi_woGCT * process.reconstructionCosmics)

