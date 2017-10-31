import FWCore.ParameterSet.Config as cms

import FastSimulation.Tracking.TrackCandidateProducer_cfi
iterativeFifthTrackCandidatesWithPairs = FastSimulation.Tracking.TrackCandidateProducer_cfi.trackCandidateProducer.clone()
iterativeFifthTrackCandidates = cms.Sequence(iterativeFifthTrackCandidatesWithPairs)
iterativeFifthTrackCandidatesWithPairs.SeedProducer = cms.InputTag("iterativeFifthSeeds","TobTecLayerPairs")
iterativeFifthTrackCandidatesWithPairs.TrackProducers = ['firstfilter','secfilter','thfilter','foufilter']
iterativeFifthTrackCandidatesWithPairs.KeepFittedTracks = False
iterativeFifthTrackCandidatesWithPairs.MinNumberOfCrossedLayers = 3

