import FWCore.ParameterSet.Config as cms

process = cms.Process("EvFDQM")

#----------------------------
#### Event Source
#----------------------------

# the source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Commissioning08/Cosmics/RAW/v1/000/064/768/2E61D6D7-0C95-DD11-B16B-000423D992DC.root',
    '/store/data/Commissioning08/Cosmics/RAW/v1/000/064/768/5A1D18A2-0E95-DD11-8E12-000423D99614.root',
    '/store/data/Commissioning08/Cosmics/RAW/v1/000/064/768/7E96B982-0895-DD11-8E1E-000423D98EC8.root',
    '/store/data/Commissioning08/Cosmics/RAW/v1/000/064/768/88C6F2F2-0B95-DD11-806C-001617E30E28.root',
    '/store/data/Commissioning08/Cosmics/RAW/v1/000/064/768/A2352808-1095-DD11-9B38-000423D98E30.root',
    '/store/data/Commissioning08/Cosmics/RAW/v1/000/064/768/BC7BEFD2-0995-DD11-BC03-001617E30D00.root',
    '/store/data/Commissioning08/Cosmics/RAW/v1/000/064/768/F6F92FA4-0D95-DD11-9D1A-001617E30F48.root',
    '/store/data/Commissioning08/Cosmics/RAW/v1/000/064/768/FE5A3DC4-0D95-DD11-B7D9-000423D99EEE.root',
    '/store/data/CRUZET3/Cosmics/RAW/v1/000/050/781/986D05EF-D44C-DD11-8F6C-000423D985E4.root',
    '/store/data/CRUZET3/Cosmics/RAW/v1/000/050/781/B284117B-CA4C-DD11-B34D-000423D9939C.root',
    '/store/data/CRUZET3/Cosmics/RAW/v1/000/050/781/C0D9C572-CA4C-DD11-AAE5-000423D98804.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )



#----------------------------
#### DQM Environment
#----------------------------

#process.DQMStore.referenceFileName = "DT_reference.root"
process.load("DQMServices.Core.DQM_cfg")
 # official DQMCollector
# srv-c2d05-16
process.DQM.collectorHost = 'localhost'
# 9090
process.DQM.collectorPort = 9190

process.load("DQMServices.Components.DQMEnvironment_cfi")
process.dqmSaver.convention = "Online"
# /cms/mon/data/dropbox
process.dqmSaver.dirName = "."
process.dqmSaver.producer = "Playback"
process.dqmSaver.saveByRun         =  1
process.dqmSaver.saveAtJobEnd      = True


# message logger
process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring('cout'),
                                    cout = cms.untracked.PSet(threshold = cms.untracked.string('WARNING'))
                                    )

# Global tag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')
process.GlobalTag.globaltag = "CRUZET4_V3P::All"


#-----------------------------
#### Sub-system configurations follow

# DT DQM sequence
process.load("DQM.DTMonitorModule.dtDataIntegrityTask_EvF_cff")




# DQM Modules
process.dqmmodules = cms.Sequence(process.dqmEnv + process.dqmSaver)


#-----------------------------
### Define the path
process.evfDQMPath = cms.Path(process.dqmmodules + process.dtDQMEvF)


