#!/usr/bin/env python

import sys
import string

class FileInfo:
    '''Stores info about a given file within a run.
    '''
    def __init__(self,infotext=None):
        self.fileName=None
        self.numEvents=0
        self.lumiBlock=0
        self.dataset=None
        self.goodFile=False
        self.infotext=string.split(infotext,",")
        if len(self.infotext)>0:
            self.fileName=self.infotext[0]
        if len(self.infotext)>1:
            self.numEvents=string.atoi(self.infotext[1])
        if len(self.infotext)>2:
            self.lumiBlock=string.atoi(self.infotext[2])
        if len(self.infotext)>3:
            self.dataset=self.infotext[3]
            self.goodFile=True
        return
        

class DBSRun:
    '''
    Stores information about a given run
    (Run number, files in run, whether DQM has been performed on run.)
    '''
    
    def __init__(self,filelist=None,lumiblocks=None,debug=False):
        
        #'''
        #Class stores all files associated with a given run number, as
        #given by DBS.  Also stores local DQM status, checking whether
        #DQM has run on the set of files, and whether it has successfully
        #completed.
        #'''
        self.debug=debug
        
        self.runnum=-1
        self.dataset=None
        self.files=[] # Stores files associated with run
        if (filelist<>None):
            self.files=filelist

        self.fileInfo=[]

        self.totalEvents=0
        self.ignoreRun=0 # if true, DQM won't be performed on run
        self.startedDQM=0 # set to true once DQM started 
        self.finishedDQM=0 # set to true once DQM finished (DQM directory for run exists)
        self.previouslyFinishedDQM=0 # This is no longer being used -- we can probably remove it completely
        self.maxEvents=1000 # number of events to run over with DQM

        # The following variables are used for searching only over certain lumi blocks
        self.numLumiBlocks=0
        self.lumiBlockIncrement=0
        self.currentLumiBlock=1
        
        return

    def AddFileInfo(self,FileInfo):
        ''' Adds run info from File to lists.'''
        if (FileInfo.goodFile==True):
            self.fileInfo.append(FileInfo)
        self.files.append(FileInfo.fileName)
        #self.UpdateRunInfo()
        return

    def UpdateRunInfo(self):
        ''' Finds largest luminosity block and updates total number of events.'''
        tempevents=0
        
        for i in self.fileInfo:
            if (i.fileName not in self.files):
                self.files.append(i.fileName)
            if (self.dataset==None):
                self.dataset=i.dataset
            elif (self.dataset<>i.dataset):
                print "<pyDBSRunClass:  UpdateRunInfo WARNING>:  datasets do not agree!"
                print "\tRun dataset = ",self.dataset
                print "\tFile dataset = ",i.dataset
            if i.lumiBlock>self.numLumiBlocks:
                self.numLumiBlocks=i.lumiBlock
            tempevents=tempevents+i.numEvents
        self.totalEvents=tempevents
        return
    

    def Print(self,screenoutput=False):
        '''
        Returns DBSRun class variable values as a string.
        '''
        mylen=0
        if (self.files<>None):
            mylen=len(self.files)
        x= "%10s     %35s     %10s     %14s%12s%15s%15s     %20s\n"%(self.runnum,
                                                                self.dataset,
                                                                mylen,
                                                                self.totalEvents,
                                                                self.ignoreRun,
                                                                self.startedDQM,
                                                                self.finishedDQM,
                                                                #self.previouslyFinishedDQM
                                                            self.currentLumiBlock
                                                       )
        if screenoutput:
            print "%10s     %55s     %10s%12s%15s%15s\n"%("Run","Dataset",
                                                          "# files","Ignore",
                                                          "Start","End")
            print x
        return x

    def Print2(self,screenoutput=False):
        '''
        Print command to be used within listbox.
        '''

        # Try to make everything as uniformly-aligned as possible:
        if (self.ignoreRun==0):
            self.ignoreRun=False
        elif (self.ignoreRun==1):
            self.ignoreRun=True
        if (self.startedDQM==0):
            self.startedDQM=False
        if (self.startedDQM==1):
            self.startedDQM=True
        if (self.finishedDQM==0):
            self.finishedDQM=False
        if (self.finishedDQM==1):
            self.finishedDQM=True

        # Set value strings so that they all have the same length
        # (This doesn't mean that they will look completely aligned,
        #  since different characters take up different amounts of space.
        #  What's a good way around this?
        
        run=`self.runnum`
        ignore=`self.ignoreRun`
        mylen=`0`
        if self.files<>None:
            mylen=`len(self.files)`
        start=`self.startedDQM`
        end=`self.finishedDQM`
        dataset=self.dataset
        while len(run)<10:
            run=" "+run
        while (len(mylen)<20):
            mylen=" "+mylen
        while len(ignore)<20:
            ignore=" "+ignore
        while len(start)<20:
            start=" "+start
        while len(end)<20:
            end=" "+end

        # Create string to be printed in listbox
        temp = "%10s%20s%30s%25s%20s%20s%s"%(run,start,end,ignore,mylen," ",dataset)
        if screenoutput:
            print "%10s%20s%30s%25s%20s%20s%s"%("Run","Start","End","Ignore","# files"," ","Dataset")
            print temp
            print "\tLumi block variables: "
            print "\t\t# lumi blocks in file: %s"%self.numLumiBlocks
            print "\t\tlumi block increment: %s"%self.lumiBlockIncrement 
            print "\t\tcurrent lumi block: %s"%self.currentLumiBlock


        return temp
    
def dbsSort(x,y):
    '''
    Sorts DBSRun objects by run number.
    '''
    return x.runnum>y.runnum





##########################################
# testing program

if __name__=="__main__":

    print "Testing creation of a DBSRun class:"
    temp=[]
    if len(sys.argv)>1:
        temp=[DBSRun(sys.argv[1:])]
    else:
        temp=[DBSRun()]
    for i in temp:
        print "\n Displaying 'Print' output of DBSRun instance:\n"
        i.Print(screenoutput=1)
        print "\n\n Displaying 'Print2' output of DBSRun instance:\n"
        i.Print2(screenoutput=1)

    print "\n\nThat's all, folks!"
