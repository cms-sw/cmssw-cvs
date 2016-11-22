#! /usr/bin/env python

###################################################################################################
###################################################################################################

# User Parameters 
# ------------------------------------------------------------------------------------------------ #

# Relval release (set if different from $CMSSW_VERSION)
NewRelease = 'CMSSW_3_4_0_pre7'

# ReferenceRelease
RefRelease = 'CMSSW_3_4_0_pre5'

#--------------------------------------------------------
# Startup standard relval samples
startupsamples = [
    # no pileup RelValsamples
    # ---------------------------#
    'RelValTTbar',
    'RelValMinBias',
    'RelValQCD_Pt_3000_3500'

    # pileup RelVal samples
    # ---------------------------#
    #'RelValTTbar_Tauola'
]

#--------------------------------------------------------
#This are the standard relvals (ideal).  uncomment desired channels

idealsamples = [
    # no pileup RelVal samples
    # ---------------------------#
    'RelValSingleMuPt1',
    'RelValSingleMuPt10',
    'RelValSingleMuPt100',
    'RelValSinglePiPt1',
    'RelValSinglePiPt10',
    'RelValSinglePiPt100',
    'RelValSingleElectronPt35',
    'RelValTTbar',
    'RelValQCD_Pt_3000_3500',
    'RelValMinBias'

    #This is pileup RelVal samples
    # ---------------------------#
    #'RelValZmumuJets_Pt_20_300_GEN'

    # summer09 preproduction (the character '-' must be avoided)
    # ---------------------------#
    #'InclusiveMu5_Pt250__Summer09', 
    #'InclusiveMu5_Pt50__Summer09', 
    #'MinBias_herwig__Summer09', 
    #'TTbar__Summer09'
]


#--------------------------------------------------------
numEventsMap = {
    'relValTTbar': -1
}


#--------------------------------------------------------
# track algorithm name and quality. Can be a list.
Algos = [
    'ootb',
    'iter0',
    'iter1',
    'iter2',
    'iter3',
    'iter4',
    'iter5'
]

Qualities=[
    '',
    'highPurity'
]

#--------------------------------------------------------
# name of the track collecion
TrackCollection = 'generalTracks'

#--------------------------------------------------------
#Reference and new repository
RefRepository = '/afs/cern.ch/cms/performance/tracker/activities/reconstruction/tracking_performance'
#NewRepository = '/afs/cern.ch/cms/performance/tracker/activities/reconstruction/tracking_performance'
NewRepository = 'new'

# Ideal and Statup tags
IdealTag   = 'MC_3XY_V14'
StartupTag = 'STARTUP3X_V14'
#IdealTag   = 'MC_3XY_V12'
#StartupTag = 'STARTUP3X_V11'

# Ideal and Statup tags (if blank uses the new release version)
RefIdealTag   = 'MC_3XY_V12'
RefStartupTag = 'STARTUP3X_V11'

# Dataset Version ('', 'v1', 'v2', etc..)
NewVersion = 'v1'
RefVersion = 'v1'

# PileUp: PU . No PileUp: noPU
PileUp='noPU'

# to append the filename  
NewSelectionLabel=''
RefSelectionLabel=''
#RefSelectionLabel='gcc345'
#NewSelectionLabel='gcc432'
#RefSelectionLabel='SLC4gcc345'
#NewSelectionLabel='SLC5gcc434'


#--------------------------------------------------------
# Sequence. Possible values:
#   -only_validation   -- not implemented yet
#   -re_tracking   -- not implemented yet
#   -digi2track   -- not implemented yet
#   -only_validation_and_TP   -- not implemented yet
#   -re_tracking_and_TP   -- not implemented yet
#   -digi2track_and_TP   -- not implemented yet
#   -harvesting
#   -preproduction   -- not implemented yet
#   -comparison_only   -- not implemented yet -- something is broken

#Sequence = 'comparison_only'
Sequence = 'harvesting'

