import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.CondDBCommon.connect = 'sqlite_file:testconf.db'
process.CondDBCommon.DBParameters.authenticationPath = '.'

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDBCommon,
    logconnect = cms.untracked.string('sqlite_file:log.db'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('DTCCBConfigRcd'),
        tag = cms.string('conf_test')
    ))
)

process.source = cms.Source("EmptyIOVSource",
    timetype   = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue  = cms.uint64(1),
    interval   = cms.uint64(1)
)

process.conf_o2o = cms.EDAnalyzer("DTCCBConfigPopConAnalyzer",
    name = cms.untracked.string('DTCCBConfig'),
    Source = cms.PSet(
        DBParameters = cms.PSet(
            messageLevel = cms.untracked.int32(0),
            authenticationPath = cms.untracked.string('.')
        ),
        onlineDB = cms.string('sqlite_file:dummy_online.db'),

        offlineDB = cms.string('sqlite_file:testconf.db'),
        tag = cms.string('conf_test'),
        token = cms.string('[DB=00000000-0000-0000-0000-000000000000][CNT=DTConfigList][CLID=9CB14BE8-30A2-DB11-9935-000E0C5CE283][TECH=00000B01][OID=00000004-00000000]'),
        onlineAuthentication = cms.string('.')
    ),
    SinceAppendMode = cms.bool(True),
    record = cms.string('DTCCBConfigRcd'),
    loggingOn = cms.untracked.bool(True),
    debug = cms.bool(False)
)

process.p = cms.Path(process.conf_o2o)

