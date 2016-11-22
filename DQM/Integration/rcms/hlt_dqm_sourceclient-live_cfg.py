import FWCore.ParameterSet.Config as cms

process = cms.Process("DQM")
process.load("DQM.Integration.test.inputsource_cfi")

process.load("DQMServices.Core.DQM_cfg")

process.load("DQMServices.Components.DQMEnvironment_cfi")

process.load("DQM.Integration.test.environment_cfi")

process.load("Configuration.StandardSequences.GeometryPilot2_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.GlobalTrackingGeometryESProducer = cms.ESProducer( "GlobalTrackingGeometryESProducer" ) # for muon hlt dqm

process.load( "Configuration.StandardSequences.FrontierConditions_GlobalTag_cff" )
process.GlobalTag.connect = "frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG"
process.GlobalTag.globaltag = "GR09_31X_V3H::All"


process.load("DQM.HLTEvF.HLTMonitor_cff")
process.load("DQM.HLTEvF.HLTMonitorClient_cff")

process.pp = cms.Path(process.dqmEnv+process.dqmSaver)
process.EventStreamHttpReader.consumerName = 'HLT DQM Consumer'
process.dqmEnv.subSystemFolder = 'HLT'
#process.hltResults.plotAll = True