# castro path to pre harvested files 
#castorHarvestedFilesDirectory='/castor/cern.ch/user/n/nuno/relval/harvest/'
castorHarvestedFilesDirectory='/castor/cern.ch/cms/store/temp/dqm/offline/harvesting_output/mc/relval/'
    

# Debug variables ( for debugging purposes)
########################################################################

# Set to True if you want a detailed print out
verbose          = True
CfgFile          = 'trackingPerformanceValidation_cfg.py'
CompareMacroName = 'TrackValHistoPublisherNew'
MacroPath        = 'macro'


###################################################################################################
###################################################################################################

# script implementation

# a class to keep track of the parameters to build the cfg's and make and label the plots
# ---------------------------------------------------------------------------------------
class Sample:
    # constructors
    def __init__(
        self, 
        newRelease, 
        refRelease, 
        channel, 
        algorithm, 
        quality, 
        collection, 
        newGlobalTag, 
        refGlobalTag, 
        pileUp, 
        newRepository, 
        refRepository, 
        newLabel, 
        refLabel, 
        version = '', 
        numEvents = -1
    ):
        self.newRelease     = newRelease
        self.refRelease     = refRelease
        self.channel        = channel
        self.algorithm      = algorithm
        self.quality        = quality
        self.newGlobalTag   = newGlobalTag
        self.refGlobalTag   = refGlobalTag
        self.collection     = collection
        self.pileUp         = pileUp
        self.newRepository  = newRepository
        self.refRepository  = refRepository
        self.newLabel       = newLabel
        self.refLabel       = refLabel
        self.version        = version 
        self.numEvents      = numEvents 
     
    # methods
    def cfgFileName(self):
        return self.channel + self.newGlobalTag 

    def cmsCfgFileName(self):
        return self.cfgFileName() + '.py'

    def cfgAlgo(self):
        if  ( self.algorithm == 'ootb' or self.algorithm == '' ):
            return ''
        else:
            return '\'' + self.algorithm + '\''

    def cfgQuality(self):
        if ( self.quality != '' ):
            return '\'' + self.quality + '\''
        else:
            return ''

    def newReleaseNumber(self):
        return self.newRelease.replace('CMSSW_', '')

    def refReleaseNumber(self):
        return self.refRelease.replace('CMSSW_', '')

    def rootMacroName(self):
        return self.cfgFileName() + '.C' 

    def newSelection(self):
        return self.newGlobalTag + '_' + self.pileUp + '_' + self.selection()

    def refSelection(self):
        return self.refGlobalTag + '_' + self.pileUp + '_' + self.selection()

    def newMacroLabel(self):
        result = self.newSelection()
        if( self.newLabel != '' ): result += ' ' + self.newLabel
        return result

    def refMacroLabel(self):
        result = self.refSelection()
        if( self.refLabel != '' ): result += ' ' + self.refLabel
        return result

    def selection(self):
        selection = ''
        if  ( self.quality == '' and self.algorithm == '' ):
            if(self.collection == 'generalTracks'): selection += 'ootb'
            else:                                   selection += self.collection
        elif( self.quality != '' and self.algorithm == '' ):
            selection += self.quality
        elif( self.quality == '' and self.algorithm != '' ): 
            selection += self.algorithm
        else:   
            if( self.algorithm == 'ootb'): selection += self.quality
            else:                          selection += self.quality + '_' + self.algorithm
        return selection

    def dataSetName(self):
        return self.channel + '__' + self.newRelease + '-' + self.newGlobalTag + '-' + self.version + '__GEN-SIM-RECO'
        
    def parentDataSetName(self):
        return self.channel + '__' + self.newRelease + '-' + self.newGlobalTag + '-' + self.version + '__GEN-SIM-RAW-DEBUG'

    def dbsSearchString(self):
        command = 'dbsql "find  dataset where dataset like *'
        if(self.version != ''): command += self.channel + '/' + self.newRelease + '_' + self.newGlobalTag + '*-' + self.version 
        else:                   command += self.channel + '/' + self.newRelease + '_' + self.newGlobalTag
        command += '*GEN-SIM-RECO* order by dataset.createdate"'
        command += '| grep ' + self.channel + '| grep -v test | tail -1 | cut -f1'
        return command

    def dbsParentsString(self, dataset):
        command = 'dbsql "find dataset.parent where dataset like '+ dataset +'"| grep ' + self.channel
        return command

    def dbsFilesString(self, dataset):
        command = 'dbsql "find file where dataset like '+ dataset +'"| grep ' + self.channel
        return command

    def dbsNumberString(self, dataset):
        command = 'dbsql "find block.numevents where dataset like '+ dataset +'"| grep ' + self.channel
        return command

    def dbsRunString(self, dataset):
        command = 'dbsql "find run where dataset like '+ dataset +'"| grep ' + self.channel
        return command

    def newRepositoryDir(self):
        folder_label = ''
        if  ( self.refLabel == '' and self.newLabel != '' ): folder_label = '_' + self.newLabel 
        elif( self.refLabel != '' and self.newLabel == '' ): folder_label = ''  
        elif( self.refLabel != '' and self.newLabel != '' ): folder_label = '_' + self.refLabel + '_vs_' + self.newLabel    
        else:                                                folder_label = ''
        return self.newRepository + '/' + self.newRelease + folder_label + '/' + self.newSelection() + '/' + self.channel 

    def refRepositoryDir(self):
        return self.refRepository + '/' + self.refRelease + '/' + self.refSelection() + '/' + self.channel 

    def refLocalDir(self):
        return self.refRelease + '/' + self.refSelection()

    def refFile(self):
        return 'val.' + self.channel + '.root'

    def newFile(self):
        return 'val.' + self.channel + '.root'

    def refFileWithPath(self):
        return self.refLocalDir() + '/' + self.refFile() 

    def harvestedFile(self, sequence):
        file = 'DQM_V0001_R000000001__'
        if  ( sequence == "harvesting"      ): file += self.newGlobalTag + '__' + self.channel + '__Validation.root'
        elif( sequence == "preproduction"   ): file += self.channel + '-' + self.newGlobalTag + '_preproduction_312-v1__GEN-SIM-RECO_1.root'
        elif( sequence == "comparison_only" ): file += self.channel + '__' + self.newRelease+ '-' + self.newGlobalTag + '-' + self.version + '__GEN-SIM-RECO_1.root'
        else:                                  file += self.newGlobalTag+ '__' + self.channel + '__Validation.root'
        return file

    def printOut(self, sequence, verbose = False):
        print "Channel: \t\t",          self.channel
        print "Algorithm: \t\t",        self.algorithm
        print "Quality: \t\t",          self.quality
        if(verbose):
            print "GlobalTag: \t\t",        self.newGlobalTag
            print "Pile up set to: \t",     self.pileUp
            if(self.version != ''): print "version: \t\t", self.version
            else:                   print "version: \t\t", self.version
            if(self.numEvents < 0): print "using all events in the dataset"
            else:                   print "number of events to process", self.numEvents
            print "harvested file is",      self.harvestedFile(sequence)


