#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Usage: dumpDataFile.sh  [*.root]  [max # of events]"
    exit 
fi

FILENAME=$1
MAXNEVENTS=$2

echo "Dumping "${MAXNEVENTS}" events from "${FILENAME}

CFGFILE=${PWD}/dumpDataFile.cfg
cat > ${CFGFILE}<<EOF

process Dump =
{
  untracked PSet maxEvents = {untracked int32 input =${MAXNEVENTS}}
  source = PoolSource
  { untracked vstring fileNames = {"file:${FILENAME}"}}
  module dump = EventContentAnalyzer{}
  path p = { dump }
}
EOF

cmsRun ${CFGFILE}
