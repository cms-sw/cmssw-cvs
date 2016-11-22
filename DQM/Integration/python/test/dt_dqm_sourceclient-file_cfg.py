import FWCore.ParameterSet.Config as cms

process = cms.Process("DTDQM")

# the source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Commissioning09/Cosmics/RAW/v2/000/102/352/DA29931E-1668-DE11-AC1F-001D09F252DA.root',
    '/store/data/Commissioning09/Cosmics/RAW/v2/000/102/352/CE1A360D-2768-DE11-8E2E-00304879FA4A.root',
    '/store/data/Commissioning09/Cosmics/RAW/v2/000/102/352/CCE0CFDD-1668-DE11-BDCB-000423D6CA02.root',
    '/store/data/Commissioning09/Cosmics/RAW/v2/000/102/352/CAFB386C-1568-DE11-BF34-0030487C6062.root',
    '/store/data/Commissioning09/Cosmics/RAW/v2/000/102/352/BCAB7169-1568-DE11-92AD-0030487A18A4.root',
    '/store/data/Commissioning09/Cosmics/RAW/v2/000/102/352/BA3C5A1E-1668-DE11-AE10-001D09F2532F.root',
    '/store/data/Commissioning09/Cosmics/RAW/v2/000/102/352/A8ECE8E1-1668-DE11-A957-000423D944F8.root',
    '/store/data/Commissioning09/Cosmics/RAW/v2/000/102/352/3E29D81B-1668-DE11-8FFD-001D09F2512C.root',
    '/store/data/Commissioning09/Cosmics/RAW/v2/000/102/352/3238776B-1568-DE11-AD68-0030487C608C.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000000)
    )

process.load("DQMServices.Core.DQM_cfg")

# official DQMCollector
# srv-c2d05-16
process.DQM.collectorHost = 'localhost'
# 9090
process.DQM.collectorPort = 9190

################# BEGIN DQM Online Environment #######################
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.dqmSaver.convention = "Online"
# /cms/mon/data/dropbox
process.dqmSaver.dirName = "."
process.dqmSaver.producer = "DQM"
process.dqmSaver.saveByRun         =  1
process.dqmSaver.saveAtJobEnd      = True

process.dqmEnv.subSystemFolder = "DT"

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("DQM.DTMonitorModule.dt_dqm_sourceclient_common_cff")
#---- for P5 (online) DB access
#process.GlobalTag.connect ="frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_21X_GLOBALTAG"
#process.GlobalTag.globaltag = "CRAFT_V4H::All"
#---- for offline DB
process.GlobalTag.globaltag = "GR09_31X_V2P::All"

# process.load("DQM.DTMonitorModule.dt_standalonedatabases_cfi")
# process.preferTTrigMap = cms.ESPrefer('PoolDBESSource','ttrigsource')


# message logger
process.MessageLogger = cms.Service("MessageLogger",
                                    debugModules = cms.untracked.vstring('*'),
                                    destinations = cms.untracked.vstring('cout'),
                                    categories = cms.untracked.vstring('DTNoiseTask'), 
                                    cout = cms.untracked.PSet(threshold = cms.untracked.string('WARNING'),
                                                              noLineBreaks = cms.untracked.bool(False),
                                                              DEBUG = cms.untracked.PSet(
                                                                      limit = cms.untracked.int32(0)),
                                                              INFO = cms.untracked.PSet(
                                                                      limit = cms.untracked.int32(0)),
                                                              DTNoiseTask = cms.untracked.PSet(
                                                                                 limit = cms.untracked.int32(-1))
                                                              )
                                    )


process.dqmmodules = cms.Sequence(process.dqmEnv + process.dqmSaver)

#---------------------------------------------------------------------------
# define prescale table: three rows (paths), three columns (scenarios)
prescaleTable = cms.VPSet(
    cms.PSet(
      pathName = cms.string('dtDQMPathPhys'),
      prescales = cms.vuint32(10)
    ) 
)


# instantiate prescale service and configure with above defined table
process.load("FWCore.PrescaleService.PrescaleService_cfi")
process.PrescaleService.prescaleTable = prescaleTable
process.PrescaleService.lvl1Labels = cms.vstring('DQMPS')
process.PrescaleService.lvl1DefaultLabel = cms.untracked.string('DQMPS')

process.psDQM = cms.EDFilter("HLTPrescaler")
#---------------------------------------------------------------------------

process.dtDQMPathPhys = cms.Path(process.psDQM * process.unpackers + process.dqmmodules + process.physicsEventsFilter * process.reco + process.dtDQMTask + process.dtDQMTest)

#process.dtDQMPathCalib = cms.Path(process.unpackers + process.dqmmodules + process.calibrationEventsFilter * process.dtDQMCalib)

# f = file('aNewconfigurationFile.cfg', 'w')
# f.write(process.dumpConfig())
# f.close()

process.options = cms.untracked.PSet(
    fileMode = cms.untracked.string('FULLMERGE')
    )


