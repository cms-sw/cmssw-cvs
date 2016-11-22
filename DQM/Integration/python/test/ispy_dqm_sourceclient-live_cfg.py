import FWCore.ParameterSet.Config as cms
import datetime as dt
process = cms.Process("IGUANA")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR09_31X_V4H::All'
process.GlobalTag.connect = "frontier://(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"

process.load("DQM.Integration.test.inputsource_cfi")
process.EventStreamHttpReader.consumerName = 'iSpy Event Display'
process.EventStreamHttpReader.maxEventRequestRate = cms.untracked.double(0.5)

from FWCore.MessageLogger.MessageLogger_cfi import *

process.add_(
    cms.Service("IguanaService",
    outputFileName = cms.untracked.string('/home/dqmprolocal/output/iSpy_%d_%s.ig' % (int(dt.date.today().strftime("%W"))+1,dt.date.today().strftime("%Y%m%d"))),
    outputESFileName=cms.untracked.string('/tmp/iSpy_ES.ig'),
    bufferSize = cms.untracked.uint32(100),
    outputHost = cms.untracked.string('localhost'),
    outputPort = cms.untracked.uint32(9000),
    outputMaxEvents = cms.untracked.int32(100),
    online = cms.untracked.bool(True),
    debug = cms.untracked.bool(False)
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load("VisReco.Analyzer.VisEventSetup_cfi")
process.load("VisReco.Analyzer.VisEvent_cfi")
process.load("VisReco.Analyzer.VisBasicCluster_cfi")
process.load("VisReco.Analyzer.VisCSCSegment_cfi")
process.load("VisReco.Analyzer.VisCSCStripDigi_cfi")
process.load("VisReco.Analyzer.VisCSCWireDigi_cfi")
process.load("VisReco.Analyzer.VisCaloTower_cfi")
process.load('VisReco.Analyzer.VisDTDigi_cfi')
process.load('VisReco.Analyzer.VisDTRecHit_cfi')
process.load("VisReco.Analyzer.VisDTRecSegment4D_cfi")
process.load("VisReco.Analyzer.VisEBRecHit_cfi")
process.load("VisReco.Analyzer.VisEERecHit_cfi")
process.load("VisReco.Analyzer.VisESRecHit_cfi")
#process.load("VisReco.Analyzer.VisEcalRecHit_cfi")
process.load("VisReco.Analyzer.VisHBRecHit_cfi")
process.load("VisReco.Analyzer.VisHERecHit_cfi")
process.load("VisReco.Analyzer.VisHFRecHit_cfi")
process.load("VisReco.Analyzer.VisHORecHit_cfi")
process.load("VisReco.Analyzer.VisJet_cfi")
process.load("VisReco.Analyzer.VisL1GlobalTriggerReadoutRecord_cfi")
process.load("VisReco.Analyzer.VisMET_cfi")
process.load("VisReco.Analyzer.VisMuon_cfi")
process.load("VisReco.Analyzer.VisPixelDigi_cfi")
process.load('VisReco.Analyzer.VisRPCRecHit_cfi')
process.load("VisReco.Analyzer.VisSiPixelCluster_cfi")
process.load("VisReco.Analyzer.VisSiPixelRecHit_cfi")
#process.load("VisReco.Analyzer.VisSiStripCluster_cfi")
process.load("VisReco.Analyzer.VisSiStripDigi_cfi")
process.load("VisReco.Analyzer.VisTrack_cfi")
process.load("VisReco.Analyzer.VisTrackingRecHit_cfi")
process.load("VisReco.Analyzer.VisTriggerEvent_cfi")

process.VisCSCSegment.visCSCSegmentTag = cms.InputTag("cscSegments")
process.VisCSCSegment.VisCSCStripDigi = cms.InputTag("muonCSCDigis:MuonCSCStripDigi")
process.VisCSCSegment.VisCSCWireDigi = cms.InputTag("muonCSCDigis:MuonCSCWireDigi")
process.VisDTRecHit.visDTRecHitTag = cms.InputTag("dt1DRecHits")
process.VisRPCRecHit.visRPCRecHitTag = cms.InputTag("rpcRecHits")
process.VisMET.visMETTag = cms.InputTag('genMetIC5GenJets')
process.VisMuon.visMuonTag = cms.InputTag('muons')
#process.VisSiStripDigi.visSiStripDigiTag = cms.InputTag('siStripDigis:ZeroSuppressed')
process.VisTrack.visTrackTag = cms.InputTag('cosmicMuons')
process.VisTrackingRecHit.visTrackingRecHitTag = cms.InputTag('cosmicMuons')
process.VisTrack.visTrackTags = cms.VInputTag(cms.InputTag('cosmicMuons'),cms.InputTag('cosmictrackfinderP5'),cms.InputTag('ctfWithMaterialTracksP5'))
process.vis = cms.Path(process.VisEvent*
                       process.VisEventSetup*
                       process.VisBasicCluster*
                       process.VisCSCSegment*
                       process.VisCSCStripDigi*
                       process.VisCSCWireDigi*
                       process.VisCaloTower*
                       process.VisTrack*
                       process.VisTrackingRecHit*
                       process.VisDTRecSegment4D*
                       process.VisDTDigi*
                       process.VisDTRecHit*
                       process.VisRPCRecHit*
                       process.VisEBRecHit*
                       process.VisEERecHit*
                       process.VisESRecHit*
                       #process.VisEcalRecHit*
                       process.VisHBRecHit*
                       process.VisHERecHit*
                       process.VisHFRecHit*
                       process.VisHORecHit*
                       process.VisJet*
                       #process.VisMET*
                       #process.VisMuon*
                       process.VisPixelDigi*
                       process.VisSiPixelCluster*
                       process.VisSiPixelRecHit*
                       #process.VisSiStripCluster*
                       process.VisSiStripDigi*
                       process.VisL1GlobalTriggerReadoutRecord*
                       process.VisTriggerEvent)

process.p3= cms.Path(process.RawToDigi)
process.p4= cms.Path(process.reconstructionCosmics)
process.schedule = cms.Schedule(process.p3,process.p4,process.vis)
