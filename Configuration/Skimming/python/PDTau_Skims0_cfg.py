import FWCore.ParameterSet.Config as cms

process = cms.Process("SKIM")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# ElectroWeakAnalysis
# skim: EWK di-tau
process.load("ElectroWeakAnalysis.ZReco.zToTauTau_DoubleTau_SkimPaths_cff")

# skim: EWK single tau
process.load("ElectroWeakAnalysis.WReco.wToTauNu_SkimPaths_cff")

process.load("ElectroWeakAnalysis.ZReco.zToTauTau_DoubleTauOutputModule_cfi")

process.load("ElectroWeakAnalysis.WReco.wToTauNuOutputModule_cfi")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    name = cms.untracked.string('$Source: /afs/cern.ch/work/c/cmsbuild/public/cvs/CMSSW/CMSSW/Configuration/Skimming/python/Attic/PDTau_Skims0_cfg.py,v $'),
    annotation = cms.untracked.string('skims to be run on PDTau')
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

process.ElectroWeakAnalysis = cms.EndPath(process.zToTauTau_DoubleTauOutputModule+process.wToTauNuOutputModule)

