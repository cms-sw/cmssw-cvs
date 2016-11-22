import FWCore.ParameterSet.Config as cms

process = cms.Process("SKIM")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# TopQuarkAnalysis
# skim: topSemiLepElectron
process.load("TopQuarkAnalysis.TopSkimming.topSemiLepElectron_SkimPaths_cff")

process.load("TopQuarkAnalysis.TopSkimming.topSemiLepElectronOutputModule_cfi")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    name = cms.untracked.string('$Source: /afs/cern.ch/work/c/cmsbuild/public/cvs/CMSSW/CMSSW/Configuration/Skimming/python/Attic/PDElectron_Skims8_cfg.py,v $'),
    annotation = cms.untracked.string('skims to be run on PDElectron, file 3/3')
)
process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring('ProductNotFound'),
    makeTriggerResults = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(True)
)
process.LoadAllDictionaries = cms.Service("LoadAllDictionaries")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/RelVal/2007/9/3/RelVal-RelValZTT-1188837971/0002/0279DD81-655A-DC11-8280-0016177CA778.root')
)

process.TopQuarkAnalysis = cms.EndPath(process.topSemiLepElectronOutputModule)

