#
# cfg file to re-run L1 Trigger emulator on a raw data file 
#
# V M Ghete 2009-02-27

import FWCore.ParameterSet.Config as cms

# process
process = cms.Process('L1')

###################### user choices ######################

# choose the type of sample used (True for RelVal, False for data)
useRelValSample = True 
#useRelValSample=False 

# choose the global tag type for RelVal
useGlobalTag = 'IDEAL'
#useGlobalTag='STARTUP'

# explicit choice of the L1 menu - available choices:
l1Menu = 'L1Menu_Commissioning2009_v0'
#l1Menu = 'L1Menu_MC2009_v0'
#l1Menu = 'L1Menu_startup2_v4'
#l1Menu = 'L1Menu_2008MC_2E30'
#l1Menu = 'myMenu'


###################### end user choices ###################


# number of events to be processed and source file
process.maxEvents = cms.untracked.PSet(
    input=cms.untracked.int32(50)
)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ('PoolSource', fileNames=readFiles, secondaryFileNames=secFiles)

# type of sample used (True for RelVal, False for data)

if useRelValSample == True :
    if useGlobalTag == 'IDEAL':

        #/RelValTTbar/CMSSW_2_2_4_IDEAL_V11_v1/GEN-SIM-DIGI-RAW-HLTDEBUG
        dataset = cms.untracked.vstring('RelValTTbar_CMSSW_2_2_4_IDEAL_V11_v1')
        
        readFiles.extend([
            '/store/relval/CMSSW_2_2_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V11_v1/0000/02697009-5CF3-DD11-A862-001D09F2423B.root',
            '/store/relval/CMSSW_2_2_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V11_v1/0000/064657A8-59F3-DD11-ACA5-000423D991F0.root',
            '/store/relval/CMSSW_2_2_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V11_v1/0000/0817F6DE-5BF3-DD11-880D-0019DB29C5FC.root',
            '/store/relval/CMSSW_2_2_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/IDEAL_V11_v1/0000/0899697C-5AF3-DD11-9D21-001617DBD472.root'
            ]);


        secFiles.extend([
            ])

    elif useGlobalTag == 'STARTUP':

        #/RelValTTbar/CMSSW_2_2_4_STARTUP_V8_v1/GEN-SIM-DIGI-RAW-HLTDEBUG
        dataset = cms.untracked.vstring('RelValTTbar_CMSSW_2_2_4_STARTUP_V8_v1')
        
        readFiles.extend([
            '/store/relval/CMSSW_2_2_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V8_v1/0000/069AA022-5BF3-DD11-9A56-001617E30D12.root',
            '/store/relval/CMSSW_2_2_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V8_v1/0000/08DA99A6-5AF3-DD11-AAC1-001D09F24493.root',
            '/store/relval/CMSSW_2_2_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V8_v1/0000/0A725E15-5BF3-DD11-8B4B-000423D99CEE.root',
            '/store/relval/CMSSW_2_2_4/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP_V8_v1/0000/0AF5B676-5AF3-DD11-A22F-001617DBCF1E.root'
            ]);


        secFiles.extend([
            ])
    else :
        print 'Error: Global Tag ', useGlobalTag, ' not defined.'    

else : 

    # CRAFT data FIXME
    dataset = ''
    
    readFiles.extend([
        ]);

    secFiles.extend([
        ])


# add explicitly the ECAL, HCAL, CSC, DT, RPC unpackers (not the RawToDigi_cff)
#process.load('Configuration.StandardSequences.RawToDigi_cff')

#
import EventFilter.EcalRawToDigiDev.EcalUnpackerData_cfi
process.ecalDigis = EventFilter.EcalRawToDigiDev.EcalUnpackerData_cfi.ecalEBunpacker.clone()
process.ecalDigis.DoRegional = False
process.ecalDigis.InputLabel = 'rawDataCollector'

#
import EventFilter.HcalRawToDigi.HcalRawToDigi_cfi
process.hcalDigis = EventFilter.HcalRawToDigi.HcalRawToDigi_cfi.hcalDigis.clone()
process.hcalDigis.InputLabel = 'rawDataCollector'

#
import EventFilter.CSCRawToDigi.cscUnpacker_cfi
process.muonCSCDigis = EventFilter.CSCRawToDigi.cscUnpacker_cfi.muonCSCDigis.clone()
process.muonCSCDigis.InputObjects = 'rawDataCollector'

#
import EventFilter.DTRawToDigi.dtunpacker_cfi
process.muonDTDigis = EventFilter.DTRawToDigi.dtunpacker_cfi.muonDTDigis.clone()
process.muonDTDigis.inputLabel = 'rawDataCollector'

