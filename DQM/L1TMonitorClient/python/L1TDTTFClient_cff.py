import FWCore.ParameterSet.Config as cms

from DQM.L1TMonitorClient.L1TDTTFClient_cfi import *
l1tdttfqTester = cms.EDFilter("QualityTester",
    qtList = cms.untracked.FileInPath('DQM/L1TMonitorClient/data/L1TDTTF_QualityTests.xml'),
    QualityTestPrescaler = cms.untracked.int32(1),
    getQualityTestsFromFile = cms.untracked.bool(True)
)

l1tdttpgseqClient = cms.Sequence(l1tdttfqTester*l1tdttpgClient)


