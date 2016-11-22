# The following comments couldn't be translated into the new config version:

# include "FWCore/MessageLogger/data/MessageLogger.cfi"

import FWCore.ParameterSet.Config as cms

process = cms.Process("SKIM")
# HeavyFlavorAnalysis
# skim: onia
process.load("HeavyFlavorAnalysis.Configuration.HeavyFlavorAnalysis_SkimPaths_cff")

process.load("HeavyFlavorAnalysis.Skimming.jpsiToMuMuOutputModuleAODSIM_cfi")

process.load("HeavyFlavorAnalysis.Skimming.upsilonToMuMuOutputModuleAODSIM_cfi")

process.load("HeavyFlavorAnalysis.Skimming.bToMuMuOutputModuleAODSIM_cfi")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    name = cms.untracked.string('$Source: /afs/cern.ch/work/c/cmsbuild/public/cvs/CMSSW/CMSSW/Configuration/Skimming/python/Attic/PDMuon_Skims1_cfg.py,v $'),
    annotation = cms.untracked.string('skims to be run on PDMuon')
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
process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    destinations = cms.untracked.vstring('cout')
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/mc/2007/9/17/RelVal-W1jet_100ptw300-alpgen-1190037682/0002/0AFADF7A-3565-DC11-BCE6-000423D944DC.root')
)

process.HeavyFlavorAnalysis = cms.EndPath(process.jpsiToMuMuOutputModuleAODSIM+process.upsilonToMuMuOutputModuleAODSIM+process.bToMuMuOutputModuleAODSIM)

