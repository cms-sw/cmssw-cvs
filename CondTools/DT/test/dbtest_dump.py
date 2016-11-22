import FWCore.ParameterSet.Config as cms

process = cms.Process("DTDBDUMP")
process.source = cms.Source("EmptyIOVSource",
    timetype   = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue  = cms.uint64(1),
    interval   = cms.uint64(1)
)

process.conf_dump = cms.EDFilter("DTConfigDBDump",
    catalog = cms.string('file:testcatalog.xml'),
    contact = cms.string('sqlite_file:testconf.db'),
    token = cms.string('[DB=000000000-0000-0000-0000-000000000000][CNT=DTConfigList][CLID=9CB14BE8-30A2-DB11-9935-000E0C5CE283][TECH=00000B01][OID=00000004-00000000]'),
    #    string token = "[DB=8C6727D8-FB91-DC11-839F-000E0C5CE282][CNT=DTConfigList][CLID=9CB14BE8-30A2-DB11-9935-000E0C5CE283][TECH=00000B01][OID=00000004-00000000]"
    authPath = cms.string('.')
)

process.p = cms.Path(process.conf_dump)

