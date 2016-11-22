import FWCore.ParameterSet.Config as cms

process = cms.Process("exoticaskimaod")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
#number of Events to be skimmed.
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

#replace fileNames  with the file you want to skim
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    "file:test.root"
    )
)

from Configuration.EventContent.EventContent_cff import *
process.exoticaAODOutputModule = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring("drop *"),
    fileName = cms.untracked.string('exoticatestaod.root')
  )
process.exoticaAODOutputModule.outputCommands.extend(AODEventContent.outputCommands)

process.endPath = cms.EndPath(process.exoticaAODOutputModule)
