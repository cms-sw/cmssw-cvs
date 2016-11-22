import FWCore.ParameterSet.Config as cms

process = cms.Process("DQM")
process.load("DQMServices.Core.DQM_cfg")

process.load("DQM.HLTEvF.HLTMonitor_cff")
process.load("DQM.HLTEvF.HLTMonitorClient_cff")


process.load("Configuration.StandardSequences.GeometryPilot2_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load( "Configuration.StandardSequences.FrontierConditions_GlobalTag_cff" )
process.GlobalTag.globaltag = 'GR09_31X_V3P::All'
process.prefer("GlobalTag")

process.GlobalTrackingGeometryESProducer = cms.ESProducer( "GlobalTrackingGeometryESProducer" )

process.load("DQMServices.Components.DQMEnvironment_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:/cms/mon/data/lookarea_SM/GlobalCruzet3.00051488.0001.DQM.storageManager.0.0000.dat')
    #fileNames = cms.untracked.vstring('file:/cms/mon/data/lookarea_SM/GlobalCruzet3.00051437.0001.DQM.storageManager.1.0000.dat')
    #fileNames = cms.untracked.vstring('file:/tmp/wteo/28E1D7F9-214C-DD11-B42C-000423D9880C.root')
#    fileNames = cms.untracked.vstring('/store/data/Commissioning08/Monitor/RAW/v1/000/068/021/000BDAE4-37A6-DD11-8411-001D09F242EF.root')
    fileNames = cms.untracked.vstring('/store/data/Commissioning09/Monitor/RAW/v1/000/082/548/A06226EF-4A30-DE11-A607-000423D94AA8.root')
    #fileNames = cms.untracked.vstring('file:/cms/mon/data/lookarea_SM/GlobalCruzet3.00051218.0001.DQM.storageManager.0.0000.dat')
    #fileNames = cms.untracked.vstring('file:/tmp/wteo/E244612F-7751-DD11-8931-000423D94700.root')
    #fileNames = cms.untracked.vstring('file:/tmp/wteo/001365AC-1C1C-DD11-AA0B-0030487D62E6.root')
    #fileNames = cms.untracked.vstring('file:/tmp/wteo/1CD42767-9B60-DD11-B56E-001617DBD224.root')


)


#process.source = cms.Source("NewEventStreamFileReader",
#    fileNames = cms.untracked.vstring('file:/nfshome0/lorenzo/305FBD08-D78F-DD11-A1D6-001617C3B65A.root')
#   fileNames = cms.untracked.vstring(
#        'file:/cmsdisk1/lookarea_SM/GlobalMW40.00064245.0001.HLTDEBUG.storageManager.01.0000.dat',
#        'file:/cmsdisk1/lookarea_SM/GlobalMW40.00064210.0001.HLTDEBUG.storageManager.03.0000.dat'
#  )
#)


#process.p = cms.EndPath(process.hlts+process.hltsClient)


process.pp = cms.Path(process.dqmEnv+process.dqmSaver)
process.DQMStore.verbose = 0
process.DQM.collectorHost = 'srv-c2d05-12'
process.DQM.collectorPort = 9190
process.dqmSaver.dirName = '.'
process.dqmSaver.producer = 'Playback'
#process.hltResults.plotAll = True
process.dqmSaver.convention = 'Online'
process.dqmEnv.subSystemFolder = 'HLT'
process.dqmSaver.saveByRun = 1
process.dqmSaver.saveAtJobEnd = True

# # Message Logger
# process.load("FWCore.MessageService.MessageLogger_cfi")
# #process.MessageLogger.categories = ['hltResults']
# process.MessageLogger.destinations = ['cout', 'detailedInfo', 'critical']
# process.MessageLogger.cout = cms.untracked.PSet(
#     #threshold = cms.untracked.string('ERROR'),
#     #threshold = cms.untracked.string('INFO'),
#     #INFO = cms.untracked.PSet(
#     # limit = cms.untracked.int32(-1)
#     #)#,
#     threshold = cms.untracked.string('DEBUG'),
#     DEBUG = cms.untracked.PSet(
#     limit = cms.untracked.int32(-1) ## DEBUG, all messages
#     )
#     )
# process.MessageLogger.categories = ['Status', 'Parameter']
# # copy stdout to a file
# process.MessageLogger.detailedInfo = process.MessageLogger.cout
# process.MessageLogger.debugModules = ['hltResults']
# process.MessageLogger.critical = cms.untracked.PSet(
#     threshold = cms.untracked.string('ERROR'),
#     #threshold = cms.untracked.string('INFO'),
#     #INFO = cms.untracked.PSet(
#     # limit = cms.untracked.int32(-1)
#     #)#,
#     #threshold = cms.untracked.string('DEBUG'),
#     ERROR = cms.untracked.PSet(
#     limit = cms.untracked.int32(-1) ## all messages
#     )
#     )
# # summary
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
    )
