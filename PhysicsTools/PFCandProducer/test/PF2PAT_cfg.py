# official example for PF2PAT

import FWCore.ParameterSet.Config as cms

process = cms.Process("PF2PAT")


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
)


process.load("PhysicsTools.PFCandProducer.Sources/Data/source_124120_cfi")

#process.load("HiggsAnalysis.VBFHTauTau.poolsource_Z2Jets_noVBFCut_noTAUOLA_cfi")
#process.load("PhysicsTools.PFCandProducer.Sources/source_ZtoEles_DBS_cfi")


# path ---------------------------------------------------------------


process.load("PhysicsTools.PFCandProducer.PF2PAT_cff")

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.p = cms.Path(
    process.PF2PAT 
    )

# output ------------------------------------------------------------

#process.load("FastSimulation.Configuration.EventContent_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.pf2pat = cms.OutputModule("PoolOutputModule",
                                  #    process.AODSIMEventContent,
                                  outputCommands = cms.untracked.vstring('drop *'),
                                  fileName = cms.untracked.string('PF2PAT.root')
)
process.aod = cms.OutputModule("PoolOutputModule",
                               process.AODSIMEventContent,
                               #outputCommands = cms.untracked.vstring('drop *'),
                               fileName = cms.untracked.string('aod.root')
)
process.load("PhysicsTools.PFCandProducer.PF2PAT_EventContent_cff")
process.pf2pat.outputCommands.extend( process.PF2PATEventContent.outputCommands )
process.pf2pat.outputCommands.extend( process.PF2PATStudiesEventContent.outputCommands )


process.outpath = cms.EndPath(
    process.pf2pat 
#    process.aod
    )


# other stuff

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10

# the following are necessary for taus:

process.load("Configuration.StandardSequences.GeometryPilot2_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

# do not forget to set the global tag according to the
# release you are using and the sample you are reading (data or MC)
# global tags can be found here:
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions#Valid_Global_Tags_by_Release
process.GlobalTag.globaltag = cms.string('GR09_R_34X_V2::All')
