import FWCore.ParameterSet.Config as cms

process = cms.Process("hlxdqmlive")
process.load("DQMServices.Components.test.MessageLogger_cfi")

process.load("DQM.HLXMonitor.hlx_dqm_sourceclient_cfi")

process.load("DQMServices.Core.DQM_cfg")

process.load("DQMServices.Components.DQMEnvironment_cfi")

process.load("DQM.Integration.test.environment_cfi")
process.dqmEnv.subSystemFolder    = "HLX"

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.hlxQualityTester = cms.EDFilter("QualityTester",
    # default is 1
    prescaleFactor = cms.untracked.int32(10000),
    # use eventloop for testing only ! default is false
    # untracked bool testInEventloop = false
    # use qtest on endRun or endJob
    # untracked bool qtestOnEndRun = false
    # untracked bool qtestOnEndJob = false
    qtList = cms.untracked.FileInPath('DQM/HLXMonitor/test/HLXQualityTests.xml')
)

process.p = cms.Path(process.dqmEnv+process.hlxdqmsource+process.hlxQualityTester+process.dqmSaver)
process.hlxdqmsource.outputDir = '/home/dqmprolocal/output'