# a simple function to test the output of the members of the Sample class
# ---------------------------------------------------------------------------------------
def testSample(sample):
    # members
    print "members"
    print 'sample.newRelease\t\t'           + sample.newRelease
    print 'sample.refRelease\t\t'           + sample.refRelease
    print 'sample.channel\t\t\t'            + sample.channel
    print 'sample.algorithm\t\t'            + sample.algorithm
    print 'sample.quality\t\t\t'            + sample.quality
    print 'sample.collection\t\t'           + sample.collection
    print 'sample.newGlobalTag\t\t'         + sample.newGlobalTag
    print 'sample.refGlobalTag\t\t'         + sample.refGlobalTag
    print 'sample.pileUp\t\t\t'             + sample.pileUp
    print 'sample.newRepository\t\t'        + sample.newRepository
    print 'sample.refRepository\t\t'        + sample.refRepository
    print 'sample.newLabel\t\t\t'           + sample.newLabel
    print 'sample.refLabel\t\t\t'           + sample.refLabel
    print 'sample.version\t\t\t'            + sample.version
    print 'sample.numEvents\t\t'            , sample.numEvents
    
    # methods
    print "method"
    print 'sample.cfgFileName()\t\t'        + sample.cfgFileName()
    print 'sample.newSelection()\t\t'       + sample.newSelection()
    print 'sample.refSelection()\t\t'       + sample.refSelection()
    print 'sample.newMacroLabel()\t\t'      + sample.newMacroLabel()
    print 'sample.refMacroLabel()\t\t'      + sample.refMacroLabel()
    print 'sample.selection()\t\t'          + sample.selection()
    print 'sample.dbsSearchString()\t'      + sample.dbsSearchString()
    print 'sample.dbsFilesString()\t'       + sample.dbsFilesString()
    #print 'sample.dbsParentsString()\t'     + sample.dbsParentsString()
    #print 'sample.newRepositoryDir()\t'     + sample.newRepositoryDir()
    print 'sample.refRepositoryDir()\t'     + sample.refRepositoryDir()
    print 'sample.harvestedFile("harvesting"     )\t\t'      + sample.harvestedFile("harvesting")
    print 'sample.harvestedFile("preproduction"  )\t\t'      + sample.harvestedFile("preproduction")
    print 'sample.harvestedFile("comparison_only")\t\t'      + sample.harvestedFile("comparison_only")
    print 'sample.refFile()\t\t'            + sample.refFile()
    print 'sample.newFile()\t\t'            + sample.newFile()

