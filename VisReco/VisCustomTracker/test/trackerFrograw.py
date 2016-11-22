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
process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(1000) )

from FWCore.MessageLogger.MessageLogger_cfi import * 

#process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff") 
process.prefer("GlobalTag") 
process.GlobalTag.connect = "frontier://PromptProd/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "CRUZET4_V5P::All"

#FROG
process.load("Visualisation.Frog.Frog_Analyzer_cff")

process.frog.OutputFile = '/tmp/gzito/Run58627.vis'

process.frog.GenParticlesProducers    = []                     #Commented Lines means default value
process.frog.SimVertexProducers       = []                     #Commented Lines means default value
process.frog.SimTrackProducers        = []                     #Commented Lines means default value
process.frog.SimHitProducers          = []                     #Commented Lines means default value
process.frog.SimCaloHitProducers      = []                     #Commented Lines means default value
process.frog.SiStripClusterProducers  = []                     #Commented Lines means default value
#process.frog.EcalRecHitProducers      = []                     #Commented Lines means default value
#process.frog.HcalHBHERecHitProducers  = []                     #Commented Lines means default value
#process.frog.HcalHORecHitProducers    = []                     #Commented Lines means default value
#process.frog.HcalHFRecHitProducers    = []                     #Commented Lines means default value
#process.frog.DTSegmentProducers       = []                     #Commented Lines means default value
#process.frog.CSCSegmentProducers      = []                     #Commented Lines means default value
#process.frog.RPCHitsProducers         = []                     #Commented Lines means default value
process.frog.CaloTowersProducers      = []                     #Commented Lines means default value
process.frog.NIProducers              = []                     #Commented Lines means default value
process.frog.TrackProducers           = ['cosmictrackfinderP5' ]                     #Commented Lines means default value
#process.frog.TrajectoryProducers      = ['TrackRefitter', 'globalMuons']                     #Commented Lines means default value
process.frog.TrajectoryProducers      = []                     #Commented Lines means default value
process.frog.BasicJetsProducers       = []                     #Commented Lines means default value
process.frog.CaloJetsProducers        = []                     #Commented Lines means default value
process.frog.RecoCandidateProducers   = []                     #Commented Lines means default value
process.frog.CaloMETProducers         = []                     #Commented Lines means default value

process.p = cms.Path( process.RawToDigi_woGCT * process.reconstructionCosmics * process.frog)