#
import EventFilter.RPCRawToDigi.rpcUnpacker_cfi
process.muonRPCDigis = EventFilter.RPCRawToDigi.rpcUnpacker_cfi.rpcunpacker.clone()
process.muonRPCDigis.InputLabel = 'rawDataCollector'


# run trigger primitive generation on unpacked digis, then central L1
process.load('L1Trigger.Configuration.CaloTriggerPrimitives_cff')
process.load('L1Trigger.Configuration.SimL1Emulator_cff')

# set the new input tags after RawToDigi for the TPG producers
process.simEcalTriggerPrimitiveDigis.Label = 'ecalDigis'
process.simHcalTriggerPrimitiveDigis.inputLabel = cms.VInputTag(cms.InputTag('hcalDigis'), 
                                                                cms.InputTag('hcalDigis'))
#
process.simDtTriggerPrimitiveDigis.digiTag = 'muonDTDigis'
#
process.simCscTriggerPrimitiveDigis.CSCComparatorDigiProducer = cms.InputTag('muonCSCDigis',
                                                                             'MuonCSCComparatorDigi')
process.simCscTriggerPrimitiveDigis.CSCWireDigiProducer = cms.InputTag('muonCSCDigis',
                                                                       'MuonCSCWireDigi')
#
process.simRpcTriggerDigis.label = 'muonRPCDigis'


# load and configure modules via Global Tag
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions

# import of standard configurations

process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')



if useRelValSample == True :
    if useGlobalTag == 'IDEAL':
        process.GlobalTag.globaltag = 'IDEAL_30X::All'

    elif useGlobalTag == 'STARTUP':
        process.GlobalTag.globaltag = 'STARTUP_30X::All'

    else :
        print 'Error: Global Tag ', useGlobalTag, ' not defined.'    

else :
    process.GlobalTag.globaltag = 'CRAFT_30X::All'


# explicit choice of the L1 menu

if l1Menu == 'L1Menu_Commissioning2009_v0' :
    process.load('L1Trigger.Configuration.L1StartupConfig_cff')
    process.load('L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_Commissioning2009_v0_L1T_Scales_20080926_startup_Imp0_Unprescaled_cff')
    
elif l1Menu == 'L1Menu_startup2_v4' :
    process.load('L1Trigger.Configuration.L1StartupConfig_cff')
    process.load('L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup2_v4_L1T_Scales_20080926_startup_Imp0_Unprescaled_cff')
    
elif l1Menu == 'L1Menu_MC2009_v0' :
    process.load('L1TriggerConfig.L1GtConfigProducers.Luminosity.lumi1031.L1Menu_MC2009_v0_L1T_Scales_20080922_Imp0_Unprescaled_cff')

elif l1Menu == 'L1Menu_2008MC_2E30' :
    process.load('L1TriggerConfig.L1GtConfigProducers.Luminosity.lumi1030.L1Menu_2008MC_2E30_Unprescaled_cff')

elif l1Menu == 'myMenu' :
    #process.load('L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup_v3_Unprescaled_cff')
    #process.load('L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup_v4_Unprescaled_cff')
    #process.load('L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup_v5_Unprescaled_cff')
    #process.load('L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup2_v1_Unprescaled_cff')
    #process.load('L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup2_v2_Unprescaled_cff')
    process.load('L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup2_v3_Unprescaled_cff')
else :
    print 'No such L1 menu: ', l1Menu  


process.p = cms.Path(
    process.ecalDigis * process.hcalDigis 
    * process.CaloTriggerPrimitives 
    * process.muonDTDigis 
    * process.muonCSCDigis 
    * process.muonRPCDigis 
    * process.SimL1Emulator
)

# Message Logger
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules = ['*']
process.MessageLogger.categories = ['*']
process.MessageLogger.destinations = ['cout']
process.MessageLogger.cout = cms.untracked.PSet(
    #threshold=cms.untracked.string('DEBUG'),
    #threshold = cms.untracked.string('INFO'),
    threshold = cms.untracked.string('ERROR'),
    DEBUG=cms.untracked.PSet(
        limit=cms.untracked.int32(-1)
    ),
    INFO=cms.untracked.PSet(
        limit=cms.untracked.int32(-1)
    ),
    WARNING=cms.untracked.PSet(
        limit=cms.untracked.int32(-1)
    ),
    ERROR=cms.untracked.PSet(
        limit=cms.untracked.int32(-1)
    )
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    fileName = cms.untracked.string('L1EmulatorFromRaw.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string("\'GEN-SIM-DIGI-RAW-HLTDEBUG\'"),
        filterName = cms.untracked.string('')
    )
)

process.out_step = cms.EndPath(process.output)