# ------------------------------------------------------------------------------------------------ #
# ------------------------------------------------------------------------------------------------ #

# Stand Alone Functions 
# ------------------------------------------------------------------------------------------------ #

# system includes
import os
import sys 
import fileinput
import string

# functions

# some constants
success = True
fail    = False

# check that the parameters are set properly
# ------------------------------------------------------------------------------------------------ #

# CMSSW enviroment
def checkCMSSWEnvironment():
    try:
        os.environ["CMSSW_VERSION"]
    except KeyError:
        raise ValueError('CMSSW environment variables are not available.  Please run cmsenv')
    

# Release parameters
def checkReleaseParameters():
    global NewRelease, RefRelease

    # Set to the current release if unset
    if( NewRelease == '' ):
        NewRelease = os.environ["CMSSW_VERSION"]
        print 'Warning: The new release is not set.  Using current CMSSW enviorment (' + NewRelease + ').'
    
    # Set to the new release if unset
    if( RefRelease == '' ):
        RefRelease = NewRelease 
        print 'Warning: The reference release is not set.  Using new release (' + NewRelease + ').'
    
# Release parameters
def checkPileUpParameters():
    global PileUp

    if( PileUp != 'PU' and PileUp != 'noPU' ):
        raise ValueError('PileUp variable invalid!  Available Values: PU, noPU')


# ensure that there are datasets to run on
def checkChannelParameters():
    global idealsamples, startupsamples 

    if( len(idealsamples) < 1 and len(startupsamples) < 1 ):
        raise ValueError('No sample to run on.  Please add either ideal and/or startup samples.')
    

# global tag parameter
def checkGlobalTagParameters():
    global IdealTag, StartupTag, RefIdealTag, RefStartupTag  

    if( IdealTag   == '' ): raise ValueError('The ideal global tag is blank.')
    if( StartupTag == '' ): raise ValueError('The startup global tag is blank.')

    if( RefIdealTag   == '' ): 
        print 'The reference ideal global tag is blank.  Using the new ideal GlobalTag'
        RefIdealTag = IdealTag
    if( RefStartupTag == '' ): 
        print 'The reference startup global tag is blank.  Using the new startup GlobalTag'
        RefStatupTag = StatupTag


