import FWCore.ParameterSet.Config as cms

process = cms.Process("IGUANA")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.DigiToRaw_cff")
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load('Configuration.StandardSequences.VtxSmearedEarly10TeVCollision_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = 'MC_31X_V5::All'

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('/store/relval/CMSSW_3_2_4/RelValWjet_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V5-v1/0009/FED69290-E483-DE11-A110-000423D98DB4.root')
)

from FWCore.MessageLogger.MessageLogger_cfi import *

process.add_(
    cms.Service("ISpyService",
    outputFileName = cms.untracked.string('RelValWjet_Pt_80_120.ig'),
    online = cms.untracked.bool(False),
    debug = cms.untracked.bool(True)
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.load("ISpy.Analyzers.ISpyCaloTower_cfi")
process.load("ISpy.Analyzers.ISpyCSCSegment_cfi")
process.load('ISpy.Analyzers.ISpyCSCStripDigi_cfi')
process.load('ISpy.Analyzers.ISpyCSCWireDigi_cfi')
process.load("ISpy.Analyzers.ISpyDTRecSegment4D_cfi")
process.load('ISpy.Analyzers.ISpyDTDigi_cfi')
process.load('ISpy.Analyzers.ISpyDTRecHit_cfi')
process.load('ISpy.Analyzers.ISpyRPCRecHit_cfi')
process.load('ISpy.Analyzers.ISpyEBRecHit_cfi')
process.load('ISpy.Analyzers.ISpyEERecHit_cfi')
process.load('ISpy.Analyzers.ISpyESRecHit_cfi')
process.load("ISpy.Analyzers.ISpyEvent_cfi")
process.load("ISpy.Analyzers.ISpyHBRecHit_cfi")
process.load("ISpy.Analyzers.ISpyHERecHit_cfi")
process.load("ISpy.Analyzers.ISpyHFRecHit_cfi")
process.load("ISpy.Analyzers.ISpyHORecHit_cfi")
process.load("ISpy.Analyzers.ISpyJet_cfi")
process.load("ISpy.Analyzers.ISpyMET_cfi")
process.load("ISpy.Analyzers.ISpyMuon_cfi")
process.load("ISpy.Analyzers.ISpyPixelDigi_cfi")
process.load("ISpy.Analyzers.ISpySiPixelCluster_cfi")
process.load("ISpy.Analyzers.ISpySiPixelRecHit_cfi")
process.load("ISpy.Analyzers.ISpySiStripCluster_cfi")
process.load("ISpy.Analyzers.ISpySiStripDigi_cfi")
process.load("ISpy.Analyzers.ISpyTrack_cfi")
process.load("ISpy.Analyzers.ISpyTrackingRecHit_cfi")
process.load('ISpy.Analyzers.ISpyTrackingParticle_cfi')
process.load('ISpy.Analyzers.ISpyTriggerEvent_cfi')
process.load('ISpy.Analyzers.ISpyL1GlobalTriggerReadoutRecord_cfi')
process.load('ISpy.Analyzers.ISpyGsfTrack_cfi')

process.ISpyCSCSegment.iSpyCSCSegmentTag = cms.InputTag("muonCSCSegments")
process.ISpyDTRecHit.iSpyDTRecHitTag = cms.InputTag("dt1DRecHits")
process.ISpyRPCRecHit.iSpyRPCRecHitTag = cms.InputTag("rpcRecHits")
process.ISpyMET.iSpyMETTag = cms.InputTag('genMetIC5GenJets')
process.ISpyMuon.iSpyMuonTag = cms.InputTag('muons')
process.ISpySiStripDigi.iSpySiStripDigiTag = cms.InputTag('siStripDigis:ZeroSuppressed')

process.iSpy = cms.Path(process.ISpyEvent*
                       process.ISpyTrack*
                       process.ISpyTrackingRecHit*
                       process.ISpyCSCSegment*
                       process.ISpyCSCWireDigi*
                       process.ISpyCSCStripDigi*
                       process.ISpyCaloTower*
                       process.ISpyDTRecSegment4D*
                       process.ISpyDTDigi*
                       process.ISpyDTRecHit*
                       process.ISpyRPCRecHit*
                       process.ISpyEBRecHit*
                       process.ISpyEERecHit*
                       process.ISpyESRecHit*
                       process.ISpyHBRecHit*
                       process.ISpyHERecHit*
                       process.ISpyHFRecHit*
                       process.ISpyHORecHit*
                       process.ISpyMET*
                       process.ISpyMuon*
                       process.ISpySiPixelCluster*
                       process.ISpySiPixelRecHit*
                       process.ISpySiStripCluster*
                       process.ISpySiStripDigi*
                       process.ISpyJet*
                       process.ISpyTrackingParticle*
                       process.ISpyTriggerEvent*
                       process.ISpyL1GlobalTriggerReadoutRecord*
                       process.ISpyGsfTrack)

process.p3 = cms.Path(process.RawToDigi)
process.p4 = cms.Path(process.reconstruction)

process.schedule = cms.Schedule(process.p3,process.p4,process.iSpy)
