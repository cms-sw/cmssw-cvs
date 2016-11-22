import FWCore.ParameterSet.Config as cms

process = cms.Process("IGUANA") 
process.load("Configuration.StandardSequences.Geometry_cff") 
process.load("Configuration.StandardSequences.MagneticField_30T_cff") 
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")
#process.load("Configuration.StandardSequences.Reconstruction_cff") 
#process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
 
process.source = cms.Source("PoolSource",
    #firstEvent = cms.untracked.uint32(11732),
    #firstRun = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/627/1694EC93-9371-DD11-A72E-0030487A18F2.root') 
    #fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/b/boudoul/219/Cosmic0T/GenSimRaw/CosmicGenSimRaw0T_149.root')
)

from FWCore.MessageLogger.MessageLogger_cfi import * 

#process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff") 
process.prefer("GlobalTag") 
process.GlobalTag.connect = "frontier://PromptProd/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "CRUZET4_V5P::All"


process.load("VisFramework.VisFrameworkBase.VisConfigurationService_cff") 
process.VisConfigurationService.VisAutoStart = False 
process.VisConfigurationService.VisCaloAnnotation = False 
process.VisConfigurationService.EnabledTwigs = ('/Objects/CMS Event and Detector/Muon DT Event/DT 4DSegment', 
                                                '/Objects/CMS Event and Detector/Muon RPC Event/RPC RecHit', 
                                                '*ECAL Event/Rec hits*', 
                                                '*Pixel Digi*', 
                                                '*SiStrip Digi*', 
                                                '*SiStrip Cluster*', 
                                                '/Objects/Event Collections/Run and Event Number' 
                                                ) 
process.VisConfigurationService.ContentProxies = ('Framework/EventSetup', 
                                                  'Reco/MuonDT', 
                                                  'Reco/MuonCSC', 
                                                  'Reco/MuonRPC', 
                                                  'Reco/Ecal', 
                                                  'Reco/Hcal', 
                                                  'Reco/Tracker', 
                                                  'Reco/Muon', 
                                                  'Reco/Jets', 
                                                  'Reco/Detector'
                                                  )
process.p = cms.Path( process.RawToDigi_woGCT * process.reconstructionCosmics)