# ensure that there are datasets to run on
def checkRepositoryParameters():
    global NewRepository, RefRepository 

    if( os.path.isdir(NewRepository) == False ):
        raise ValueError('New repository invalid! ' + NewRepository + ' not found!')

    if( os.path.isdir(RefRepository) == False ):
        raise ValueError('Ref repository invalid! ' + RefRepository + ' not found!')


# ensure that the Sequence parameter is correct 
def checkSequenceParameter():
    global Sequence 

    values = [ 
        #'only_validation',
        #'re_tracking',
        #'digi2track',
        #'only_validation_and_TP',
        #'re_tracking_and_TP',
        #'digi2track_and_TP',
        'harvesting',
        #'preproduction',
        'comparison_only'
    ]

    error = 'Sequence parameter invalid!  please choose from: '
    for value in values: error += ' ' + value + ','
    error += '\b.'

    if( Sequence not in values): 
        raise ValueError(error)


# check the parameters from Param.py 
def checkParameters():
    try:
        checkCMSSWEnvironment()
        checkReleaseParameters()
        checkPileUpParameters()
        checkChannelParameters()
        checkRepositoryParameters()
        checkGlobalTagParameters()
        checkSequenceParameter()
    except ValueError, e:
        print '\n***********************************************************************************'
        print 'Value Exception!!'
        print 'Exception occurred: ', e
        print '***********************************************************************************\n'
        sys.exit()


# copy files from castor 
# ------------------------------------------------------------------------------------------------ #

def copyFileFromCastor( dir, sample, sequence ):
    #numEvents = os.system('dbsql'
    path = dir + sample.newReleaseNumber() + '/' + sample.dataSetName() + '/run_1/nevents_9000'
    copy_command = 'rfcp ' + path + '/' + sample.harvestedFile(sequence) + ' .'
    #copy_command = 'rfcp ' + dir + sample.newReleaseNumber() + '/' + sample.harvestedFile(sequence) + ' .'
    if(verbose): print "Copying from castor to working directory: \n", copy_command

    return_code = os.system(copy_command)
    if( return_code != 0 ): 
        print 'Copy of harvested file from castor for sample ' + sample.channel + ' failed!'
        if(verbose): print 'Return code = ', return_code
    return return_code
        

# get the ROOT file for the new release
# ------------------------------------------------------------------------------------------------ #
def getNewFile(sample, sequence):
    global castorHarvestedFilesDirectory

    # if the file is already in the working directoy, use this one
    if( os.path.isfile( sample.harvestedFile(sequence) ) == True ):
        if(verbose): print "File already in working directory.  Using this one"
        return success

    if  ( sequence == 'comparison_only'):
        result = copyFileFromCastor( castorHarvestedFilesDirectory, sample, sequence ) == 0 
        if(verbose): print "result\t", result
        return result
    elif( sequence == 'harvesting'):
        if ( prepareCmsCfg(sample,sequence) == fail ) : return fail
        if ( runCmsJob(sample,sequence) == fail ) : return fail
        return success  
    else: 
        return fail


# get the ROOT file for the reference release
# ------------------------------------------------------------------------------------------------ #
def getRefFile(sample):

    if( os.path.exists(sample.refLocalDir())==False ): 
        os.system( 'mkdir -p ' + sample.refLocalDir() ) 

    copy_command = 'cp ' + sample.refRepositoryDir() + '/' + sample.refFile() + ' ' + sample.refLocalDir() + '/.'
    if(verbose): print "Copying from afs to working directory: \n", copy_command

    result = os.system(copy_command) == 0
    if( result == fail ): 
        print 'Copy of reference file for ' + sample.channel + ' failed!'
        if(verbose): print 'Return code = ', result
    return result


