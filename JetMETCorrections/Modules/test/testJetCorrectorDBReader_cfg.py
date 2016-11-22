import FWCore.ParameterSet.Config as cms

process = cms.Process("myprocess")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START38_V6::All'
process.GlobalTag.connect = 'sqlite_file:START38_V6.db'

from JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff import *

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1)
        )

process.source = cms.Source("EmptySource")

process.demo2 = cms.EDAnalyzer('JetCorrectorDBReader', 
        label          = cms.untracked.string('AK5Calo'),
        createTextFile = cms.untracked.bool(True)
)


process.demo3 = cms.EDAnalyzer('JetCorrectorDBReader', 
        label          = cms.untracked.string('AK5PF'),
        createTextFile = cms.untracked.bool(True)
)

process.p = cms.Path(process.demo2 * process.demo3)
