import FWCore.ParameterSet.Config as cms

ISpySimTrack = cms.EDAnalyzer('ISpySimTrack',
                              iSpySimTrackTags = cms.VInputTag(cms.InputTag('g4SimHits')),
                              iSpySimVertexTags = cms.VInputTag(cms.InputTag('g4SimHits')),
                              iSpySimHitTags = cms.VInputTag(cms.InputTag('g4SimHits:BSCHits'),
                                                             cms.InputTag('g4SimHits:MuonCSCHits'),
                                                             cms.InputTag('g4SimHits:MuonDTHits'),
                                                             cms.InputTag('g4SimHits:MuonRPCHits'),
                                                             cms.InputTag('g4SimHits:TrackerHitsPixelBarrelHighTof'),
                                                             cms.InputTag('g4SimHits:TrackerHitsPixelBarrelLowTof'),
                                                             cms.InputTag('g4SimHits:TrackerHitsPixelEndcapHighTof'),
                                                             cms.InputTag('g4SimHits:TrackerHitsPixelEndcapLowTof'),
                                                             cms.InputTag('g4SimHits:TrackerHitsTECHighTof'),
                                                             cms.InputTag('g4SimHits:TrackerHitsTECLowTof'),
                                                             cms.InputTag('g4SimHits:TrackerHitsTIBHighTof'),
                                                             cms.InputTag('g4SimHits:TrackerHitsTIBLowTof'),
                                                             cms.InputTag('g4SimHits:TrackerHitsTIDHighTof'),
                                                             cms.InputTag('g4SimHits:TrackerHitsTIDLowTof'),
                                                             cms.InputTag('g4SimHits:TrackerHitsTOBHighTof'),
                                                             cms.InputTag('g4SimHits:TrackerHitsTOBLowTof'))
                              )

