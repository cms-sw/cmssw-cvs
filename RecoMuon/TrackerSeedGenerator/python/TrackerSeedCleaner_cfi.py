import FWCore.ParameterSet.Config as cms

TrackerSeedCleanerCommon = cms.PSet(
    TrackerSeedCleaner = cms.PSet(
        cleanerFromSharedHits = cms.bool(True),
        # should be true only for tests
        ptCleaner = cms.bool(False),
        TTRHBuilder = cms.string('WithTrackAngle'),
        beamSpot = cms.InputTag("offlineBeamSpot"),
        directionCleaner = cms.bool(False)
    )
)


