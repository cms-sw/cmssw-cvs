
import FWCore.ParameterSet.Config as cms
# Select MC truth
from L1TriggerOffline.L1Analyzer.GenSelection_cff import *
# Select L1
from L1TriggerOffline.L1Analyzer.L1Selection_cff import *
# Histogram limits
from L1TriggerOffline.L1Analyzer.HistoLimits_cfi import *
# Root output file
from L1TriggerOffline.L1Analyzer.TFile_cfi import *
# Match generator and L1 jets 
MatchCenJetsMc = cms.EDFilter("TrivialDeltaRMatcher",
    src = cms.InputTag("SelectL1CenJets"),
    distMin = cms.double(0.5),
    matched = cms.InputTag("SelectGenCenJets")
)

# Match L1 and generator jets
MatchMcCenJets = cms.EDFilter("TrivialDeltaRMatcher",
    src = cms.InputTag("SelectGenCenJets"),
    distMin = cms.double(0.5),
    matched = cms.InputTag("SelectL1CenJets")
)

# Analyzer
L1AnalyzerCenJetsMC = cms.EDAnalyzer("L1Analyzer",
    histoLimits,
    EffMatchMapSource = cms.untracked.InputTag("MatchMcCenJets"),
    ReferenceSource = cms.untracked.InputTag("SelectGenCenJets"),
    CandidateSource = cms.untracked.InputTag("SelectL1CenJets"),
    ResMatchMapSource = cms.untracked.InputTag("MatchCenJetsMc")
)

# Define analysis sequence
L1CenJetMCAnalysis = cms.Sequence(L1CenJetSelection+GenCenJetSelection*MatchCenJetsMc+MatchMcCenJets*L1AnalyzerCenJetsMC)

