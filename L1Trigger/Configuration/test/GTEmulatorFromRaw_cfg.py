import FWCore.ParameterSet.Config as cms

process = cms.Process("L1")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerScalesConfig_cff")

process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerPtScaleConfig_cff")

process.load("L1TriggerConfig.L1ScalesProducers.L1MuGMTScalesConfig_cff")

process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")

process.load("L1TriggerConfig.GMTConfigProducers.L1MuGMTParametersConfig_cff")

import EventFilter.GctRawToDigi.l1GctHwDigis_cfi
process.gctDigis = EventFilter.GctRawToDigi.l1GctHwDigis_cfi.l1GctHwDigis.clone()
import EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi
process.gmtDigis = EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi.l1GtUnpack.clone()
process.load("L1Trigger.GlobalTrigger.gtDigis_cfi")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.p = cms.Path(process.gctDigis*process.gmtDigis*process.gtDigis)
process.gctDigis.inputLabel = 'rawDataCollector'
process.gmtDigis.DaqGtInputTag = 'rawDataCollector'
process.PoolSource.fileNames = ['/store/relval/2008/2/5/RelVal-QCD_Pt_80_120-1202115095-HLT/0000/0C84C079-D8D3-DC11-BD81-001617E30E28.root']

