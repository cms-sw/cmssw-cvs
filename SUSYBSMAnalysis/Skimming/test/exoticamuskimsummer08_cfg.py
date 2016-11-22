import FWCore.ParameterSet.Config as cms

process = cms.Process("exoticaMuonSummer08Skim")
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
process.load('SUSYBSMAnalysis.Skimming.ExoticaMuonSummer08_EventContent_cfi')
process.load('SUSYBSMAnalysis.Skimming.ExoticaMuonSummer08_cff')

process.eventInfo = cms.OutputModule (
    "AsciiOutputModule"
)

process.exoticaMuOutputModule.fileName = cms.untracked.string('ExoticaMuonSkimSummer08.root')

#possible cut/evt content modification by production team
#process.exoticaMuRecoQalityCut.cut=  cms.string('pt > 15.0')
#process.exoticaMuHLT.HLTPaths = ['HLT_Mu3']
process.exoticaMuHLT.HLTPaths = ['HLT1MuonNonIso','HLT2MuonNonIso']
#from ExoticaMuSkim.ExoticaMuSkim.exoticaMuSummer08_EventContent_cfi import *
#process.exoticaMuOutputModule.outputCommands.extend(AODEventContent.outputCommands)
#process.exoticaMuOutputModule.outputCommands.extend(SpecifiedEvenetContent.outputCommands)

process.exoticaMuSkimPath=cms.Path(process.exoticaMuRecoQualitySeq)
process.endPath = cms.EndPath(process.eventInfo+process.exoticaMuOutputModule)
