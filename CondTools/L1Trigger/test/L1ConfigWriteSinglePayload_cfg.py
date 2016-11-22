# Example using L1RCTParameters

import FWCore.ParameterSet.Config as cms

process = cms.Process("L1ConfigWritePayloadDummy")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cout.placeholder = cms.untracked.bool(False)
process.MessageLogger.cout.threshold = cms.untracked.string('DEBUG')
process.MessageLogger.debugModules = cms.untracked.vstring('*')

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing()
options.register('objectKey',
                 'dummy', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "object key")
options.register('objectType',
                 'L1RCTParameters', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "object C++ type")
options.register('recordName',
                 'L1RCTParametersRcd', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Name of EventSetup record")
options.register('tagBase',
                 'IDEAL', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "IOV tags = object_{tagBase}")
options.register('outputDBConnect',
                 'sqlite_file:l1config.db', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Connection string for output DB")
options.register('outputDBAuth',
                 '.', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Authentication path for outputDB")
options.register('overwriteKey',
                 0, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Overwrite existing key")
options.register('startup',
                 0, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Use L1StartupConfig_cff instead of L1DummyConfig_cff")
options.register('rpcFileDir',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Replacement value of rpcconf.filedir; no replacement by default")
options.register('rpcPACsPerTower',
                 -999, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Replacement value of rpcconf.PACsPerTower; no replacement by default")
options.register('rpcBxOrFirstBx',
                 -999, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Replacement value of l1RPCBxOrConfig.firstBX; no replacement by default")
options.register('rpcBxOrLastBx',
                 -999, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Replacement value of l1RPCBxOrConfig.lastBX; no replacement by default")
options.register('rpcHsb0Mask',
                 [-999], #default value
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.int,
                 "Replacement value of l1RPCHsbConfig.hsb0Mask; no replacement by default")
options.register('rpcHsb1Mask',
                 [-999], #default value
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.int,
                 "Replacement value of l1RPCHsbConfig.hsb1Mask; no replacement by default")
options.parseArguments()

# Generate L1TriggerKey
process.load("CondTools.L1Trigger.L1TriggerKeyDummy_cff")
process.L1TriggerKeyDummy.objectKeys = cms.VPSet(cms.PSet(
    record = cms.string(options.recordName),
    type = cms.string(options.objectType),
    key = cms.string(options.objectKey)
))

# Get L1TriggerKeyList from DB
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.outputDB = cms.ESSource("PoolDBESSource",
    process.CondDBCommon,
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('L1TriggerKeyListRcd'),
        tag = cms.string('L1TriggerKeyList_' + options.tagBase)
    ))
)
process.es_prefer_outputDB = cms.ESPrefer("PoolDBESSource","outputDB")
process.outputDB.connect = cms.string(options.outputDBConnect)
process.outputDB.DBParameters.authenticationPath = cms.untracked.string(options.outputDBAuth)

# Generate dummy configuration data
if options.startup == 0:
    process.load("L1Trigger.Configuration.L1DummyConfig_cff")
    process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.lumi1031.L1Menu_MC2009_v2_L1T_Scales_20090624_Imp0_Unprescaled_cff")
else:
    process.load("L1Trigger.Configuration.L1StartupConfig_cff")
    process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_Commissioning2009_v3_L1T_Scales_20080926_startup_Imp0_Unprescaled_cff")
    
process.l1CSCTFConfig.ptLUT_path = '/afs/cern.ch/cms/MUON/csc/fast1/track_finder/luts/PtLUT.dat'

# Optional customization for RPC objects
if options.rpcFileDir != "":
    process.rpcconf.filedir = options.rpcFileDir
    print "New rpcconf.filedir =", process.rpcconf.filedir
if options.rpcPACsPerTower != -999:
    process.rpcconf.PACsPerTower = options.rpcPACsPerTower
    print "New rpcconf.PACsPerTower =", process.rpcconf.PACsPerTower
if options.rpcBxOrFirstBx != -999:
    process.l1RPCBxOrConfig.firstBX = options.rpcBxOrFirstBx
    print "New l1RPCBxOrConfig.firstBX =", process.l1RPCBxOrConfig.firstBX
if options.rpcBxOrLastBx != -999:
    process.l1RPCBxOrConfig.lastBX = options.rpcBxOrLastBx
    print "New l1RPCBxOrConfig.lastBX =", process.l1RPCBxOrConfig.lastBX
if options.rpcHsb0Mask != [[-999]]:
    process.l1RPCHsbConfig.hsb0Mask = options.rpcHsb0Mask
    print "New l1RPCHsbConfig.hsb0Mask =", process.l1RPCHsbConfig.hsb0Mask
if options.rpcHsb1Mask != [[-999]]:
    process.l1RPCHsbConfig.hsb1Mask = options.rpcHsb1Mask
    print "New l1RPCHsbConfig.hsb1Mask =", process.l1RPCHsbConfig.hsb1Mask

# writer modules
from CondTools.L1Trigger.L1CondDBPayloadWriter_cff import initPayloadWriter
initPayloadWriter( process,
                   outputDBConnect = options.outputDBConnect,
                   outputDBAuth = options.outputDBAuth,
                   tagBase = options.tagBase )
process.L1CondDBPayloadWriter.writeL1TriggerKey = False

if options.overwriteKey == 0:
    process.L1CondDBPayloadWriter.overwriteKeys = False
else:
    process.L1CondDBPayloadWriter.overwriteKeys = True

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource")

process.p = cms.Path(process.L1CondDBPayloadWriter)
