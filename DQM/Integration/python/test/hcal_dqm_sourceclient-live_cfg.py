import FWCore.ParameterSet.Config as cms
from DQM.HcalMonitorModule.HcalMonitorModule_cfi import * # Can this be done better?
from DQM.HcalMonitorClient.HcalMonitorClient_cfi import * 

process = cms.Process("HCALDQM")
#----------------------------
# Event Source
#-----------------------------
process.load("DQM.Integration.test.inputsource_cfi")
process.EventStreamHttpReader.consumerName = 'Hcal DQM Consumer'

# process.maxEvents = cms.untracked.PSet(
#     input = cms.untracked.int32(-1)
#     )
# 
# process.source = cms.Source("EventStreamHttpReader",
#                             sourceURL = cms.string('http://cmsmon:50082/urn:xdaq-application:lid=29'),
#                             consumerPriority = cms.untracked.string('normal'),
#                             max_event_size = cms.int32(7000000),
#                             consumerName = cms.untracked.string('DQM Source'),
#                             max_queue_depth = cms.int32(5),
#                             maxEventRequestRate = cms.untracked.double(15.0),
#                             SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('*DQM')
#                                                               ),
#                             headerRetryInterval = cms.untracked.int32(3)
#                             )

#----------------------------
# DQM Environment
#-----------------------------
process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
#process.DQMStore.referenceFileName = '/home/dqmdevlocal/reference/hcal_reference.root'

#----------------------------
# DQM Live Environment
#-----------------------------
process.load("DQM.Integration.test.environment_cfi")
process.dqmEnv.subSystemFolder = "Hcal"

# This one ?  process.load("DQM.Integration.test.environment_cfi")


#-----------------------------
# Hcal Conditions: from Global Conditions Tag 
#-----------------------------
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.connect = "frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"
process.GlobalTag.globaltag = 'GR09_31X_V4H::All' # or any other appropriate
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

process.load("FWCore.MessageLogger.MessageLogger_cfi")


#-----------------------------
# Hcal DQM Source, including SimpleReconstrctor
#-----------------------------
process.load("DQM.HcalMonitorModule.HcalMonitorModule_cfi")
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("RecoLocalCalo.HcalRecProducers.HcalSimpleReconstructor_hbhe_cfi")
process.load("RecoLocalCalo.HcalRecProducers.HcalSimpleReconstructor_ho_cfi")
process.load("RecoLocalCalo.HcalRecProducers.HcalSimpleReconstructor_hf_cfi")
process.load("RecoLocalCalo.HcalRecProducers.HcalSimpleReconstructor_zdc_cfi")

# hcalMonitor configurable values -----------------------
process.hcalMonitor.debug = 0
#process.hcalMonitor.DigiOccThresh = -999999999 ##Temporary measure while DigiOcc is reworked.
process.hcalMonitor.pedestalsInFC = True
process.hcalMonitor.showTiming = False
process.hcalMonitor.checkNevents=1000
process.hcalMonitor.dump2database = False

# Turn on/off individual hcalMonitor modules ------------
process.hcalMonitor.DataFormatMonitor   = True
process.hcalMonitor.DataIntegrityTask   = False
process.hcalMonitor.DigiMonitor         = True
process.hcalMonitor.RecHitMonitor       = True
process.hcalMonitor.TrigPrimMonitor     = False
process.hcalMonitor.DeadCellMonitor     = True
process.hcalMonitor.HotCellMonitor      = True
process.hcalMonitor.BeamMonitor         = True
process.hcalMonitor.PedestalMonitor     = True
process.hcalMonitor.LEDMonitor          = False
process.hcalMonitor.CaloTowerMonitor    = False
process.hcalMonitor.MTCCMonitor         = False
process.hcalMonitor.HcalAnalysis        = False

# This takes the default cfg values from the hcalMonitor base class and applies them to the subtasks.
setHcalTaskValues(process.hcalMonitor)

# Set individual Task values here (otherwise they will remain set to the values specified for the hcalMonitor.)

process.hcalMonitor.HotCellMonitor_makeDiagnosticPlots  = False
process.hcalMonitor.HotCellMonitor_test_neighbor        = False

#-----------------------------
# Hcal DQM Client
#-----------------------------
process.load("DQM.HcalMonitorClient.HcalMonitorClient_cfi")

# hcalClient configurable values ------------------------
# suppresses html output from HCalClient  
process.hcalClient.baseHtmlDir = ''  # set to '' to prevent html output

# Set client settings to the same as monitor.  At the moment, this doesn't affect client minErrorFlag
# Summary Client is also unaffected
setHcalClientValuesFromMonitor(process.hcalClient,process.hcalMonitor, debug=False)  # turn debug to True to dump out client settings

process.hcalClient.SummaryClient        = True
#process.hcalClient.HotCellClient        = False
#process.hcalClient.DeadCellClient       = False
#process.hcalClient.PedestalClient       = False


#-----------------------------
# Scheduling
#-----------------------------
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound', 
        'TooManyProducts', 
        'TooFewProducts')
)

process.p = cms.Path(process.hcalDigis
                     *process.horeco
                     *process.hfreco
                     *process.hbhereco
                     *process.zdcreco
                     *process.hcalMonitor
                     *process.hcalClient
                     *process.dqmEnv
                     *process.dqmSaver)


#-----------------------------
# Quality Tester 
# will add switch to select histograms to be saved soon
#-----------------------------
process.qTester = cms.EDFilter("QualityTester",
    prescaleFactor = cms.untracked.int32(1),
    qtList = cms.untracked.FileInPath('DQM/HcalMonitorClient/data/hcal_qualitytest_config.xml'),
    getQualityTestsFromFile = cms.untracked.bool(True)
)

