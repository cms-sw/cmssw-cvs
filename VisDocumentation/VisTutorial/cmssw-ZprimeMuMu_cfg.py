import FWCore.ParameterSet.Config as cms

process = cms.Process("IGUANA")
process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/data01/yana/tests/CMSSW_2_1_0/src/PYTHIA6_ZPSSMmumu_M3000_Mcut1500_10TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT.root')
)

from FWCore.MessageLogger.MessageLogger_cfi import *

process.load("VisFramework.VisFrameworkBase.VisConfigurationService_cff")
process.VisConfigurationService.VisExceptionMessage = "none"

