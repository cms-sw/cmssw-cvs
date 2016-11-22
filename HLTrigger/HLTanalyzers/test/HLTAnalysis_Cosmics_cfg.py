import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/express/CRAFT09/OfflineMonitor/FEVTHLTALL/v1/000/110/485/E02283E4-BB85-DE11-AB1F-001D09F2423B.root'
#    '/store/express/CRAFT09/OfflineMonitor/FEVTHLTALL/v1/000/110/485/C8E6280E-B985-DE11-A61E-000423D99EEE.root',
#    '/store/express/CRAFT09/OfflineMonitor/FEVTHLTALL/v1/000/110/485/A20C0D0C-B985-DE11-88C2-000423D8FA38.root',
#    '/store/express/CRAFT09/OfflineMonitor/FEVTHLTALL/v1/000/110/485/80FDFD7C-BA85-DE11-B151-00304879FA4C.root',
#    '/store/express/CRAFT09/OfflineMonitor/FEVTHLTALL/v1/000/110/485/6E041F0E-B985-DE11-B379-000423D174FE.root',
#    '/store/express/CRAFT09/OfflineMonitor/FEVTHLTALL/v1/000/110/485/5002A97A-BA85-DE11-B9DA-000423D6CA72.root',
#    '/store/express/CRAFT09/OfflineMonitor/FEVTHLTALL/v1/000/110/485/4E83F70D-B985-DE11-8E12-000423D952C0.root',
#    '/store/express/CRAFT09/OfflineMonitor/FEVTHLTALL/v1/000/110/485/489E1B7E-BA85-DE11-889F-00304879FA4A.root',
#    '/store/express/CRAFT09/OfflineMonitor/FEVTHLTALL/v1/000/110/485/2EFD2F0D-B985-DE11-81FA-000423D98FBC.root',
#    '/store/express/CRAFT09/OfflineMonitor/FEVTHLTALL/v1/000/110/485/1C326AC8-B985-DE11-AED2-000423D987E0.root'
    )
)

process.maxEvents = cms.untracked.PSet(   input = cms.untracked.int32( -1 )   )

process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# Which AlCa condition for what. Available from pre11
# * DESIGN_31X_V1 - no smearing, alignment and calibration constants = 1.  No bad channels.
# * MC_31X_V1 (was IDEAL_31X) - conditions intended for 31X physics MC production: no smearing,
#   alignment and calibration constants = 1.  Bad channels are masked.
# * STARTUP_31X_V1 (was STARTUP_31X) - conditions needed for HLT 8E29 menu studies: As MC_31X_V1 (including bad channels),
#   but with alignment and calibration constants smeared according to knowledge from CRAFT.
# * CRAFT08_31X_V1 (was CRAFT_31X) - conditions for CRAFT08 reprocessing.
# * CRAFT_31X_V1P, CRAFT_31X_V1H - initial conditions for 2009 cosmic data taking - as CRAFT08_31X_V1 but with different
#   tag names to allow append IOV, and DT cabling map corresponding to 2009 configuration (10 FEDs).
# Meanwhile...:
##process.GlobalTag.globaltag = 'MC_31X_V2::All'
process.GlobalTag.globaltag = 'GR09_P_V1::All'

process.load('Configuration/StandardSequences/SimL1Emulator_cff')

# OpenHLT specificss
# Define the HLT reco paths
process.load("HLTrigger.HLTanalyzers.HLTopen_cff")
# Remove the PrescaleService which, in 31X, it is expected once HLT_XXX_cff is imported
del process.PrescaleService

# AlCa OpenHLT specific settings

# Define the analyzer modules
process.load("HLTrigger.HLTanalyzers.HLTAnalyser_cfi")

# Events passing HLT_Random do not store L1 GT information, so 
# add the hltTriggerType filter to select events passing physics triggers.
# This is already added through HLTBeginSequence for the OpenHLT paths.
#
process.analyzeThis = cms.Path( process.hltTriggerType + process.hltanalysis )
##process.analyzeThis = cms.Path( process.hltanalysis )


# pdt
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")



# Schedule the whole thing
process.schedule = cms.Schedule( 
    process.DoHLTJets, 
    process.DoHltMuon, 
    process.DoHLTPhoton, 
##    process.DoHLTElectron, 
    process.DoHLTElectronStartUpWindows, 
    process.DoHLTElectronLargeWindows,
    process.DoHLTElectronSiStrip,
##    process.DoHLTTau, 
    process.DoHLTBTag,
    process.DoHLTAlCaECALPhiSym,
    process.DoHLTAlCaPi0Eta8E29,
##    process.DoHLTIsoTrack,
    process.DoHLTMinBiasPixelTracks,
    process.analyzeThis )

##print process.dumpPython()
