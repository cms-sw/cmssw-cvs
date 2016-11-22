import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")

# Keep output to a nice level
#process.Timing =  cms.Service("Timing")
process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("detailedInfo.txt")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Maximum number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

#############################################################################
# Put here a file pre-simulated with Fast Simulation, if you wish
#process.source = cms.Source(
#    "PoolSource",
#    fileNames = cms.untracked.vstring(
#    '/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/0EF324BD-9160-DD11-B591-000423D944F8.root' )
#)
#
# Otherwise, generate events on the fly, followed by FastSimulation
# Generate ttbar events
process.load("FastSimulation.Configuration.ttbar_cfi")
process.load("PhysicsTools.HepMCCandAlgos.genEventWeight_cfi")
process.load("PhysicsTools.HepMCCandAlgos.genEventScale_cfi")
# *Note* : you can also read a pre-generated event file
# Add here the pre-generated source
#
# *Note* : you can also read a pre-simulated event file. In this case, skip
#          the simulation, HLT and RECO step.
# Add here the pre-simulated source
#############################################################################


#############################################################################
# FastSimulation Step
# Include the RandomNumberGeneratorService definition
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")

# L1 Menu and prescale factors : useful for testing all L1 paths
# Note: the L1 conditions and menu now come from the GlobalTag !
### process.load("Configuration.StandardSequences.L1TriggerDefaultMenu_cff")

# Common inputs, with frontier conditions
process.load("FastSimulation.Configuration.CommonInputs_cff")
process.GlobalTag.globaltag = "STARTUP_31X::All"
# L1 Emulator and HLT Setup
process.load("FastSimulation.HighLevelTrigger.HLTSetup_cff")
# Famos sequences
process.load("FastSimulation.Configuration.FamosSequences_cff")
# Apply ECAL and HCAL miscalibration 
process.ecalRecHit.doMiscalib = True
process.hbhereco.doMiscalib = True
process.horeco.doMiscalib = True
process.hfreco.doMiscalib = True
# Apply Tracker misalignment
process.famosSimHits.ApplyAlignment = True
process.misalignedTrackerGeometry.applyAlignment = True
# Apply HCAL miscalibration (not ideal in that case).
# Choose between hcalmiscalib_startup.xml , hcalmiscalib_1pb.xml , hcalmiscalib_10pb.xml (startup is the default)
process.hbhereco.RecHitsFactory.HCAL.Refactor = 1.0
process.hbhereco.RecHitsFactory.HCAL.Refactor_mean = 1.0
process.horeco.RecHitsFactory.HCAL.Refactor = 1.0
process.horeco.RecHitsFactory.HCAL.Refactor_mean = 1.0
process.hfreco.RecHitsFactory.HCAL.Refactor = 1.0
process.hfreco.RecHitsFactory.HCAL.Refactor_mean = 1.0
# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True
# HLT paths - defined by configDB
# This one is created on the fly by FastSimulation/Configuration/test/IntegrationTestWithHLT_py.csh
#process.load("FastSimulation.Configuration.HLT_cff")
process.load("HLTrigger.Configuration.HLT_FULL_cff")
# Simulation sequence (with 0 pileup event prem bunch crossing)
process.simulation = cms.Sequence(process.simulationWithFamos+process.genEventScale+process.genEventWeight)
process.famosPileUp.PileUpSimulator.averageNumber = 0.0
# Reconstruction sequence (comment if you don't want reconstruction !)
process.HLTEndSequence = cms.Sequence(process.reconstructionWithFamos)
# The current schedule, with all HLT paths
process.schedule = cms.Schedule()
process.schedule.extend(process.HLTSchedule)
###############################################################################


###############################################################################
# To write out events (to be read later - maybe we need RECO only?) 
# process.load("FastSimulation.Configuration.EventContent_cff")
# process.o1 = cms.OutputModule("PoolOutputModule",
#    process.FEVTSIMEventContent,
#    fileName = cms.untracked.string('FEVTSIMWithHLT.root')
#)
# process.outpath = cms.EndPath(process.o1)
# Add endpaths to the schedule
# process.schedule.append(process.outpath)
###############################################################################


###############################################################################
# OpenHLT specificss
# Can be run on a sample, but can also be run with FastSimulation on the fly.
# Define the HLT reco paths
process.load("FastSimulation.HighLevelTrigger.HLTopen_cff")
# Define the analyzer modules
process.load("FastSimulation.HighLevelTrigger.HLTAnalyser_cfi")
process.analyzeThis = cms.EndPath( process.hltanalysis )
# Schedule the whole thing
#process.schedule = cms.Schedule(
#    process.DoHltMuon,
#    process.DoHLTJets,
#    process.DoHLTPhoton ,
#    process.DoHLTElectron ,
#    process.DoHLTElectronStartUpWindows ,
#    process.DoHLTElectronLargeWindows ,
#    process.DoHLTTau,
#    process.DoHLTBTag
#)
process.schedule.append(process.DoHltMuon) 
process.schedule.append(process.DoHLTJets)
process.schedule.append(process.DoHLTPhoton)
#process.schedule.append(process.DoHLTElectron)
process.schedule.append(process.DoHLTElectronStartUpWindows)
process.schedule.append(process.DoHLTElectronLargeWindows)
process.schedule.append(process.DoHLTTau)
process.schedule.append(process.DoHLTBTag)
process.schedule.append(process.analyzeThis)
###############################################################################
