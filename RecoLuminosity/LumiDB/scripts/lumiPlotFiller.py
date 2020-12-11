#! /usr/bin/env python
import os,os.path,sys,commands,time,datetime,shutil
import coral
from RecoLuminosity.LumiDB import argparse,lumiQueryAPI,lumiTime,csvReporter
###
#Script to fill the lumi monitoring site. This is not a generic tool
###
def findFileTrueName(filename):
    '''given a filename, find its true name
    '''
    truename=filename
    if os.path.islink(filename):
        truename=os.path.realpath(filename)
    else:
        print '[WARNING] ',filename,' is not a link'
    return truename
def createRunList(c,p='.',o='.',dryrun=False):
    '''
     input:
      c connect string
      p authenticaion path
    '''
    msg=coral.MessageStream('')
    msg.setMsgVerbosity(coral.message_Level_Error)
    os.environ['CORAL_AUTH_PATH']='/afs/cern.ch/cms/DB/lumi'
    svc = coral.ConnectionService()
    connectstr=c
    session=svc.connect(connectstr,accessMode=coral.access_ReadOnly)
    session.typeConverter().setCppTypeForSqlType("unsigned int","NUMBER(10)")
    session.typeConverter().setCppTypeForSqlType("unsigned long long","NUMBER(20)")
    session.transaction().start(True)
    schema=session.nominalSchema()
    allruns=lumiQueryAPI.allruns(schema,requireLumisummary=True,requireTrg=True,requireHlt=True)
    session.transaction().commit()  
    del session
    del svc
    allruns.sort()
    if not dryrun:
        report=csvReporter.csvReporter(os.path.join(o,'runlist.txt'))
        for run in allruns:
            report.writeRow([run])
    else:
        print allruns
def totalLumivstime(c,p='.',i='',o='.',begTime="03/30/10 10:00:00.00",endTime=None,selectionfile=None,beamstatus=None,beamenergy=None,beamfluctuation=None,dryrun=False,withTextOutput=False,annotateBoundaryRunnum=False):
    '''
    input:
      c connect string
      p authenticaion path
      i input selection file name
      o output path
    '''
    plotoutname='totallumivstime.png'
    textoutname='totallumivstime.csv'
    elements=['lumiSumPlot.py','-c',c,'-P',p,'-begin','"'+begTime+'"','-batch',os.path.join(o,plotoutname),'time']
    if selectionfile:
        elements.append('-i')
        elements.append(selectionfile)
    if endTime:
        elements.append('-end')
        elements.append('"'+endTime+'"')
    if beamstatus:
        elements.append('-beamstatus')
        if beamstatus=='stable':
            elements.append('"STABLE BEAMS"')
    if beamenergy:
        elements.append('-beamenergy')
        elements.append(str(beamenergy))
    if beamfluctuation:
        elements.append('-beamfluctuation')
        elements.append(str(beamfluctuation))   
    if withTextOutput:
        elements.append('-o')
        elements.append(os.path.join(o,textoutname))
    if annotateBoundaryRunnum:
        elements.append('--annotateboundary')
    command=' '.join(elements)
    print command
    if not dryrun:
        statusAndOutput=commands.getstatusoutput(command)
        print statusAndOutput[1]
def totalLumivstimeLastweek(c,p='.',i='',o='.',selectionfile=None,beamstatus=None,beamenergy=None,beamfluctuation=None,dryrun=False,withTextOutput=False):
    '''
    input:
      c connect string
      p authenticaion path
      i input selection file name
      o output path
      ##fix me: year boundary is not considered!
    '''
    t=lumiTime.lumiTime()
    plotoutname='totallumivstime-weekly.png'
    textoutname='totallumivstime-weekly.csv'
    nowTime=datetime.datetime.now()
    lastMondayStr=' '.join([str(nowTime.isocalendar()[0]),str(nowTime.isocalendar()[1]-1),str(nowTime.isocalendar()[2])])
    
    lastweekMonday=datetime.datetime(*time.strptime(lastMondayStr,'%Y %W %w')[0:5])
    lastweekEndSunday=lastweekMonday+datetime.timedelta(days=7,hours=24)
    
    elements=['lumiSumPlot.py','-c',c,'-P',p,'-begin','"'+t.DatetimeToStr(lastweekMonday)+'"','-end','"'+t.DatetimeToStr(lastweekEndSunday)+'"','-batch',os.path.join(o,plotoutname),'time']
    if withTextOutput:
        elements.append('-o')
        elements.append(os.path.join(o,textoutname))
    if beamstatus:
        elements.append('-beamstatus')
        if beamstatus=='stable':
            elements.append('"STABLE BEAMS"')
    if beamenergy:
        elements.append('-beamenergy')
        elements.append(str(beamenergy))
    if beamfluctuation:
        elements.append('-beamfluctuation')
        elements.append(str(beamfluctuation))   
    command=' '.join(elements)
    print command
    if not dryrun:
        statusAndOutput=commands.getstatusoutput(command)
        print statusAndOutput[1]
        
