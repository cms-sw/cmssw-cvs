import FWCore.ParameterSet.Config as cms

from FastSimulation.Tracking.IterativeFirstSeedProducer_cff import *
from FastSimulation.Tracking.IterativeFirstCandidateProducer_cff import *
from FastSimulation.Tracking.IterativeFirstTrackProducer_cff import *
from FastSimulation.Tracking.IterativeFirstTrackMerger_cfi import *
from FastSimulation.Tracking.IterativeFirstTrackFilter_cff import *
iterativeFirstTracking = cms.Sequence(iterativeFirstSeeds
                                      +iterativeFirstTrackCandidatesWithTriplets
                                      +iterativeFirstTrackCandidatesWithPairs
                                      +iterativeFirstTracks
                                      +iterativeZeroTrackMerging
                                      +iterativeZeroTrackFiltering
                                      +iterativeFirstTrackMerging
                                      +iterativeFirstTrackFiltering)
                                      