# prepare the cms config file
# ------------------------------------------------------------------------------------------------ #
def prepareCmsCfg(sample,sequence):
    global CfgFile

    # search for the primary dataset
    dataset = os.popen(sample.dbsSearchString()).readline().strip()
    if(verbose):
        print 'DataSet:  ', dataset
        print 'search command: ', sample.dbsSearchString()
    if( dataset == "" ):
        print 'No dataset found, skipping sample, ' + sample.channel
        return fail

    command   = sample.dbsFilesString(dataset) 
    filenames ='import FWCore.ParameterSet.Config as cms\n'
    filenames+='readFiles = cms.untracked.vstring()\n'
    filenames+='secFiles = cms.untracked.vstring()\n'
    filenames+='source = cms.Source ("PoolSource", fileNames = readFiles, secondaryFileNames = secFiles)\n'
    filenames+='readFiles.extend( [\n'
    first=True
    if( verbose ): print command 
    for line in os.popen(command).readlines():
       filename=line.strip()
       if first==True:
           filenames+="'"
           filenames+=filename
           filenames+="'"
           first=False
       else :
           filenames+=",\n'"
           filenames+=filename
           filenames+="'"
    filenames+=']);\n'

    # secondary file names
    if( Sequence == "preproduction" ):
        filenames += 'secFiles.extend( (       ) )'
    else:
        command = sample.dbsParentsString(dataset) 
        if( verbose ): print command 
        parentdataset = os.popen(command).readline()
        if( verbose ): print 'Parent DataSet:  ', parentdataset + '  length = ', len(parentdataset), '\n'
        if len(parentdataset) != 0:
            command = sample.dbsFilesString(parentdataset)
            if( verbose ): print command 
            filenames += 'secFiles.extend( [\n'
            first = True
            for line in os.popen(command).readlines():
               secfilename = line.strip()
               if first == True:
                   filenames += "'"
                   filenames += secfilename
                   filenames += "'"
                   first = False
               else :
                   filenames += ",\n'"
                   filenames += secfilename
                   filenames += "'"
            filenames += '\n]);\n'
        else:
            if( verbose ): "No parent dataset found skipping sample: ", sample.channel  

    # create cfg file
    symbol_map = { 
        'NEVENT'    : sample.numEvents, 
        'GLOBALTAG' : sample.newGlobalTag, 
        'SEQUENCE'  : sequence, 
        'SAMPLE'    : sample.channel, 
        'ALGORITHM' : sample.cfgAlgo(), 
        'QUALITY'   : sample.cfgQuality(),
        'TRACKS'    : sample.collection 
    }
    
    cfgFile = open(sample.cmsCfgFileName(), 'w' )
    cfgFile.write(filenames)
    cfgFile.close()

    replace( symbol_map, CfgFile, sample.cmsCfgFileName(), 'a' )
    return success


# Run the cms Job 
# ------------------------------------------------------------------------------------------------ #
def runCmsJob(sample, sequence):
    command = 'nohup cmsRun ' + sample.cmsCfgFileName() + ' >&  ' + sample.cfgFileName() + '.log < /dev/zero '
    if(verbose): print "running:", command
    result = os.system(command) == 0
    if(verbose): print "result of cmsRun", result
    return result


# copy the sub directory of the needed histograms
# ------------------------------------------------------------------------------------------------ #
def copySubDir(sample, sequence):

    algo_map = {}
    if( sample.quality == 'highPurity' ):
        algo_map = {
            'ootb' :'cutsRecoHp_AssociatorByHitsRecoDenom',
            'iter0':'cutsRecoZeroHp_AssociatorByHitsRecoDenom',
            'iter1':'cutsRecoFirstHp_AssociatorByHitsRecoDenom' ,
            'iter2':'cutsRecoSecondHp_AssociatorByHitsRecoDenom',
            'iter3':'cutsRecoThirdHp_AssociatorByHitsRecoDenom',
            'iter4':'cutsRecoFourthHp_AssociatorByHitsRecoDenom',
            'iter5':'cutsRecoFifthHp_AssociatorByHitsRecoDenom'
        }
    else:
        algo_map = {
            'ootb' :'general_AssociatorByHitsRecoDenom',
            'iter0':'cutsRecoZero_AssociatorByHitsRecoDenom',
            'iter1':'cutsRecoFirst_AssociatorByHitsRecoDenom',
            'iter2':'cutsRecoSecond_AssociatorByHitsRecoDenom',
            'iter3':'cutsRecoThird_AssociatorByHitsRecoDenom',
            'iter4':'cutsRecoFourth_AssociatorByHitsRecoDenom',
            'iter5':'cutsRecoFifth_AssociatorByHitsRecoDenom'
        }

    print algo_map[sample.algorithm]
    root_command =  'root -b -q -l CopySubdir.C\\(' \
                    + '\\\"' + sample.harvestedFile(sequence) + '\\\",\\\"' \
                    + sample.refFile() + '\\\",\\\"' \
                    + algo_map[sample.algorithm] + '\\\"\\)>& /dev/null'
    os.system(root_command)


