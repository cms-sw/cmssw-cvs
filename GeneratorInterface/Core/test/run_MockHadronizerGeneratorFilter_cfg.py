import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.load("FWCore.Framework.test.cmsExceptionsFatal_cff")

# The following three lines reduce the clutter of repeated printouts
# of the same exception message.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cerr']
process.MessageLogger.statistics = []
process.MessageLogger.fwkJobReports = []

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(11))

process.source = cms.Source("EmptySource")

process.mockgf = cms.EDFilter("MockHadronizerGeneratorFilter")
process.counter = cms.EDAnalyzer("EventCounterTester", expected=cms.int32(11))

process.p = cms.Path(process.mockgf * process.counter)
