# The following comments couldn't be translated into the new config version:

# upload to database 

#string timetype = "timestamp"    

import FWCore.ParameterSet.Config as cms

process = cms.Process("Builder")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring(''),
    BadModuleBuilder = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    destinations = cms.untracked.vstring('ModuleLVBuilder.log')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(1),
    firstRun = cms.untracked.uint32(1)
)

process.load("CalibTracker.SiStripESProducers.fake.SiStripModuleHVFakeESSource_cfi")
process.load("CalibTracker.SiStripESProducers.DBWriter.SiStripModuleHVDummyDBWriter_cfi")

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(2),
        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
    ),
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string('sqlite_file:dbfile.db'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('SiStripModuleHVRcd'),
        tag = cms.string('SiStripModuleLVOff_Ideal_31X')
    ))
)

process.siStripModuleHVDummyDBWriter.record=process.PoolDBOutputService.toPut[0].record
process.p1 = cms.Path(process.siStripModuleHVDummyDBWriter)


