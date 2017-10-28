#
# cfg file to print the L1 GT trigger menu using L1Trigger_custom 
# options to choose the source of L1 Menu are to be given in L1Trigger_custom
#
# V M Ghete  2008 - 2010 - 2012


import FWCore.ParameterSet.Config as cms

# choose a valid global tag for the release you are using 
#
# for the option "l1MenuSource='globalTag'" and a data global tag, give also
# the run number 
#
# for other options, set useRunNumber = 0
#

# 5_0_0
useGlobalTag='GR_R_50_V10'

#useRunNumber = 180250
useRunNumber = 0

# process
process = cms.Process("L1GtTriggerMenuTest")
process.l1GtTriggerMenuTest = cms.EDAnalyzer("L1GtTriggerMenuTester")

from L1Trigger.Configuration.L1Trigger_custom import customiseL1Menu
process=customiseL1Menu(process)

# number of events and source
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

if useRunNumber != 0 :   
    process.source = cms.Source("EmptyIOVSource",
                        timetype = cms.string('runnumber'),
                        firstValue = cms.uint64(useRunNumber),
                        lastValue = cms.uint64(useRunNumber),
                        interval = cms.uint64(1)
                        )
else :
    process.source = cms.Source("EmptySource")


# load and configure modules via Global Tag
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions

process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = useGlobalTag+'::All'

# path to be run
process.p = cms.Path(process.l1GtTriggerMenuTest)

# services

# Message Logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.debugModules = ['l1GtTriggerMenuTest']
process.MessageLogger.cout = cms.untracked.PSet(
    #INFO = cms.untracked.PSet(
    #    limit = cms.untracked.int32(-1)
    #)#,
    threshold = cms.untracked.string('ERROR'), ## DEBUG 
    
    ERROR = cms.untracked.PSet( ## DEBUG, all messages  

        limit = cms.untracked.int32(-1)
    )
)


