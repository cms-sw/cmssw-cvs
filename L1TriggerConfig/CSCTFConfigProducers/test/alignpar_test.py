import FWCore.ParameterSet.Config as cms

process = cms.Process("L1ConfigWritePayloadDummy")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cout.placeholder = cms.untracked.bool(False)
process.MessageLogger.cout.threshold = cms.untracked.string('INFO')
process.MessageLogger.debugModules = cms.untracked.vstring('*')

# Generate dummy L1TriggerKeyList
process.load("CondTools.L1Trigger.L1TriggerKeyListDummy_cff")


# Get configuration data from OMDS.  This is the subclass of =L1ConfigOnlineProdBase=.
process.load("L1TriggerConfig.CSCTFConfigProducers.CSCTFAlignmentOnline_cfi")

## For a known object key (MyObjectKey): 
process.load("CondTools.L1Trigger.L1TriggerKeyDummy_cff")
process.L1TriggerKeyDummy.objectKeys = cms.VPSet(cms.PSet(
        record = cms.string('L1MuCSCTFAlignmentRcd'),
        type = cms.string('L1MuCSCTFAlignment'),
        key = cms.string('999')
))


# # Subclass of L1ObjectKeysOnlineProdBase.
# process.load("L1TriggerConfig.CSCTFConfigProducers.CSCTFObjectKeysOnline_cfi")
# process.CSCTFObjectKeysOnline.subsystemLabel = cms.string('')

# process.load("L1TriggerConfig.CSCTFConfigProducers.L1CSCTFConfig_cff")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)

process.getter = cms.EDAnalyzer("EventSetupRecordDataGetter",
   toGet = cms.VPSet(cms.PSet(
   record = cms.string('L1MuCSCTFAlignmentRcd'),
   data = cms.vstring('L1MuCSCTFAlignment')
   )),
   verbose = cms.untracked.bool(True)
)

process.p = cms.Path(process.getter)

# process.myanalyzer = cms.EDAnalyzer("CSCTFConfigTestAnalyzer")
# process.p = cms.Path(process.myanalyzer)

