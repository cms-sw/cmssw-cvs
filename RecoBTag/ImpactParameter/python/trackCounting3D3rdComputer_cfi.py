import FWCore.ParameterSet.Config as cms

# trackCounting3D3rd btag computer
trackCounting3D3rd = cms.ESProducer("TrackCountingESProducer",
    impactParameterType = cms.int32(0), ## 0 = 3D, 1 = 2D

    maximumDistanceToJetAxis = cms.double(0.07),
    deltaR = cms.double(-1.0), ## use cut from JTA

    maximumDecayLength = cms.double(5.0),
    nthTrack = cms.int32(3),
    trackQualityClass = cms.string("any")
)


