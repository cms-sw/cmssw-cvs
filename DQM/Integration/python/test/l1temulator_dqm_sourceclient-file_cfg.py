import FWCore.ParameterSet.Config as cms

process = cms.Process("L1TEmuDQMfile")

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

process.load("DQM.L1TMonitor.L1TEmulatorMonitor_cff")    
process.load("DQM.L1TMonitorClient.L1TEMUMonitorClient_cff")    

#include geometry only at top level cfg
process.load("Configuration.StandardSequences.Geometry_cff")

#specify subsystems with qt's to be temporarily masked in summary map
#sequence: dtf,dtp,ctf,ctp,rpc,gmt, etp,htp,rct,gct,gt
#process.l1temuEventInfoClient.maskedSystems = [0,1,1,0,0,0, 1,1,1,0,1]

#off-line
#process.GlobalTag.globaltag = 'CRAFT_V2P::All'
#process.GlobalTag.connect = 'frontier://FrontierProd/CMS_COND_21X_GLOBALTAG'


#online
process.GlobalTag.connect = "frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"   
process.GlobalTag.globaltag = 'GR09_31X_V3H::All' # or any other appropriate
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#process.source = cms.Source("PoolSource",  #"NewEventStreamFileReader",
process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring('file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat',
                                      'file:/lookarea_SM/MWGR_29.00106019.0036.A.storageManager.07.0000.dat')
)

process.DQMStore.verbose = 0
process.DQM.collectorHost = "srv-c2d05-12"
process.DQM.collectorPort = 9190
#process.DQMStore.referenceFileName = "/cmsnfshome0/nfshome0/nuno/DQM_L1TEMU_Ref.root"


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

process.DQMStore.verbose = 0
process.dqmSaver.convention = 'Online'
process.dqmSaver.dirName = '.'
process.dqmSaver.producer = 'DQM'
process.dqmEnv.subSystemFolder = 'L1TEMU'
process.dqmSaver.saveByRun = 1
process.dqmSaver.saveAtJobEnd = True