# get the create a new file with the values from the map from the ref_file
# ------------------------------------------------------------------------------------------------ #

def replace(map, template, new, write_option = 'w'):
    outputFile = file(new, write_option)

    data = file(template,'r').read() 
    for key, value in map.items():
        if( not value) : value = ''
        data = data.replace(key, str(value))

    outputFile.write(data)
    outputFile.close()


# run the ROOT macro that builds the comparison plots
# ------------------------------------------------------------------------------------------------ #
def runComparisonMacro(sample):
    global CompareMacro

    if(verbose):
        print 'comparing plots from to', sample.refFileWithPath()

    mineff  = '0.0'
    maxeff  = '1.0'
    minfake = '0.0'
    maxfake = '0.8'
    if( sample.algorithm == 'iter0' or sample.algorithm == 'ootb' ):
        mineff  = '0.5'
        maxeff  = '1.025'
        minfake = '0.0'
        maxfake = '0.8'

    macro_map = {}
    if ( os.path.isfile(sample.refFileWithPath()) ):
        macro_map = {
            'NEW_FILE'       : sample.newFile(),
            'REF_FILE'       : sample.refFileWithPath(),
            'NEW_LABEL'      : sample.channel,
            'REF_LABEL'      : sample.channel,
            'NEW_RELEASE'    : sample.newRelease,
            'REF_RELEASE'    : sample.refRelease,
            'NEWSELECTION'   : sample.newMacroLabel(),
            'REFSELECTION'   : sample.refMacroLabel(),
            CompareMacroName : sample.cfgFileName(),
            'MINEFF'         : mineff,
            'MAXEFF'         : maxeff,
            'MINFAKE'        : minfake,
            'MAXFAKE'        : maxfake
        }
    else:
        print "No reference file found at: ", RefRelease+'/'+RefSelection
        macro_map = {
            'NEW_FILE'       : sample.newFile(), 
            'REF_FILE'       : sample.newFile(), 
            'NEW_LABEL'      : sample.channel,
            'REF_LABEL'      : sample.channel,
            'NEW_RELEASE'    : sample.newRelease,
            'REF_RELEASE'    : sample.refRelease,
            'NEWSELECTION'   : sample.newMacroLabel(),
            'REFSELECTION'   : sample.refMacroLabel(),
            CompareMacroName : sample.cfgFileName(),
            'MINEFF'         : mineff,
            'MAXEFF'         : maxeff,
            'MINFAKE'        : minfake,
            'MAXFAKE'        : maxfake
        }

    CompareMacro = MacroPath + '/' + CompareMacroName + '.C'
    replace(macro_map, CompareMacro, sample.rootMacroName())
    print CompareMacroName
    os.system('root -b -q -l '+ sample.rootMacroName() + '+ >  macro.' + sample.cfgFileName() + '.log')


