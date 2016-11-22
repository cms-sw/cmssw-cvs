import FWCore.ParameterSet.Config as cms

ctfWithMaterialTracksDAF = cms.EDProducer("DAFTrackProducer",
    src = cms.InputTag("DAFTrackCandidateMaker"),
    UpdatorName = cms.string('SiTrackerMultiRecHitUpdator'),
    beamSpot = cms.InputTag("offlineBeamSpot"),
    Fitter = cms.string('MRHFittingSmoother'),
    MeasurementCollector = cms.string('groupedMultiRecHitCollector'),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithAngleAndTemplate'),
    Propagator = cms.string('RungeKuttaTrackerPropagator'),
    MinHits = cms.int32(3),
    #set to "", the secondary hit pattern will not be filled (backward compatible with DetLayer=0)                               
    NavigationSchool = cms.string('')                              
)


