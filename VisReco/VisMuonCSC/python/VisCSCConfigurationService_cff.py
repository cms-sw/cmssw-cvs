import FWCore.ParameterSet.Config as cms

VisCSCConfigurationService = cms.Service('VisCSCConfigurationService',
	VisCSCChamberFlag = cms.untracked.bool(True),
	VisCSCSegmentEnvelopeFlag = cms.untracked.bool(False)
	)
