import FWCore.ParameterSet.Config as cms

maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
source = cms.Source("EventStreamHttpReader",
   sourceURL = cms.string('http://srv-c2d05-14.cms:22100/urn:xdaq-application:lid=30'),
###    sourceURL = cms.string('http://srv-c2d05-05:50082/urn:xdaq-application:lid=29'),
    consumerPriority = cms.untracked.string('normal'),
    max_event_size = cms.int32(7000000),
    consumerName = cms.untracked.string('DQM Source'),
    SelectHLTOutput = cms.untracked.string('hltOutputDQM'),
    max_queue_depth = cms.int32(5),
    maxEventRequestRate = cms.untracked.double(150.0),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('*')
    ),
    headerRetryInterval = cms.untracked.int32(3)
)


