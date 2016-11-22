import FWCore.ParameterSet.Config as cms

process = cms.Process("DTDBINIT")
process.source = cms.Source("EmptyIOVSource",
    timetype   = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue  = cms.uint64(1),
    interval   = cms.uint64(1)
)

process.conf_init = cms.EDFilter("DTConfigDBInit",
    catalog = cms.string('file:testcatalog.xml'),
    contact = cms.string('sqlite_file:testconf.db'),
    name = cms.string('list_conf'),
    authPath = cms.string('.')
)

process.p = cms.Path(process.conf_init)