def lumiPerDay(c,p='.',i='',o='',begTime="03/30/10 10:00:00.00",endTime=None,selectionfile=None,beamstatus=None,beamenergy=None,beamfluctuation=None,dryrun=False,withTextOutput=False,annotateBoundaryRunnum=False):
    '''
    input:
      c connect string
      p authenticaion path
      i input selection file
      o outputpath
    '''
    plotoutname='lumiperday.png'
    textoutname='lumiperday.csv'
    elements=['lumiSumPlot.py','-c',c,'-P',p,'-begin','"'+begTime+'"','-batch',os.path.join(o,plotoutname),'perday']
    if selectionfile:
        elements.append('-i')
        elements.append(selectionfile)
    if endTime:
        elements.append('-end')
        elements.append('"'+endTime+'"')
    if beamstatus:
        elements.append('-beamstatus')
        if beamstatus=='stable':
            elements.append('"STABLE BEAMS"')
    if beamenergy:
        elements.append('-beamenergy')
        elements.append(str(beamenergy))
    if beamfluctuation:
        elements.append('-beamfluctuation')
        elements.append(str(beamfluctuation))
    if withTextOutput:
        elements.append('-o')
        elements.append(os.path.join(o,textoutname))
    if annotateBoundaryRunnum:
        elements.append('--annotateboundary')
    command=' '.join(elements)
    print command
    if not dryrun:
        statusAndOutput=commands.getstatusoutput(command)
        print statusAndOutput[1]
def totalLumivsRun(c,p='.',i='',o='',begRun="132440",endRun=None,selectionfile=None,beamstatus=None,beamenergy=None,beamfluctuation=None,dryrun=False,withTextOutput=False):
    '''
    input:
      c connect string
      p authenticaion path
      i input selection file
      o outputpath 
    '''
    plotoutname='totallumivsrun.png'
    textoutname='totallumivsrun.csv'
    elements=['lumiSumPlot.py','-c',c,'-P',p,'-begin',begRun,'-batch',os.path.join(o,plotoutname),'run']
    if endRun:
        elements.append('-end')
        elements.append(endRun)
    if beamstatus:
        elements.append('-beamstatus')
        if beamstatus=='stable':
            elements.append('"STABLE BEAMS"')
    if beamenergy:
        elements.append('-beamenergy')
        elements.append(str(beamenergy))
    if beamfluctuation:
        elements.append('-beamfluctuation')
        elements.append(str(beamfluctuation))
    if withTextOutput:
        elements.append('-o')
        elements.append(os.path.join(o,textoutname))
    command=' '.join(elements)
    print command
    if not dryrun:
        statusAndOutput=commands.getstatusoutput(command)
        print statusAndOutput[1]

def totalLumivsFill(c,p='.',i='',o='',begFill="1005",endFill=None,selectionfile=None,beamenergy=None,beamstatus=None,beamfluctuation=None,dryrun=False,withTextOutput=False):
    '''
    input:
      c connect string
      p authenticaion path
      i input selection file
      o output path
    '''
    plotoutname='totallumivsfill.png'
    textoutname='totallumivsfill.csv'
    elements=['lumiSumPlot.py','-c',c,'-P',p,'-begin',begFill,'-batch',os.path.join(o,plotoutname),'fill']
    if endFill:
        elements.append('-end')
        elements.append(endFill)
    if beamstatus:
        elements.append('-beamstatus')
        if beamstatus=='stable':
            elements.append('"STABLE BEAMS"')
    if beamenergy:
        elements.append('-beamenergy')
        elements.append(str(beamenergy))
    if beamfluctuation:
        elements.append('-beamfluctuation')
        elements.append(str(beamfluctuation))
    if withTextOutput:
        elements.append('-o')
        elements.append(os.path.join(o,textoutname))
    command=' '.join(elements)
    print command
    if not dryrun:
        statusAndOutput=commands.getstatusoutput(command)
        print statusAndOutput[1]
