import FWCore.ParameterSet.Config as cms

process = cms.Process("EvFDQM")

#----------------------------
#### Event Source
#----------------------------
process.load("DQM.Integration.test.inputsource_cfi")
process.EventStreamHttpReader.consumerName = 'FEDTest DQM Consumer'
process.EventStreamHttpReader.maxEventRequestRate = cms.untracked.double(50.0)



#----------------------------
#### DQM Environment
#----------------------------
process.load("DQMServices.Core.DQM_cfg")
#process.DQMStore.referenceFileName = "DT_reference.root"


#----------------------------
#### DQM Environment
#----------------------------
process.load("DQM.Integration.test.environment_cfi")
process.dqmEnv.subSystemFolder = 'FEDTest'
#-----------------------------


# message logger
process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring('cout'),
                                    cout = cms.untracked.PSet(threshold = cms.untracked.string('WARNING'))
                                    )

# Global tag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.connect = "frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"
process.GlobalTag.globaltag = 'GR09_31X_V4H::All' # or any other appropriate
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')


#-----------------------------
#### Sub-system configuration follows

# CSC DQM sequence
process.load("DQM.CSCMonitorModule.test.csc_hlt_dqm_sourceclient_cfi")

# DT DQM sequence
process.load("DQM.DTMonitorModule.dtDataIntegrityTask_EvF_cff")

# ECAL DQM sequences
process.load("EventFilter.EcalRawToDigiDev.EcalUnpackerMapping_cfi")
process.load("EventFilter.EcalRawToDigiDev.EcalUnpackerData_cfi")
process.load("Geometry.EcalMapping.EcalMapping_cfi")
process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")
import DQM.EcalBarrelMonitorTasks.EBHltTask_cfi
process.ebDQMEvF = DQM.EcalBarrelMonitorTasks.EBHltTask_cfi.ecalBarrelHltTask.clone()
import DQM.EcalEndcapMonitorTasks.EEHltTask_cfi
process.eeDQMEvF = DQM.EcalEndcapMonitorTasks.EEHltTask_cfi.ecalEndcapHltTask.clone()

# L1T DQM sequences 
process.load("DQM.L1TMonitor.L1TFED_cfi")

# Pixel DQM sequences
process.load("Geometry.TrackerSimData.trackerSimGeometryXML_cfi")
process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")
process.load("Configuration.StandardSequences.MagneticField_cff")
# Pixel RawToDigi conversion
process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.siPixelDigis.InputLabel = "source"
process.siPixelDigis.Timing = False
process.siPixelDigis.IncludeErrors = True
process.load("DQM.SiPixelMonitorRawData.SiPixelMonitorHLT_cfi")
process.SiPixelHLTSource.saveFile = False
process.SiPixelHLTSource.slowDown = False

# SiStrip DQM sequences
process.load("DQM.SiStripMonitorHardware.siStripFEDCheck_cfi")

# Hcal DQM sequences
process.load("DQM.HcalMonitorModule.HcalMonitorModule_cfi")
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
# Turn on/off individual hcalMonitor modules ------------
process.hcalMonitor.DataIntegrityTask   = True
process.hcalMonitor.DataFormatMonitor   = False
process.hcalMonitor.DigiMonitor         = False
process.hcalMonitor.RecHitMonitor       = False
process.hcalMonitor.TrigPrimMonitor     = False
process.hcalMonitor.PedestalMonitor     = False
process.hcalMonitor.DeadCellMonitor     = False
process.hcalMonitor.HotCellMonitor      = False
process.hcalMonitor.LEDMonitor          = False
process.hcalMonitor.BeamMonitor         = False

# RPC
#process.RPCCabling = cms.ESSource("PoolDBESSource",
#    DBParameters = cms.PSet(
#        messageLevel = cms.untracked.int32(0),
#        authenticationPath = cms.untracked.string('/nfshome0/hltpro/cmssw/cfg/')
#    ),
#    timetype = cms.string('runnumber'),
#    toGet = cms.VPSet(cms.PSet(
#        record = cms.string('RPCEMapRcd'),
#        tag = cms.string('RPCEMap_v2')
#    )),
##    connect = cms.string('frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG'),
#    connect = cms.string('frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_RPC'),
#    siteLocalConfig = cms.untracked.bool(False)
#)
process.load("EventFilter.RPCRawToDigi.rpcUnpacker_cfi")
process.load("DQM.RPCMonitorClient.RPCFEDIntegrity_cfi")

# ES raw2digi
import EventFilter.ESRawToDigi.esRawToDigi_cfi
process.ecalPreshowerDigis = EventFilter.ESRawToDigi.esRawToDigi_cfi.esRawToDigi.clone()
process.ecalPreshowerDigis.sourceTag = 'source'
process.ecalPreshowerDigis.debugMode = False
from DQM.EcalPreshowerMonitorModule.ESFEDIntegrityTask_cfi import *
process.load("DQM.EcalPreshowerMonitorModule.ESFEDIntegrityTask_cfi")

# FED Integrity Client
process.load("DQMServices.Components.DQMFEDIntegrityClient_cff")
process.dqmFEDIntegrity.moduleName = "FEDTest"

# DQM Modules
process.dqmmodules = cms.Sequence(process.dqmEnv + process.dqmSaver)

#-----------------------------
### Define the path
process.evfDQMPath = cms.Path(process.dqmmodules +
                              process.cscDQMEvF +
 			      process.dtDQMEvF +
 			      process.ecalEBunpacker  + process.ebDQMEvF + process.eeDQMEvF +
			      process.ecalPreshowerDigis + process.ecalPreshowerFEDIntegrityTask +
 			      process.l1tfed +
 			      process.siPixelDigis + process.SiPixelHLTSource +
                              process.siStripFEDCheck + 
			      process.hcalDigis + process.hcalMonitor +
			      process.rpcunpacker + process.rpcFEDIntegrity +
                              process.dqmFEDIntegrityClient 
)
