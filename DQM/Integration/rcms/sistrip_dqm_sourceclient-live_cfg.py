import FWCore.ParameterSet.Config as cms

process = cms.Process("SiStrpDQMLive")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siStripDigis', 
                                         'siStripClusters', 
                                         'siStripZeroSuppression', 
                                         'SiStripClusterizer'),
    cout = cms.untracked.PSet(threshold = cms.untracked.string('ERROR')),
    destinations = cms.untracked.vstring('cout')
)

#----------------------------
# Event Source
#-----------------------------
process.load("DQM.Integration.test.inputsource_cfi")
process.EventStreamHttpReader.consumerName = 'SiStrip DQM Consumer'
#process.EventStreamHttpReader.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('HLT_TrackerCosmics_CTF'))
process.EventStreamHttpReader.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('*'))


#----------------------------
# DQM Environment
#-----------------------------
process.load("DQMServices.Core.DQM_cfg")
process.DQMStore.referenceFileName = '/home/dqmdevlocal/reference/sistrip_reference.root'
process.DQM.filter = '^(SiStrip|Tracking)(/[^/]+){0,5}$'

process.load("DQMServices.Components.DQMEnvironment_cfi")

#----------------------------
# DQM Live Environment
#-----------------------------
process.load("DQM.Integration.test.environment_cfi")
process.dqmEnv.subSystemFolder    = "SiStrip"
process.dqmSaver.producer = "Playback"
process.dqmSaver.saveByTime = 16
process.dqmSaver.saveByMinute = 16

process.dqmEnvTr = cms.EDFilter("DQMEventInfo",
                 subSystemFolder = cms.untracked.string('Tracking'),
                 eventRateWindow = cms.untracked.double(0.5),
                 eventInfoFolder = cms.untracked.string('EventInfo')
)

#-----------------------------
# Magnetic Field
#-----------------------------
# 0T field
#process.load("Configuration.StandardSequences.MagneticField_0T_cff")
# 3.8T field
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# 4.0T field
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
#process.prefer("VolumeBasedMagneticFieldESProducer")

process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

#-------------------------------------------------
# GEOMETRY
#-------------------------------------------------
process.load("Configuration.StandardSequences.Geometry_cff")

#--------------------------
# Calibration
#--------------------------
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.connect ="frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"
process.GlobalTag.globaltag = "GR09_31X_V4H::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

#-----------------------
#  Reconstruction Modules
#-----------------------
# Real data raw to digi
process.load("EventFilter.SiStripRawToDigi.SiStripDigis_cfi")
process.siStripDigis.ProductLabel = 'source'
process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.siPixelDigis.InputLabel = 'source'

# Local and Track Reconstruction
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_Cosmics_cff")
process.load("RecoTracker.Configuration.RecoTrackerP5_cff")

# offline beam spot
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")

#--------------------------
# Strip DQM Source and Client
#--------------------------
process.load("DQM.SiStripMonitorClient.SiStripSourceConfigP5_cff")
process.load("DQM.SiStripMonitorClient.SiStripClientConfigP5_cff")
process.SiStripAnalyser.TkMapCreationFrequency  = -1
process.SiStripAnalyser.ShiftReportFrequency = -1

#--------------------------
# Quality Test
#--------------------------
process.qTester = cms.EDFilter("QualityTester",
    qtList = cms.untracked.FileInPath('DQM/SiStripMonitorClient/data/sistrip_qualitytest_config.xml'),
    prescaleFactor = cms.untracked.int32(1),                               
    getQualityTestsFromFile = cms.untracked.bool(True)
)

#--------------------------
# Web Service
#--------------------------
process.ModuleWebRegistry = cms.Service("ModuleWebRegistry")

process.AdaptorConfig = cms.Service("AdaptorConfig")

#--------------------------
# Scheduling
#--------------------------
process.SiStripSources = cms.Sequence(process.siStripFEDMonitor*process.SiStripMonitorDigi*process.SiStripMonitorClusterReal*process.SiStripMonitorTrack_ckf*process.MonitorTrackResiduals_ckf*process.TrackMon_ckf)
process.SiStripClients = cms.Sequence(process.SiStripAnalyser*process.TrackEffClient)
process.DQMCommon = cms.Sequence(process.qTester*process.dqmEnv*process.dqmEnvTr*process.dqmSaver)
process.RecoForDQM = cms.Sequence(process.siPixelDigis*process.siStripDigis*process.offlineBeamSpot*process.trackerlocalreco*process.ctftracksP5)
process.p = cms.Path(process.RecoForDQM*process.DQMCommon*process.SiStripSources*process.SiStripClients)


