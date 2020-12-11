import FWCore.ParameterSet.Config as cms

process = cms.Process("IGUANA") 
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff") 
process.load("Configuration.StandardSequences.Geometry_cff") 
process.load("Configuration.StandardSequences.MagneticField_cff") 
process.load("Configuration.StandardSequences.Reconstruction_cff") 
 
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_SuperPointing_v1/0000/48DAB45A-AD6E-DD11-80FE-001731AF67C5.root')
)

from FWCore.MessageLogger.MessageLogger_cfi import * 
 
process.prefer("GlobalTag") 
process.GlobalTag.connect = "frontier://PromptProd/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "CRZT210_V3P::All"

process.load("VisFramework.VisFrameworkBase.VisConfigurationService_cff") 
process.VisConfigurationService.VisAutoStart = True 
process.VisConfigurationService.VisCaloAnnotation = False 
process.VisConfigurationService.Views = ('RPhi Window', 'RZ Window') 
process.VisConfigurationService.EnabledTwigs = ('/Objects/CMS Event and Detector/Muon DT Event/DT 4DSegment', 
                                                '/Objects/CMS Event and Detector/Muon RPC Event/RPC RecHit', 
                                                '/Objects/CMS Event and Detector/ECAL Event/Rec hits', 
                                                '/Objects/CMS Event and Detector/ECAL Event/Uncalibrated rec hits', 
                                                '/Objects/CMS Event and Detector/Tracker Event/Pixel Digis', 
                                                '/Objects/CMS Event and Detector/Tracker Event/SiStrip Digis', 
                                                '/Objects/CMS Event and Detector/Tracker Event/SiStrip Cluster', 
                                                '/Objects/CMS Event and Detector/Tracker Event/SiStrip Rechits 2D', 
                                                '/Objects/CMS Event and Detector/Tracker Event/Reconstructed Tracks/[N/A] Track (recoTracks:ctfWithMaterialTracksP5::IGUANA)', 
                                                '/Objects/CMS Event and Detector/HCAL Event/Hcal Barrel and Endcap RecHits', 
                                                '/Objects/CMS Event and Detector/Muon/Barrel/Drift Tubes', 
                                                '/Objects/CMS Event and Detector/Muon/Endcap/CSCs', 
                                                '/Objects/CMS Event and Detector/ECAL/Barrel', 
                                                '/Objects/CMS Event and Detector/HCAL/Barrel/Towers', 
                                                '/Objects/CMS Event and Detector/Tracker/Tracker', 
                                                '/Objects/CMS Event and Detector/Magnet', 
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
                                                  'Reco/Detector') 
 
 
