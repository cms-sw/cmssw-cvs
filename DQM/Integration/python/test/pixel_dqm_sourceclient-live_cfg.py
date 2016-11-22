import FWCore.ParameterSet.Config as cms

process = cms.Process("PIXELDQMLIVE")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('siPixelDigis', 
                                         #'siPixelClusters', 
                                         'SiPixelRawDataErrorSource', 
                                         'SiPixelDigiSource', 
                                         #'SiPixelClusterSource',
					 'sipixelEDAClient'),
    cout = cms.untracked.PSet(threshold = cms.untracked.string('ERROR')),
    destinations = cms.untracked.vstring('cout')
)

#----------------------------
# Event Source
#-----------------------------
process.load("DQM.Integration.test.inputsource_cfi")
process.EventStreamHttpReader.consumerName = 'Pixel DQM Consumer'
#process.EventStreamHttpReader.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('HLT_TrackerCosmics_CTF'))
process.EventStreamHttpReader.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('*'))

#----------------------------
# DQM Environment
#-----------------------------
process.load("DQMServices.Core.DQM_cfg")
#process.DQMStore.referenceFileName = '/home/dqmprolocal/reference/pixel_reference.root'

process.load("DQMServices.Components.DQMEnvironment_cfi")

#----------------------------
# DQM Live Environment
#-----------------------------
process.load("DQM.Integration.test.environment_cfi")
process.dqmEnv.subSystemFolder    = "Pixel"

#-----------------------------
# Magnetic Field
#-----------------------------
# 0T field
process.load("Configuration.StandardSequences.MagneticField_0T_cff")
# 3.8T field
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.prefer("VolumeBasedMagneticFieldESProducer")

#-------------------------------------------------
# GEOMETRY
#-------------------------------------------------
process.load("Configuration.StandardSequences.Geometry_cff")

#-------------------------------------------------
# GLOBALTAG
#-------------------------------------------------
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.connect = "sqlite_file:/nfshome0/malgeri/public/globtag/CRZT210_V1H.db"
process.GlobalTag.connect ="frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"
process.GlobalTag.globaltag = "GR09_31X_V4H::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

#If Frontier is used in xdaq environment use the following service
#    service = SiteLocalConfigService {}

#-----------------------
#  Reconstruction Modules
#-----------------------
###process.load("Configuration.StandardSequences.Reconstruction_cff")
#process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")
# Real data raw to digi
process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
#process.load("Configuration.StandardSequences.RawToDigi_cff")
process.siPixelDigis.InputLabel = 'source'
process.siPixelDigis.IncludeErrors = True

# Local Reconstruction
###process.load("RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizer_cfi")

#process.load("EventFilter.SiStripRawToDigi.SiStripRawToDigis_standard_cff")
#process.siStripDigis.ProductLabel = 'source'

#process.load("RecoLocalTracker.SiStripClusterizer.SiStripClusterizer_cfi")
#process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi")
#process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitMatcher_cfi")
#process.load("RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTrackAngle_cfi")
#process.load("RecoLocalTracker.SiStripZeroSuppression.SiStripZeroSuppression_cfi")

#process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
#process.load("RecoPixelVertexing.Configuration.RecoPixelVertexing_cff")
##process.load("RecoLocalTracker.Configuration.RecoLocalTracker_Cosmics_cff")
#process.load("RecoTracker.Configuration.RecoTrackerP5_cff")


#--------------------------
# Pixel DQM Source and Client
#--------------------------
process.load("DQM.SiPixelCommon.SiPixelP5DQM_source_cff")
process.load("DQM.SiPixelCommon.SiPixelP5DQM_client_cff")

process.qTester = cms.EDFilter("QualityTester",
    qtList = cms.untracked.FileInPath('DQM/SiPixelMonitorClient/test/sipixel_qualitytest_config.xml'),
    QualityTestPrescaler = cms.untracked.int32(1),
    getQualityTestsFromFile = cms.untracked.bool(True),
    verboseQT = cms.untracked.bool(False)
)

#--------------------------
# Web Service
#--------------------------
process.ModuleWebRegistry = cms.Service("ModuleWebRegistry")
process.AdaptorConfig = cms.Service("AdaptorConfig")

#--------------------------
# Scheduling
#--------------------------
#process.Reco = cms.Sequence(process.siPixelDigis)
#process.Reco = cms.Sequence(process.siPixelDigis*process.siPixelClusters*process.siPixelRecHits)
#process.RecoStrips = cms.Sequence(process.siStripDigis*process.siStripClusters)
#process.siPixelLocalReco = cms.Sequence(process.siPixelRecHits) 
#process.siStripLocalReco = cms.Sequence(process.siStripMatchedRecHits)
#process.trackerLocalReco = cms.Sequence(process.siPixelLocalReco*process.siStripLocalReco)
#process.trackReco = cms.Sequence(process.trackerLocalReco*process.offlineBeamSpot*process.recopixelvertexing*process.tracksP5) #*process.rstracks 
process.DQMmodules = cms.Sequence(process.dqmEnv*process.qTester*process.dqmSaver)

process.p = cms.Path(process.siPixelDigis*process.dqmEnv*process.SiPixelRawDataErrorSource*process.SiPixelDigiSource*process.PixelP5DQMClientWithDataCertification*process.dqmSaver)
#process.p = cms.Path(process.Reco*process.RecoStrips*process.trackReco*process.dqmEnv*process.siPixelP5DQM_cosmics_source*process.qTester*process.PixelP5DQMClientWithDataCertification*process.dqmSaver)
