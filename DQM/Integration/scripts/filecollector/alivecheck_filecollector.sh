#! /bin/sh

export WorkDir=/nfshome0/dqmpro/filecollector
export YourEmail=lilopera@cern.ch
source $WorkDir/env.sh

#set ROOT environment
#export ROOTSYS=/nfshome0/cmssw2/slc4_ia32_gcc345/lcg/root/5.18.00a-cms11/
#export ROOT_DIR=${ROOTSYS}
#export LD_LIBRARY_PATH=${ROOTSYS}/lib
#export PATH=${ROOTSYS}/bin:${PATH}


EXE=$WorkDir/filecollector.py
RUN_STAT=`ps -ef | grep filecollector.py | grep -v grep | wc | awk '{print $1}'`
#DOG_STAT=`ps -ef | grep alivecheck_filesave.sh | grep -v grep | wc | awk '{print $1}'`

#if [ $DOG_STAT -gt 10 ]
#then
#    echo watchdog script seems to have some trouble at $HOSTNAME. | mail Hyunkwan.Seo@cern.ch
#    exit 0
#fi

if [ $RUN_STAT -ne 0 ]
then
    echo filecollector.py is running at $HOSTNAME.
else
    echo filecollector.py stopped by unknown reason and restarted now.
    LOG=$WorkDir/log/LOG.filesave.$HOSTNAME.$$
    date >& $LOG
    echo filecollector.py stopped by unknown reason and restarted at $HOSTNAME. >> $LOG
    $EXE >> $LOG & 
    echo filecollector.py stopped by unknown reason and restarted now at $HOSTNAME. | mail $YourEmail
fi
