# Test pseudorandom number generation.
# In this process random numbers are generated
# in the TestRandomNumberServiceAnalyzer.  The
# state of the random number engines is saved in
# two places.  The states are saved in a text file
# that gets written first after beginRun and then
# is overwritten after processing each event.
# The states are also saved into the event data
# for each event in the root data file produced
# by the PoolOutputModule.

# The analyzer also writes each generated random number
# to a text file named testRandomService.txt, which
# can be examined to determine that the expected
# sequence of numbers has been generated.

import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST1")

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",

    # Tell the service to save the state of all engines
    # to a separate file written initially after beginJob
    # and then overwritten after processing each event
    saveFileName = cms.untracked.string('StashState.data'),

    # If the source generates random numbers, then
    # supply a seed for the source.  (EmptySource
    # does not, so we do not really need this here)
    # Internally, supplying a seed causes the service
    # to create one engine for the source.
    # sourceSeed = cms.untracked.uint32(181),

    # There is an alternate syntax which can be
    # used to specify the seed instead.  You can
    # use either syntax but should not use both.
    # For engines that require more than one seed
    # this form is required.
    # sourceSeedVector = cms.untracked.vuint32(7, 11),

    # Optionally, one can specify different types
    # of CLHEP engines.  Currently the only two
    # implemented are HepJamesRandom and RanecuEngine.
    # If you do not specify, you always get HepJamesRandom
    # by default.  This has no effect unless a seed is also
    # specified somewhere else.
    # sourceEngine = cms.untracked.string('RanecuEngine'),

    # HepJamesRandom requires one seed between 0 and 900000000
    # RanecuEngine requires two seeds between 0 and 2147483647

    # Modules are handled in an analogous way.  The only
    # difference being there can be more than one so there
    # is a parameter set to handle each of the three cases.
    # You need to specify seeds for all modules that request
    # random engines from the service (and only those modules
    # unless you want to waste a little CPU and memory).
    moduleSeeds = cms.PSet(
        t1 = cms.untracked.uint32(81),
        t3 = cms.untracked.uint32(83),
        t4 = cms.untracked.uint32(84),
        t5 = cms.untracked.uint32(191)
    ),

    moduleSeedVectors = cms.PSet(
        t2 = cms.untracked.vuint32(1, 2)
    ),

    moduleEngines = cms.PSet(
        t2 = cms.untracked.string('RanecuEngine'),
        t3 = cms.untracked.string('TRandom3'),
        t5 = cms.untracked.string('TRandom3')
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5)
)

# The RandomNumberGeneratorService should work with
# any kind of source
process.source = cms.Source("EmptySource")

process.t1 = cms.EDAnalyzer("TestRandomNumberServiceAnalyzer")
process.t2 = cms.EDAnalyzer("TestRandomNumberServiceAnalyzer")
process.t3 = cms.EDAnalyzer("TestRandomNumberServiceAnalyzer")
process.t4 = cms.EDAnalyzer("TestRandomNumberServiceAnalyzer")

# If you do not want to save the state of the random engines
# leave this line out.
# Including this producer causes the states to be stored
# in the event.  The label used here must be referenced
# in a later process to restore the state of the engines.
process.randomEngineStateProducer = cms.EDProducer("RandomEngineStateProducer")

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('testRandomService1.root')
)

process.p = cms.Path(process.t1+process.t2+process.t3+process.t4+process.randomEngineStateProducer)

process.o = cms.EndPath(process.out)
