import FWCore.ParameterSet.Config as cms

process = cms.Process("DQM")
#----------------------------
# Event Source
#-----------------------------
process.load("DQM.Integration.test.inputsource_playback_cfi")

#----------------------------
# DQM Environment
#-----------------------------
process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

#----------------------------
# DQM Playback Environment
#-----------------------------
process.load("DQM.Integration.test.environment_playback_cfi")
process.DQMStore.referenceFileName = '/home/dqmdevlocal/reference/l1t_reference.root'

#-----------------------------
#  DQM SOURCES
#-----------------------------
process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Geometry.MuonCommonData.muonIdealGeometryXML_cfi")

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.connect = "frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_21X_GLOBALTAG"   


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.connect ="frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"   
process.GlobalTag.globaltag = 'GR09_31X_V3H::All' # or any other appropriate
#process.prefer("GlobalTag")
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')


process.load("DQM.L1TMonitor.L1TMonitor_cff")
process.load("DQM.L1TMonitorClient.L1TMonitorClient_cff")
#process.l1GtParameters.BstLengthBytes = 52


##  Available data masks (case insensitive):
##    gt, muons, jets, taujets, isoem, nonisoem, met
process.l1tEventInfoClient.dataMaskedSystems = cms.untracked.vstring("Jets","TauJets","IsoEm","NonIsoEm","MET")

##  Available emulator masks (case insensitive):
##    all, dttf, dttpg, csctf, csctpg, rpc, gmt, ecal, hcal, rct, gct, glt
process.l1tEventInfoClient.emulatorMaskedSystems = cms.untracked.vstring("All")



process.EventStreamHttpReader.consumerName = 'L1T DQM Consumer'
process.dqmEnv.subSystemFolder = 'L1T'

