import FWCore.ParameterSet.Config as cms

process = cms.Process("exoticaElectronSummer08Skim")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
#number of Events to be skimmed.
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

#replace fileNames  with the file you want to skim
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    "file:/uscms_data/d1/jchen/18B6A5BB-EA33-DD11-91FD-000423D6B48C.root"
    )
)

#load the EventContent and Skim cff/i files.
process.load('SUSYBSMAnalysis.Skimming.ExoticaElectronSummer08_EventContent_cfi')
process.load('SUSYBSMAnalysis.Skimming.ExoticaElectronSummer08_cff')

process.eventInfo = cms.OutputModule (
    "AsciiOutputModule"
)
process.exoticaEOutputModule.fileName = cms.untracked.string('ExoticaElectronSkimSummer08.root')

#possible cut/evt content modification by production team
#process.exoticaERecoQalityCut.cut=  cms.string('pt > 15.0')
process.exoticaEHLT.HLTPaths = ['HLT1ElectronRelaxed','HLT1Electron']
#process.exoticaEHLT.HLTPaths = ['HLT_Ele10_SW_L1R']
#from ExoticaMuSkim.ExoticaMuSkim.exoticaESummer08_EventContent_cfi import *
#process.exoticaEOutputModule.outputCommands.extend(AODEventContent.outputCommands)
#process.exoticaEOutputModule.outputCommands.extend(SpecifiedEvenetContent.outputCommands)


# Select here HLT/RECO quantity cut. 
process.exoticaESkimPath=cms.Path(process.exoticaERecoQualitySeq)

process.endPath = cms.EndPath(process.eventInfo+process.exoticaEOutputModule)