def instLumiForRuns(c,runnumbers,p='.',o='',dryrun=False):
    '''
    draw instlumperrun plot for the given runs
    input:
      c connect string
      runnumbers []
      p authenticaion path
      o output path
    '''
    plotoutname='rollinginstlumi_'
    textoutname='rollinginstlumi_'
    for idx,run in enumerate(runnumbers):
        batch=os.path.join(o,plotoutname+str(idx+1)+'.png')
        elements=['lumiInstPlot.py','-c',c,'-P',p,'-begin',str(run),'-batch',batch,'run']
        command=' '.join(elements)
        print command
        if not dryrun:
            statusAndOutput=commands.getstatusoutput(command)
            print statusAndOutput[1]
def instPeakPerday(c,p='.',o='.',begTime="03/30/10 10:00:00.00",endTime=None,dryrun=False,withTextOutput=False,annotateBoundaryRunnum=False):
    '''
    input:
      c connect string
      p authenticaion path
      o outout text/csv file
    '''
    plotoutname='lumipeak.png'
    textoutname='lumipeak.csv'
    elements=['lumiInstPlot.py','-c',c,'-P',p,'-begin','"'+begTime+'"','-batch',os.path.join(o,plotoutname),'peakperday']
    if endTime:
        elements.append('-end')
        elements.append('"'+endTime+'"')
    if withTextOutput:
        elements.append('-o')
        elements.append(os.path.join(o,textoutname))
    if annotateBoundaryRunnum:
        elements.append('--annotateboundary')
    command=' '.join(elements)
    print command
    if not dryrun:
        statusAndOutput=commands.getstatusoutput(command)
        print statusAndOutput[1]
        
