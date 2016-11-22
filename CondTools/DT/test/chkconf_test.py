# The following comments couldn't be translated into the new config version:

# Configuration file for EventSetupTest_t

import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    loadAll = cms.bool(True),
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('DTCCBConfigRcd'),
        tag = cms.string('conf_test')
    )),
    connect = cms.string('sqlite_file:testconf.db'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('.')
    )
)

process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(1),
    firstRun = cms.untracked.uint32(54544)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.conf = cms.EDFilter("DTConfigDump",
    token = cms.string('[DB=000000000-0000-0000-0000-000000000000][CNT=DTConfigList][CLID=9CB14BE8-30A2-DB11-9935-000E0C5CE283][TECH=00000B01][OID=00000004-00000000]'),
    authenticationPath = cms.string('.'),
    connect = cms.string('sqlite_file:testconf.db'),
    siteLocalConfig = cms.bool(True),
    dumpCCBKeys = cms.bool(False),
    dumpAllData = cms.bool(False)
)

process.p = cms.Path(process.conf)

