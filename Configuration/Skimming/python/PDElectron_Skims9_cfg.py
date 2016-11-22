import FWCore.ParameterSet.Config as cms

process = cms.Process("SKIM")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# EgammaAnalysis
# all of their skims will be run here
process.load("EgammaAnalysis.Configuration.EgammaAnalysis_SkimPaths_cff")

process.load("EgammaAnalysis.CSA07Skims.egammaVeryHighEtOutputModuleRECOSIM_cfi")

process.load("EgammaAnalysis.CSA07Skims.egammaVeryHighEtOutputModuleAODSIM_cfi")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    name = cms.untracked.string('$Source: /afs/cern.ch/work/c/cmsbuild/public/cvs/CMSSW/CMSSW/Configuration/Skimming/python/Attic/PDElectron_Skims9_cfg.py,v $'),
    annotation = cms.untracked.string('skims to be run on PDElectron, file 1/2')
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
    fileNames = cms.untracked.vstring('/store/mc/2007/9/17/RelVal-W1jet_100ptw300-alpgen-1190037682/0002/0AFADF7A-3565-DC11-BCE6-000423D944DC.root')
)

process.EgammaAnalysis = cms.EndPath(process.egammaVeryHighEtOutputModuleAODSIM+process.egammaVeryHighEtOutputModuleRECOSIM)

