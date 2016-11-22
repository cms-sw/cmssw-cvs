import FWCore.ParameterSet.Config as cms

HLTMuonTrackMassFilter = cms.EDFilter("HLTMuonTrackMassFilter",
                                      BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
                                      CandTag = cms.InputTag("hltL3MuonCandidates"),
                                      PreviousCandTag = cms.InputTag( '' ),
                                      TrackTag = cms.InputTag( '' ),
                                      MinMasses = cms.vdouble( 2.8 ),
                                      MaxMasses = cms.vdouble( 3.4 ),
                                      checkCharge = cms.bool(True),
                                      MinTrackPt = cms.double(0.),
                                      MinTrackP = cms.double(3.),
                                      MaxTrackEta = cms.double(999.),
                                      MaxTrackDxy = cms.double(999.),
                                      MaxTrackDz = cms.double(999.),
                                      MinTrackHits = cms.int32(5),
                                      MaxTrackNormChi2 = cms.double(999999999.),
                                      MaxDzMuonTrack = cms.double(0.5),
                                      SaveTag = cms.untracked.bool( False )
                                      )
