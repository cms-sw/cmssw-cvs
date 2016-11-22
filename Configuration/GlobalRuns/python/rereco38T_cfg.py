
import FWCore.ParameterSet.Config as cms

process = cms.Process("Rec2")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("CondCore.DBCommon.CondDBSetup_cfi")


#process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(100) )

process.load("Configuration.EventContent.EventContentCosmics_cff")

#Drop old reco
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V9_SuperPointing_225-v3/0005/B8FB3273-5DFF-DD11-BEAB-00304875A7B5.root'
    ),
    inputCommands = cms.untracked.vstring('drop *')
)
process.source.inputCommands.extend(process.RAWEventContent.outputCommands)
process.source.inputCommands.append('drop *_*_*_Rec')
process.source.inputCommands.append('keep *_eventAuxiliaryHistoryProducer_*_*')


# output module
#

process.FEVT = cms.OutputModule("PoolOutputModule",
    process.FEVTEventContent,
    dataset = cms.untracked.PSet(dataTier = cms.untracked.string('RAW-RECO')),
    fileName = cms.untracked.string('promptRerecoCosmics.root')
)

process.FEVT.outputCommands.append('keep *_eventAuxiliaryHistoryProducer_*_*')
process.FEVT.outputCommands.append('keep CaloTowersSorted_calotoweroptmaker_*_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCALCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCALCTDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCCLCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCCLCTDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCComparatorDigiMuonDigiCollection_muonCSCDigis_MuonCSCComparatorDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCCorrelatedLCTDigiMuonDigiCollection_csctfDigis_*_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCCorrelatedLCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCCorrelatedLCTDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCRPCDigiMuonDigiCollection_muonCSCDigis_MuonCSCRPCDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCStripDigiMuonDigiCollection_muonCSCDigis_MuonCSCStripDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCWireDigiMuonDigiCollection_muonCSCDigis_MuonCSCWireDigi_*')
process.FEVT.outputCommands.append('keep cscL1TrackCSCDetIdCSCCorrelatedLCTDigiMuonDigiCollectionstdpairs_csctfDigis_*_*')
process.FEVT.outputCommands.append('keep DTChamberIdDTLocalTriggerMuonDigiCollection_muonDTDigis_*_*')
process.FEVT.outputCommands.append('keep DTLayerIdDTDigiMuonDigiCollection_muonDTDigis_*_*')
process.FEVT.outputCommands.append('keep intL1CSCSPStatusDigisstdpair_csctfDigis_*_*')
process.FEVT.outputCommands.append('keep L1MuDTChambPhContainer_dttfDigis_*_*')
process.FEVT.outputCommands.append('keep L1MuDTChambThContainer_dttfDigis_*_*')
process.FEVT.outputCommands.append('keep L1MuDTTrackContainer_dttfDigis_DATA_*')
process.FEVT.outputCommands.append('keep PixelDigiedmDetSetVector_siPixelDigis_*_*')
process.FEVT.outputCommands.append('keep recoCandidatesOwned_caloTowersOpt_*_*')
process.FEVT.outputCommands.append('keep RPCDetIdRPCDigiMuonDigiCollection_muonRPCDigis_*_*')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.10 $'),
    name = cms.untracked.string('$Source: /afs/cern.ch/work/c/cmsbuild/public/cvs/CMSSW/CMSSW/Configuration/GlobalRuns/python/Attic/rereco38T_cfg.py,v $'),
    annotation = cms.untracked.string('CRUZET Prompt Reco with DQM with Mag field at 3.8T')
)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) ## default is false


# Conditions (Global Tag is used here):
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "CRAFT_ALL_V11::All"
process.prefer("GlobalTag")

# Magnetic fiuld: force mag field to be 3.8 tesla
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

#Geometry
process.load("Configuration.StandardSequences.Geometry_cff")

# Real data raw to digi
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

# reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

# offline DQM
process.load("DQMOffline.Configuration.DQMOfflineCosmics_cff")
process.load("DQMServices.Components.MEtoEDMConverter_cff")

#L1 trigger validation
process.load("L1Trigger.HardwareValidation.L1HardwareValidation_cff")
process.load("L1Trigger.Configuration.L1Config_cff")
#process.load("L1TriggerConfig.CSCTFConfigProducers.CSCTFConfigProducer_cfi")
#process.load("L1TriggerConfig.CSCTFConfigProducers.L1MuCSCTFConfigurationRcdSrc_cfi")

#Paths
process.load("FWCore.Modules.eventAuxiliaryHistoryProducer_cfi")
process.allPath = cms.Path( process.RawToDigi * process.reconstructionCosmics * process.L1HardwareValidation_woGT * process.DQMOfflineCosmics * process.MEtoEDMConverter )

process.outpath = cms.EndPath(process.FEVT)


