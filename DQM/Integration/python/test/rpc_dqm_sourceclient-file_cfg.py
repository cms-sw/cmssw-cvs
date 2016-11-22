##########################################
#                                        #
#    RPC DQM config file for RAW Data    #
#    unpack, DIGI, RecHit, RPC_source    #
#              RPC_Client                #
#                                        #
#              CMSSW_3_1_X               #
#                                        #
##########################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("DQM")

####### Geometry #######
process.load("Geometry.MuonCommonData.muonIdealGeometryXML_cfi")
process.load("Geometry.RPCGeometry.rpcGeometry_cfi")
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")

####### BD #######
process.load("CondCore.DBCommon.CondDBSetup_cfi")

####### Muon RecHit #######
process.load("RecoLocalMuon.RPCRecHit.rpcRecHits_cfi")
process.rpcRecHits.rpcDigiLabel = 'rpcunpacker'

####### DQM Central Modules #######
process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

####### RPC Source DQM #######
process.load("DQM.RPCMonitorDigi.RPCDigiMonitoring_cfi")
process.rpcdigidqm.DigiEventsInterval = 100
process.rpcdigidqm.DigiDQMSaveRootFile = True
process.rpcdigidqm.dqmshifter = True
process.rpcdigidqm.dqmexpert = True
process.rpcdigidqm.dqmsuperexpert = True
process.rpcdigidqm.RootFileNameDigi = '/tmp/dlomidze/DQM_3.root'

####### Pool Source #######
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    moduleLogName = cms.untracked.string('source'),
    fileNames = cms.untracked.vstring('file:/tmp/dlomidze/Raw_1.root')
                            )

process.RPCCabling = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('/nfshome0/hltpro/cmssw/cfg/')
        
    ),
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('RPCEMapRcd'),
        tag = cms.string('RPCEMap_v2')
    )),
    connect = cms.string('frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_RPC'),
    siteLocalConfig = cms.untracked.bool(False)
)

####### RAW to Digi #######
process.rpcunpacker = cms.EDFilter("RPCUnpackingModule",
    InputLabel = cms.InputTag("source"),
             doSynchro = cms.bool(True)                       
)

####### DQM Client Modules #######
process.load("DQM.RPCMonitorClient.RPCDqmClient_cfi")
process.rpcdqmclient.RPCDqmClientList = cms.untracked.vstring("RPCNoisyStripTest","RPCOccupancyTest","RPCClusterSizeTest","RPCDeadChannelTest","RPCMultiplicityTest")
process.rpcdqmclient.DiagnosticPrescale = cms.untracked.int32(5)
process.rpcdqmclient.NumberOfEndcapDisks  = cms.untracked.int32(3)

####### RPC Event Summary Module #######
process.load("DQM.RPCMonitorClient.RPCEventSummary_cfi")
process.rpcEventSummary.EventInfoPath = 'RPC/EventInfo'
process.rpcEventSummary.PrescaleFactor = 5

####### Quality Tests #######
process.qTesterRPC = cms.EDFilter("QualityTester",
    qtList = cms.untracked.FileInPath('DQM/RPCMonitorClient/test/RPCQualityTests.xml'),
    QualityTestPrescaler = cms.untracked.int32(5)
)

####### Chamber Quality #######
process.rpcChamberQuality = cms.EDAnalyzer("RPCChamberQuality",
                                           MinimumRPCEvents = cms.untracked.int32(100),
                                           PrescaleFactor = cms.untracked.int32(1)
                                           )

####### Other Clients #######
process.load("DQM.RPCMonitorClient.RPCMon_SS_Dbx_Global_cfi")

####### FED #######
process.load("DQM.RPCMonitorClient.RPCMonitorRaw_cfi")
process.load("DQM.RPCMonitorClient.RPCFEDIntegrity_cfi")
process.load("DQM.RPCMonitorClient.RPCMonitorLinkSynchro_cfi")

####### Message Loger #######
process.MessageLogger = cms.Service("MessageLogger",
    detailedInfo = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    critical = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    ),
    debugModules = cms.untracked.vstring('*'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING'),
        WARNING = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        noLineBreaks = cms.untracked.bool(True)
    ),
    destinations = cms.untracked.vstring('detailedInfo', 
        'critical', 
        'cout')
)

####### DQM environment #######
#process.DQM.collectorHost = 'srv-c2d05-19'
process.DQM.collectorPort = 9190
process.DQM.debug = False
process.dqmSaver.dirName = "/tmp/dlomidze/Out_file_cfg.root"
process.dqmSaver.producer = "DQM"
process.dqmEnv.subSystemFolder = 'RPC'


process.rpcDigi = cms.Sequence(process.rpcunpacker*process.rpcRecHits*process.rpcdigidqm*process.rpcAfterPulse)
process.rpcClient = cms.Sequence(process.qTesterRPC*process.rpcdqmclient*process.rpcChamberQuality*process.rpcEventSummary*process.dqmEnv)
process.p = cms.Path(process.rpcDigi*process.rpcClient)

