import FWCore.ParameterSet.Config as cms

pfNoJet = cms.EDProducer(
    "TPPFJetsOnPFCandidates",
    verbose = cms.untracked.bool( False ),
    name = cms.untracked.string("noJet"),
    topCollection = cms.InputTag("pfJets"),
    bottomCollection = cms.InputTag("pfNoElectron"),
)
