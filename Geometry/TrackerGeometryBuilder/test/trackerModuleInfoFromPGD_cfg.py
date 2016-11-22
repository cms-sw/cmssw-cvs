import FWCore.ParameterSet.Config as cms

process = cms.Process("GeometryTest")
# empty input service, fire 10 events
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")

process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    loadAll = cms.bool(True),
    #	string connect = "sqlite_file:test.db"
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('IdealGeometryRecordcd'),
        tag = cms.string('PGeometricDet')
    )),
    catalog = cms.untracked.string('file:testcatalog.xml'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('.')
    )
)

process.print = cms.OutputModule("AsciiOutputModule")

process.prod = cms.EDFilter("ModuleInfo",
    fromDDD = cms.bool(False)
)

process.p1 = cms.Path(process.prod)
process.ep = cms.EndPath(process.print)
process.TrackerGeometricDetESModule.fromDDD = False
process.TrackerDigiGeometryESModule.fromDDD = False


