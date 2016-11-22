# Auto generated configuration file
# using: 
# $Revision: 1.6 $
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
import FWCore.ParameterSet.Config as cms

process = cms.Process('EXPRESS')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
#process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/RawToDigi_Data_cff')
process.load('Configuration/StandardSequences/L1Reco_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('DQMOffline/Configuration/DQMOffline_cff')
process.load('Configuration/StandardSequences/AlCaRecoStreams_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.6 $'),
    annotation = cms.untracked.string('promptReco nevts:-1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'/store/data/BeamCommissioning09/Calo/RAW/v1/000/120/347/FE9E4A7E-90CE-DE11-893E-001D09F2447F.root')
)

# Other statements
process.GlobalTag.globaltag = 'GR09_P_V6::All'

process.fifthCkfTrajectoryFilter.filterPset.minimumNumberOfHits = 2
process.fifthCkfTrajectoryFilter.filterPset.maxLostHits = 4
process.fifthCkfTrajectoryFilter.filterPset.maxConsecLostHits = 2
process.fifthCkfInOutTrajectoryFilter.filterPset.minimumNumberOfHits = 2
process.fifthCkfInOutTrajectoryFilter.filterPset.maxLostHits = 4
process.fifthCkfInOutTrajectoryFilter.filterPset.maxConsecLostHits = 2
process.fifthCkfTrajectoryBuilder.minNrOfHitsForRebuild = 2
process.fifthRKTrajectorySmoother.minHits = 2
process.fifthRKTrajectoryFitter.minHits = 2
process.fifthFittingSmootherWithOutlierRejection.MinNumberOfHits = 2
process.tobtecStepLoose.minNumberLayers = 2
process.tobtecStepLoose.maxNumberLostLayers = 2
process.tobtecStepLoose.dz_par1 = cms.vdouble(10.5, 4.0)
process.tobtecStepLoose.dz_par2 = cms.vdouble(10.5, 4.0)
process.tobtecStepLoose.d0_par1 = cms.vdouble(10.5, 4.0)
process.tobtecStepLoose.d0_par2 = cms.vdouble(10.5, 4.0)
process.tobtecStepLoose.chi2n_par = cms.double(100.0)
process.fifthSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = 100
process.fifthSeeds.RegionFactoryPSet.RegionPSet.originRadius     = 10
process.Chi2MeasurementEstimator.MaxChi2 = 100


# Path and EndPath definitions

process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction_withPixellessTk)
process.dqmoffline_step = cms.Path(process.DQMOffline)

process.pathALCARECOSiStripCalZeroBias = cms.Path(process.seqALCARECOSiStripCalZeroBias*process.ALCARECOSiStripCalZeroBiasDQM)
process.pathALCARECORpcCalHLT = cms.Path(process.seqALCARECORpcCalHLT)
process.pathALCARECOMuAlCalIsolatedMu = cms.Path(process.seqALCARECOMuAlCalIsolatedMu*process.ALCARECOMuAlCalIsolatedMuDQM*process.ALCARECODTCalibrationDQM)
process.pathALCARECOTkAlMinBias = cms.Path(process.seqALCARECOTkAlMinBias*process.ALCARECOTkAlMinBiasDQM)
process.pathALCARECODQM = cms.Path(process.MEtoEDMConverter)
process.endjob_step = cms.Path(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.dqmoffline_step,process.pathALCARECOSiStripCalZeroBias,process.pathALCARECORpcCalHLT,process.pathALCARECOMuAlCalIsolatedMu,process.pathALCARECOTkAlMinBias,process.pathALCARECODQM,process.endjob_step)
