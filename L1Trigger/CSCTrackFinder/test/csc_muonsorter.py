#!/usr/bin/env python
import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
		fileNames = cms.untracked.vstring(
			'/store/relval/CMSSW_3_4_0_pre2/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3XY_V9-v1/0003/F297D9D4-97BD-DE11-8AEF-001731AF6653.root',
			'/store/relval/CMSSW_3_4_0_pre2/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3XY_V9-v1/0003/A4EA7F95-9ABD-DE11-BD83-001A92971ACC.root',
			'/store/relval/CMSSW_3_4_0_pre2/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3XY_V9-v1/0003/7AD839D3-C0BD-DE11-88E9-001731AF65F9.root',
			'/store/relval/CMSSW_3_4_0_pre2/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3XY_V9-v1/0003/6452B3DF-90BD-DE11-AAAC-0030486792AC.root',
			'/store/relval/CMSSW_3_4_0_pre2/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3XY_V9-v1/0003/4A2753A7-96BD-DE11-AC63-001BFCDBD1BC.root',
			'/store/relval/CMSSW_3_4_0_pre2/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3XY_V9-v1/0003/32DF8C96-91BD-DE11-A0F6-003048679168.root',
			'/store/relval/CMSSW_3_4_0_pre2/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3XY_V9-v1/0003/1EA4A325-90BD-DE11-8010-0026189438D3.root',
			'/store/relval/CMSSW_3_4_0_pre2/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3XY_V9-v1/0003/1E13E856-92BD-DE11-8420-00248C55CC3C.root'
	),
	#skipEvents=cms.untracked.uint32(173)
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")

# Event Setup
##############
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.Simulation_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'MC_31X_V10::All'
process.GlobalTag.globaltag = 'GR09_R_35X_V3::All'
# L1 Emulator
#############
process.load("Configuration.StandardSequences.Simulation_cff")
process.load("Configuration.StandardSequences.SimL1Emulator_cff")

# Analysis Module Definition
############################
process.effic = cms.EDAnalyzer("CSCTFEfficiencies",
	OutFile = cms.untracked.string("Validation.root")
)
	
process.FEVT = cms.OutputModule("PoolOutputModule",
	fileName = cms.untracked.string("test.root"),
	outputCommands = cms.untracked.vstring(	
		"keep *"
	)
)
	
# Path Definition
#################
#process.p = cms.Path(process.effic) process.pdigi*
process.p = cms.Path(process.simCscTriggerPrimitiveDigis*process.simDtTriggerPrimitiveDigis*process.simCsctfTrackDigis*process.simCsctfDigis*process.effic)
#process.p = cms.Path(process.simCscTriggerPrimitiveDigis*process.simDtTriggerPrimitiveDigis*process.simCsctfTrackDigis*process.simCsctfDigis)
#process.outpath = cms.EndPath(process.FEVT)
#process.schedule = cms.Schedule(process.p, process.outpath)