def main():
    actionlist=['instperrun','instpeakvstime','totalvstime','totallumilastweek','totalvsfill','totalvsrun','perday','createrunlist','physicsperday','physicsvstime']
    parser = argparse.ArgumentParser(prog=os.path.basename(sys.argv[0]),description="Produce lumi plots")
    parser.add_argument('-c',dest='connect',action='store',required=True,help='connect string to lumiDB')
    parser.add_argument('-P',dest='authpath',action='store',required=False,help='auth path. Optional. Default to .')
    parser.add_argument('-L',dest='logpath',action='store',required=False,help='log path. Optional. Default to .')
    parser.add_argument('-i',dest='ifile',action='store',required=False,help='input selection file. Optional.')
    parser.add_argument('-o',dest='opath',action='store',required=False,help='output file path. Optional')
    parser.add_argument('-beamenergy',dest='beamenergy',action='store',required=False,help='beamenergy (in GeV) selection criteria,e.g. 3.5e3')
    parser.add_argument('-beamfluctuation',dest='beamfluctuation',action='store',required=False,help='allowed beamenergy fluctuation (in GeV),e.g. 0.2e3')
    parser.add_argument('--annotateboundary',dest='annotateboundary',action='store_true',help='annotate boundary run numbers')
    parser.add_argument('-beamstatus',dest='beamstatus',action='store',required=False,help='selection criteria beam status,e.g. stable')
    parser.add_argument('--withTextOutput',dest='withtextoutput',action='store_true',help='write to text output file')
    parser.add_argument('--dryrun',dest='dryrun',action='store_true',help='dryrun mode')
    parser.add_argument('action',choices=actionlist,help='command actions')
    args=parser.parse_args()

    authpath='.'
    logpath='.'
    opath='.'
    connectstr=args.connect
    isDryrun=False
    withTextOutput=False
    beamstatus=None
    beamenergy=None
    beamfluctuation=None
    if args.beamenergy:
        beamenergy=args.beamenergy
    if args.beamstatus:
        beamstatus=args.beamstatus
    if args.beamfluctuation:
        beamfluctuation=args.beamfluctuation
    if args.dryrun:
        isDryrun=True
    if args.withtextoutput:
        withTextOutput=True
    if args.authpath:
        authpath=args.authpath
    if args.logpath:
        lumilogpath=args.logpath
    if args.ifile:
        ifile=args.ifile
    if args.opath:
        opath=args.opath
    if args.action == 'createrunlist':
        createRunList(connectstr,authpath,o=opath,dryrun=isDryrun)
    if args.action == 'instperrun':
        if not args.ifile:
            print 'option -i is required for action instperrun'
            return 2
        f=open(args.ifile,'r')
        runs=[]
        for run in f:
            runs.append(int(run))
        last2runs=[runs[-2],runs[-1]]
        instLumiForRuns(connectstr,last2runs,p=authpath,o=opath,dryrun=isDryrun)
    if args.action == 'totalvsrun':
        if args.ifile:
            f=open(args.ifile,'r')
            runs=[]
            for run in f:
                runs.append(int(run))
        else:
            runs=['132440','']
        totalLumivsRun(connectstr,p=authpath,begRun=str(runs[0]),o=opath,endRun=str(runs[-1]),beamstatus=beamstatus,beamenergy=beamenergy,beamfluctuation=beamfluctuation,dryrun=isDryrun,withTextOutput=withTextOutput)
    if args.action == 'instpeakvstime':
        instPeakPerday(connectstr,p=authpath,o=opath,dryrun=isDryrun,withTextOutput=withTextOutput,annotateBoundaryRunnum=args.annotateboundary)
    if args.action == 'totalvstime':
        totalLumivstime(connectstr,p=authpath,o=opath,beamstatus=beamstatus,beamenergy=beamenergy,beamfluctuation=beamfluctuation,dryrun=isDryrun,withTextOutput=withTextOutput,annotateBoundaryRunnum=args.annotateboundary)
    if args.action == 'totallumilastweek':
        totalLumivstimeLastweek(connectstr,p=authpath,o=opath,beamstatus=beamstatus,beamenergy=beamenergy,beamfluctuation=beamfluctuation,dryrun=isDryrun,withTextOutput=withTextOutput)
    if args.action == 'totalvsfill':
        totalLumivsFill(connectstr,p=authpath,o=opath,beamstatus=beamstatus,beamenergy=beamenergy,beamfluctuation=beamfluctuation,dryrun=isDryrun,withTextOutput=withTextOutput)
    if args.action == 'perday':       
        lumiPerDay(connectstr,p=authpath,o=opath,dryrun=isDryrun,beamstatus=beamstatus,beamenergy=beamenergy,beamfluctuation=beamfluctuation,withTextOutput=withTextOutput,annotateBoundaryRunnum=args.annotateboundary)
    if args.action == 'physicsperday' or args.action == 'physicsvstime':
        if not args.ifile:
            print 'input selection file is required'
            return 3
        if not os.path.isfile(args.ifile):
            print 'file: '+args.ifile+' does not exist'
            return 4
        truefilename=findFileTrueName(args.ifile)
        #print 'truefilename ',truefilename
        #
        #if truefilename modification time is more recent than the output plot, replot it
        #
        inputmodtime=os.path.getmtime(truefilename)
        #print 'inputmodtime ',inputmodtime
        if not os.path.isfile(os.path.join(opath,'lumiperday.png')) or not os.path.isfile(os.path.join(opath,'totallumivstime.png')):
            if not isDryrun:
                shutil.copy2(truefilename,os.path.join(opath,os.path.basename(truefilename)))
            print 'cp '+truefilename+' '+os.path.join(opath,os.path.basename(truefilename))
            if args.action == 'physicsperday':
                lumiPerDay(connectstr,p=authpath,selectionfile=truefilename,o=opath,beamstatus=beamstatus,beamenergy=beamenergy,beamfluctuation=beamfluctuation,dryrun=isDryrun,withTextOutput=withTextOutput)
            if args.action == 'physicsvstime':
                totalLumivstime(connectstr,p=authpath,selectionfile=truefilename,o=opath,beamstatus=beamstatus,beamenergy=beamenergy,beamfluctuation=beamfluctuation,dryrun=isDryrun,withTextOutput=withTextOutput)
        else:
            outputmodtime=os.path.getmtime(os.path.join(opath,'lumiperday.png'))
            #print 'outputmodtime ',outputmodtime
            if inputmodtime > outputmodtime :
                print 'physics selection file '+truefilename+' modified, updating physics plots: '
                if not isDryrun:
                    shutil.copy2(truefilename,os.path.join(opath,os.path.basename(truefilename)))
                    print 'cp '+truefilename+' '+os.path.join(opath,os.path.basename(truefilename))
                    if args.action == 'physicsperday':
                        lumiPerDay(connectstr,p=authpath,selectionfile=truefilename,o=opath,beamstatus=beamstatus,beamenergy=beamenergy,beamfluctuation=beamfluctuation,dryrun=isDryrun,withTextOutput=withTextOutput)
                    if args.action == 'physicsvstime':
                        totalLumivstime(connectstr,p=authpath,selectionfile=truefilename,o=opath,beamstatus=beamstatus,beamenergy=beamenergy,beamfluctuation=beamfluctuation,dryrun=isDryrun,withTextOutput=withTextOutput)
            else:
                print 'physics selection file older than plot, do nothing'
                return 0

if __name__=='__main__':
    main()