# process an individual sample
# ------------------------------------------------------------------------------------------------ #
def moveFiles(sample, sequence):
    if( os.path.isdir(sample.newRepositoryDir()) == False ):
        os.system('mkdir -p ' + sample.newRepositoryDir() ) 
    
    if( os.path.isfile("building.pdf") ):
        print "moving pdf files for sample: " , sample.channel
        os.system('mv *.pdf ' + sample.newRepositoryDir() + '/')    

    if( os.path.isfile(sample.newFile()) ):
        print "moving root file for sample: " , sample.channel
        os.system('mv ' + sample.newFile() + '  ' + sample.newRepositoryDir() + '/.')   

    if( os.path.isfile(sample.cmsCfgFileName()) ):
        print "copy py file for sample: " , sample.channel
        os.system('cp ' + sample.cmsCfgFileName() + ' ' + sample.newRepositoryDir() + '/.') 

# process an individual sample
# ------------------------------------------------------------------------------------------------ #
def appendList(sample):
    if( os.path.isfile( "listofdatasets.txt" ) ):
        listofdatasets = open('listofdataset.txt' , 'w' )
    else:
        listofdatasets = open('listofdataset.txt' , 'a' )
    listofdatasets.write(sample.dataSetName() + '\n')
    listofdatasets.write(sample.parentDataSetName() + '\n')


# process an individual sample
# ------------------------------------------------------------------------------------------------ #
def processSample(sample, sequence):
    global verbose

    print "Processing"
    #testSample(sample)
    sample.printOut(Sequence, verbose)
    print ""

    if( os.path.isfile( sample.newRepositoryDir() + "/building.pdf" ) ):
        print 'validation for sample ' + sample.channel + ' (' + sample.selection() + ') ' + 'has already been done.  Skipping!\n'
        return;

    if( getNewFile(sample, sequence) == fail ):
        print "Failed to find/create the new harvested file. Skipping this channel"
        return 

    if( getRefFile(sample) == fail ):
        print "Failed to find the reference harvested file. Using the new havested file!"
        os.system( 'cp ' + sample.harvestedFile(sequence) + ' ' + sample.refLocalDir() + '/' + sample.refFile() )

    copySubDir(sample,sequence)
    runComparisonMacro(sample)
    moveFiles(sample, sequence)
    appendList(sample)
    print "Complete!\n"


# process all the samples for each algorithm and quality  
# ------------------------------------------------------------------------------------------------ #
def doValidation():
    
    print "Creating Validation Plots for", NewRelease + NewSelectionLabel, "with respect to", RefRelease + RefSelectionLabel, "\b."
    if(verbose):
        print "The sequence is set to", Sequence, "\b.\n"

    #if( os.path.isfile( "listofdatasets.txt" ) ): 
    #    os.system('rm listofdataset.txt')
    
    for algo in Algos:
        for quality in Qualities:
            if(verbose):
                if(quality == ''):  print "\trunning validation for", algo
                else:               print "\trunning validation for", algo, "with", quality
                print ""

            # build the list of samples 
            samples = []
            for channel in idealsamples: 
                samples.append( 
                    Sample(
                        NewRelease, 
                        RefRelease, 
                        channel, 
                        algo, 
                        quality, 
                        TrackCollection, 
                        IdealTag, 
                        RefIdealTag, 
                        PileUp, 
                        NewRepository, 
                        RefRepository, 
                        NewSelectionLabel, 
                        RefSelectionLabel, 
                        NewVersion,
                        #numEventsMap[sample]
                    )
                )
            for channel in startupsamples: 
                samples.append( 
                    Sample(
                        NewRelease, 
                        RefRelease, 
                        channel, 
                        algo, 
                        quality, 
                        TrackCollection, 
                        StartupTag, 
                        RefStartupTag, 
                        PileUp, 
                        NewRepository, 
                        RefRepository, 
                        NewSelectionLabel, 
                        RefSelectionLabel, 
                        NewVersion,
                         #numEventsMap[sample]
                    )
                )
            
            # process the channel
            for sample in samples: 
                processSample(sample, Sequence)



# ------------------------------------------------------------------------------------------------ #
# ------------------------------------------------------------------------------------------------ #

# run validation code 
# ------------------------------------------------------------------------------------------------ #

checkParameters()
doValidation()


