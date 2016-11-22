import FWCore.ParameterSet.Config as cms

from DQM.L1TMonitor.L1TGT_unpack_cff import *
from DQM.L1TMonitor.L1TGCT_unpack_cff import *
from EventFilter.CSCTFRawToDigi.csctfunpacker_cfi import *
from HLTrigger.special.HLTTriggerTypeFilter_cfi import *
hltTriggerTypeFilter.SelectedTriggerType = 1

l1tdttfunpack = cms.EDProducer("DTTFFEDReader",
    DTTF_FED_Source = cms.InputTag("source")
)


l1bxtimingpath = cms.Path(cms.SequencePlaceholder("bxTiming"))
#l1tfedpath = cms.Path(cms.SequencePlaceholder("l1tfed"))
l1tltcpath = cms.Path(cms.SequencePlaceholder("l1tltcunpack")*cms.SequencePlaceholder("l1tltc"))
l1tgtpath = cms.Path(l1GtUnpack*l1GtEvmUnpack*cms.SequencePlaceholder("l1tgt"))
l1trpctfpath = cms.Path(l1GtUnpack*cms.SequencePlaceholder("l1trpctf"))
l1tcsctfpath = cms.Path(csctfunpacker*cms.SequencePlaceholder("l1tcsctf"))
l1tdttpgpath = cms.Path(l1tdttfunpack*cms.SequencePlaceholder("l1tdttf"))
l1tgmtpath = cms.Path(l1GtUnpack*cms.SequencePlaceholder("l1tgmt"))
#l1trctpath = cms.Path(triggerTypeFilter*l1GctHwDigis*cms.SequencePlaceholder("l1trct"))
#l1tgctpath = cms.Path(triggerTypeFilter*l1GctHwDigis*cms.SequencePlaceholder("l1tgct"))
l1trctpath = cms.Path(hltTriggerTypeFilter*l1GctHwDigis*cms.SequencePlaceholder("l1trct"))
l1tgctpath = cms.Path(hltTriggerTypeFilter*l1GctHwDigis*cms.SequencePlaceholder("l1tgct"))

