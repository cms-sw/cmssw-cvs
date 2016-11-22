import FWCore.ParameterSet.Config as cms

process = cms.Process("GR09APVLatencyBuilder")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.infos.placeholder = cms.untracked.bool(False)
process.MessageLogger.infos.threshold = cms.untracked.string("INFO")
process.MessageLogger.infos.default = cms.untracked.PSet(
    limit = cms.untracked.int32(1000000)
    )
process.MessageLogger.infos.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(10000),
    limit = cms.untracked.int32(10000000)
)

process.MessageLogger.cerr.threshold = cms.untracked.string("WARNING")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("EmptySource",
                            firstRun = cms.untracked.uint32(77777),
                            numberEventsInRun = cms.untracked.uint32(1)
                            )

process.PoolDBOutputService = cms.Service('PoolDBOutputService',
                           connect = cms.string("sqlite_file:/afs/cern.ch/user/v/venturia/GlobalRuns/scratch/TEST/latency09_fromDPG.db"),
                           timetype = cms.untracked.string("runnumber"),
#                           BlobStreamerName = cms.untracked.string("TBufferBlobStreamingService"),
                           DBParameters = cms.PSet(
                                                  authenticationPath = cms.untracked.string("/afs/cern.ch/cms/DB/conddb"),
                                                  messageLevel = cms.untracked.int32(2)
                                                  ),
                          toPut = cms.VPSet(cms.PSet(
                                                    record = cms.string("APVLatencyRcd"),
                                                    tag = cms.string("latency_V0")
                                                    )
                                           )
                           )


process.load("DPGAnalysis.SiStripTools.apvlatency.apvlatencybuilder_cfi")
process.apvLatencyBuilder.latencyIOVs = cms.VPSet(
    cms.PSet( runNumber = cms.int32(100966), latency = cms.int32(100)),
    cms.PSet( runNumber = cms.int32(102129), latency = cms.int32(140)),
    cms.PSet( runNumber = cms.int32(105505), latency = cms.int32(142)),
    cms.PSet( runNumber = cms.int32(108449), latency = cms.int32(143))
    )

process.p = cms.Path(process.apvLatencyBuilder)
