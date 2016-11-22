import FWCore.ParameterSet.Config as cms

from FWCore.MessageLogger.MessageLogger_cfi import *
process = cms.Process("IGUANA")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.VisConfigurationService = cms.Service("VisConfigurationService",
                                              Views = cms.untracked.vstring('3D Window'),
                                              ContentProxies = cms.untracked.vstring('Reco/CMS Magnetic Field')
                                              )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
    )
process.source = cms.Source("EmptySource")
