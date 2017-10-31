import FWCore.ParameterSet.Config as cms

negativeTrackCountingHighPurJetTags = cms.EDProducer("JetTagProducer",
     jetTagComputer = cms.string('negativeTrackCounting3D3rd'),
     tagInfos =  cms.VInputTag(cms.InputTag("impactParameterTagInfos")),
     trackQualityClass = cms.string ( "any" )
)
